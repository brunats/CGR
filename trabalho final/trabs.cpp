/**
*Trabalho final da disciplina de Computação Gráfica
*Apresentado em 2017.1
* 	Implementar algoritmo para controle de câmera permitindo que o usuário
*	se movimente em um ambiente virtual de diferentes maneiras ("voando",
*	caminhando,...), ou seja, fazer diferentes tipos de controle de câmera.
*
*Autores:
*	Bruna Tavares Silva
*	André Morotti
*
*Compile:
*	$gcc trabs.cpp -lglut -lGL -lGLU -lm -o trabsfinale
*	$./trabsfinale
**/
/*
Extras:
	cores rgb
		https://sistemas.riopomba.ifsudestemg.edu.br/dcc/materiais/926330044_Cores.pdf
	tutorial opengl
		http://www2.ic.uff.br/PosGraduacao/RelTecnicos/282.pdf
*/
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <math.h>
//#include <GLUT/glut.h>
#include <GL/glut.h>
 
// Variaveis de Rotacao
double rotate_y=0; 
double rotate_x=0.5;
//pos cam em x/y/z; olhar da cam em x/y/z
float px, py, pz, ox, oy, oz;
//considerado para cima da cam em x/y/z
int pCX, pCY, pCZ, aux;
GLUquadricObj *qobj;
GLfloat angle, fAspect;

float vx = sin(rotate_y * M_PI / 180) * 0.1;
float vy = 0;
float vz = cos(rotate_y * M_PI / 180) * 0.1;

float vxn = sin((180 - rotate_y) * M_PI / 180) * 0.1;
float vyn = 0;
float vzn = cos((180 - rotate_y) * M_PI / 180) * 0.1;

//Constantes de luminosidade
GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f }; 
 
const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f }; 

void teclado(unsigned char key, int x, int y);

GLfloat coordenadas(char ch, GLfloat k, GLfloat teta, GLfloat alfa){
	switch (ch){
		case 'x':
			return (k * cos(teta) * sin(alfa));
		break;

		case 'y':
			return (k * sin(teta) * sin(alfa));
		break;

		case 'z':
			return (k * cos(alfa));
		break;
	}
}

//abc: sao as constanstes que def o elipsoide, nteta: slices, nalfa: stacks;
GLvoid desenhaElipsoide(GLfloat a, GLfloat b, GLfloat c, GLfloat nteta, GLfloat nalfa){
	GLfloat x11, y11, z11, x12, y12, z12, x21, y21, z21, x22, y22, z22;
	GLfloat teta1, teta2, alfa1, alfa2;
	GLfloat pi = 3.14159;

	for(int i = 0; i < nteta; i++){
		teta1 = i * 2 * pi / nteta;
		teta2 = (i+1) * 2 * pi / nteta;

		for(int j = 0; j < nalfa; j++){
			alfa1 = j * 2 * pi / nalfa;
			alfa2 = (j+1) * 2 * pi / nalfa;

			x11 = coordenadas('x', a, teta1, alfa1);
			y11 = coordenadas('y', b, teta1, alfa1);
			z11 = coordenadas('z', c, -1, alfa1);

			x12 = coordenadas('x', a, teta1, alfa2);
			y12 = coordenadas('y', b, teta1, alfa2);
			z12 = coordenadas('z', c, -1, alfa2);

			x21 = coordenadas('x', a, teta2, alfa1);
			y21 = coordenadas('y', b, teta2, alfa1);
			z21 = coordenadas('z', c, -1, alfa1);

			x22 = coordenadas('x', a, teta2, alfa2);
			y22 = coordenadas('y', b, teta2, alfa2);
			z22 = coordenadas('z', c, -1, alfa2);

			glBegin(GL_QUADS);                 // renderizado, desenha quadriláteros preenchidos
				glNormal3f( (x11+x22)/2, (y11+y22)/2, (z11+z22)/2 ); // Normal de cada quadrilátero
				glVertex3f(x11,y11,z11);
				glVertex3f(x12,y12,z12);
				glVertex3f(x22,y22,z22);
				glVertex3f(x21,y21,z21);
			glEnd();
		}
	}
}

void desabilitaLuz(){
	glDisable(GL_LIGHT0);
  	glDisable(GL_LIGHTING);
}

void habilitaLuz(){
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

void desenhaSol(){
	//sol
	glPushMatrix();
		glColor3f (1.0f, 1.0f, 0.0f); //amarelo
        glTranslated(0.0,6,-18);
        glutSolidSphere(0.6,50,50);
    glPopMatrix(); 
}

void desenhaNuvens(){
	//nuvens
	/*
	glColor3f (0.53f, 0.81f, 0.92f); //azul céu
	glPushMatrix();
		glTranslated(-2,1,-5);
		desenhaElipsoide(0.4, 0.4, 0.8, 50, 50);
	glPopMatrix();
	*/

	//colocar para desabilitar faces invisiveis
	glColor3f (0.53f, 0.81f, 0.92f); //azul céu
	glPushMatrix();
		glPushMatrix();
	        glTranslated(-2,1,-3.7);
	        glutSolidSphere(0.2,50,50);
	    glPopMatrix();

	    glPushMatrix();
	        glTranslated(-2,1,-4);
	        glutSolidSphere(0.2,50,50);
	    glPopMatrix();

	    glPushMatrix();
	        glTranslated(-1.9,1.1,-3.9);
	        glutSolidSphere(0.15,50,50);
	    glPopMatrix();

	    glPushMatrix();
	        glTranslated(-2.1,0.9,-3.9);
	        glutSolidSphere(0.15,50,50);
	    glPopMatrix();
    glPopMatrix();
}

void desenhaChao(){
  	glPushMatrix();
  		glTranslated(0.0,4,-10);
		glColor3f (0.0f, 0.39f, 0.0f); //verde escuro
		glBegin(GL_QUADS);
			glVertex3f (-10.0f, -5.0f, -10.0f); //tras esq
			glVertex3f (10.0f, -5.0f, -10.0f); //tras dir
			glVertex3f (10.0f, -5.0f, 10.0f); //frente  dir
			glVertex3f (-10.0f, -5.0f, 10.0f); //frente esq
		glEnd();
	glPopMatrix();
}

void desenhaQueda(){
	glPushMatrix();
		glTranslated(0.0,4,-10);
		glColor3f (0.59f, 0.29f, 0.0f); // marrom procurar
		glBegin(GL_QUADS);
			glVertex3f (-10.0f, -5.0f, -10.0f); //frente esq
			glVertex3f (10.0f, -5.0f, -10.0f); //frente dir
			glVertex3f (10.0f, -10.0f, -16.0f); //fundo dir  dir
			glVertex3f (-10.0f, -10.0f, -16.0f); //fundo esq esq
		glEnd();
	glPopMatrix();
}

void desenhaCasa(){
	//addT(-0.5, 0.5);
	//addT(-0.5, 0.5);
	glPushMatrix();
		glColor3f( 0.6f, 0.4f, 0.8f ); //roxo claro
		glBegin(GL_QUADS);
			// Frente
			glVertex3f( -0.5,  -5,  0.5 );
			glVertex3f( -0.5,  -4,  0.5 );
			glVertex3f(  0.5,  -4,  0.5 );
			glVertex3f(  0.5,  -5,  0.5 );
			// Fundos
			glVertex3f( -0.5,  -5, -0.5 );
			glVertex3f(  0.5,  -5, -0.5 );
			glVertex3f(  0.5,  -4, -0.5 );
			glVertex3f( -0.5,  -4, -0.5 );	
			// Lado esquerdo
		  	glVertex3f( -0.5,  -5,  0.5 );
		  	glVertex3f( -0.5,  -4,  0.5 );
		  	glVertex3f( -0.5,  -4, -0.5 );
		  	glVertex3f( -0.5,  -5, -0.5 );
			// Lado direito
			glVertex3f(  0.5,  -5,  0.5 );
			glVertex3f(  0.5,  -5, -0.5 );
			glVertex3f(  0.5,  -4, -0.5 );
			glVertex3f(  0.5,  -4,  0.5 );
		glEnd();

		// Teto é uma pirâmide de base quadrangular
		glColor3f( 0.7f, 0.13f, 0.13f); //marrom "telhado"
		glBegin(GL_TRIANGLES);
			// Frente
		  	glVertex3f( -0.5,  -4,  0.5 );
		  	glVertex3f(  0,  -3,  0 );
		  	glVertex3f(  0.5,  -4,  0.5 );
			// Lado direito
		  	glVertex3f(  0.5,  -4,  0.5 );
		  	glVertex3f(  0.5,  -4, -0.5 );
		  	glVertex3f(  0,  -3,  0 );
			// Traseira
		  	glVertex3f(  0.5,  -4, -0.5 );
		  	glVertex3f( -0.5,  -4, -0.5 );
		  	glVertex3f(  0,  -3,  0 );
			// Lado esquerdo
		  	glVertex3f( -0.5,  -4,  0.5 );
		  	glVertex3f(  0,  -3,  0 );
		  	glVertex3f( -0.5,  -4, -0.5 );
		glEnd();
	glPopMatrix();
}

void moveCam(){
	if(aux == 1){
		px = 0; py = 0; pz == -0.1;
		ox = 0;	oy = 0;	oz = -20;
		pCX = 0;pCY = 1;pCZ = 0;
	}
	glTranslatef(px , py , pz);
	gluLookAt (0,0,0, ox, oy, oz, pCX , pCY , pCZ);
}

void display(){
	//z frente(profundidade); x lado(largura); y cima(altura)
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluPerspective(angle,fAspect,0.1,500);

	
	glScalef( rotate_x,rotate_x,1.0f );
	glPushMatrix();
		glRotatef(rotate_y, 0.0, 1.0, 0.0); //rotacao

		if(aux != 0)
			moveCam();
		desenhaSol();
		glEnable (GL_CULL_FACE);
		desenhaNuvens();
		glDisable (GL_CULL_FACE);
		desabilitaLuz();
			desenhaChao();
			desenhaQueda();
		habilitaLuz();
		glTranslated(-3,4,-10);
			desenhaCasa();
		glTranslated(3,0,0);
			desenhaCasa();
	glPopMatrix();
	
    glutSwapBuffers();
}

void inicializa(){ 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    angle=45;
}

void animacao(){
	while(pz<30){
		printf("a\n");
		px -= vx;
      	pz += vz;
      	py += 0.1;
		oy += 0.1;
		sleep(0.8);
		glutPostRedisplay();
	}
}

void teclado(unsigned char key, int x, int y){
	/*
		gluLookAt
		(tres 1º's: pos cam
		prox tres: pra onde olha)
		tres ult's: dir p/ cima
	*/ 
	if(key == 27 | key == 'q' | key == 'Q')
		exit(0);

	if(pz > 18 && (key == 'f' || key == 'F')){
		printf("entrooou\n");
		animacao();
	}else{
		printf("%f\n", pz);
		switch (key) {
			case 'f': case 'F': case 'w': case 'W':
				aux++;
				px -= vx;
	  			pz += vz;
				break;

			case 't': case 'T': case 's': case 'S':
				aux++;
				px += vx;
	    		pz -= vz;
				break;

			case 'e': case 'E': case 'a': case 'A':
				aux++;
				px -= vzn;
	  			pz += vxn;
	  			break;

			case 'd': case 'D':
				aux++;
				px += vzn;
	 			pz -= vxn;
				break;

			case 'c': case 'C':
				aux++;
				if(pCY){
					py += -0.1;
					oy += -0.1;
				}
				if(pCX){
					px += -0.1;
					ox += -0.1;
				}
				if(pCZ){
					pz += -0.1;
					oz += -0.1;
				}
				break;

			case 'b': case 'B':
				aux++;
				if(py < 0){
					if(pCY){
						py += 0.1;
						oy += 0.1;
					}
					if(pCX){
						px += 0.1;
						ox += 0.1;
					}
					if(pCX){
						pz += 0.1;
						oz += 0.1;
					}
				}
			break;
		}
	}	
	glutPostRedisplay();
}

void teclasEspeciais(int key, int x, int y){
	switch(key){
		case GLUT_KEY_RIGHT: //dir
			rotate_y += 2;
			vx = sin(rotate_y * M_PI / 180) * 0.1;
			vz = cos(rotate_y * M_PI / 180) * 0.1;

			vxn = sin((180 - rotate_y) * M_PI / 180) * 0.1;
			vzn = cos((180 - rotate_y) * M_PI / 180) * 0.1;
		break;

		case GLUT_KEY_LEFT: //esq
			rotate_y -= 2;
			vx = sin(rotate_y * M_PI / 180) * 0.1;
			vz = cos(rotate_y * M_PI / 180) * 0.1;

			vxn = sin((180 - rotate_y) * M_PI / 180) * 0.1;
			vzn = cos((180 - rotate_y) * M_PI / 180) * 0.1;
		break;

		case GLUT_KEY_UP:
			//zoom in
			rotate_x += .05;
		break;

		case GLUT_KEY_DOWN:
			//zoom out
			rotate_x -= .05;
		break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
	/*
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_DOWN)
			if(angle >= 10)
				angle -= 50;
	}else{
		if(state == GLUT_DOWN)
			if(angle <= 130)
				angle += 50;
	}
	*/
	glutPostRedisplay();
}

void resize(int width, int height){
    const float ar = (float) width / (float) height; 
 
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);	//trata da matriz perspectiva
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
} 

int main(int argc, char* argv[]){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(10,10); 
	glutCreateWindow("Trabalho Final de CGR 2017.1");

	aux=0;
	glEnable(GL_DEPTH_TEST); //z-buffer
	glutDisplayFunc(display);

	glutKeyboardFunc(teclado);
	glutSpecialFunc(teclasEspeciais);
	glutMouseFunc(mouse);

	glutReshapeFunc(resize);

	glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); 
 
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 
 
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 

    inicializa();
	glutMainLoop();

	return 0;
}