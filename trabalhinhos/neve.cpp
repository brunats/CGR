#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>

//g++ -o neve neve.cpp -lm -L/usr/local/lib -lglut -lGL -lGLU

#define NROPART 1500 
#define TELAX	640 
#define TELAY	480 

float queda = 1; // queda da particula
float velocidade = 0.7; // velocidade
float zoom = -13.0; // Zoom da tela
double rotate_y=0; 
double rotate_x=0;
int i;

typedef struct {
	bool existe;	// existe
	float desaparece,tempoExistente; // tempo da particula
	float xpos,zpos,ypos; // posição
	float gravidade, vel;  // Gravidade e velocidade
}particulas;

// vetor de particulas
particulas p[NROPART];

//Função para setar os atributos das particulas definidos anteriormente na struct
//Auto Explicativo
void inicializaParticula(int i) {
    p[i].existe = true;
    p[i].tempoExistente = 2.0;
    p[i].desaparece = float(rand()%100)/1000.0f+0.001f;
    p[i].xpos = (float) (rand() % 21) - 10;
    p[i].ypos = 10.0;
    p[i].zpos = (float) (rand() % 21) - 10;
    p[i].vel = velocidade;
    p[i].gravidade = -0.8;
}

void inicializa( ) {
  int x, z;

    glShadeModel(GL_SMOOTH); // Smooth Shading selecionado (não testei com flat shading)
    glClearColor(0.5, 0.5, .95, 0.5);//Cores do fundo rgba(135, 206, 235, 1.0)
    glClearDepth(1.0); // para limpar o buffer de profundidade
    glEnable(GL_DEPTH_TEST); // Habilita teste de profundidade

    // Inicia as particulas
    for (i = 0; i < NROPART; i++) {
        inicializaParticula(i);
    }
}

// Função para desenhar as particulas de neve
void desenhaParticulasNeve() {
	float x, y, z;
	for (i = 0; i < NROPART; i=i+2) {
		if (p[i].existe == true) {
			x = p[i].xpos;
			y = p[i].ypos;
			if(z != 0)
				z = p[i].zpos + zoom;
			glColor3f(1.0, 1.0, 1.0); // para cor branca
			glPushMatrix(); // Para adicionar matriz na pilha
			glTranslatef(x, y, z); // Cria matriz de tradução, matriz atual será multiplicada por esta
			glutSolidSphere(0.1, 10, 10); // Formato da particula (circulo)
			glPopMatrix(); // para retirar matriz da pilha
			p[i].ypos += p[i].vel / (queda*1000); //Queda para baixo
			p[i].xpos += p[i].vel / (queda*3000); //Queda para esquerda em velocidade menor
			p[i].vel += p[i].gravidade; //Queda pela gravidade
			p[i].tempoExistente -= p[i].desaparece; //Tempo de vida da particula sendo decrementado
			//Reinicia particula caso seu tempo de vida esgote
			if (p[i].tempoExistente < 0.0) {
				inicializaParticula(i);
			}
		}
	}
}

void specialKeys( int key, int x, int y ) {
	switch (key) {
		case GLUT_KEY_RIGHT:
			rotate_y += 5;
			break;

		case GLUT_KEY_LEFT:
			rotate_y -= 5;
			break;
		
		case GLUT_KEY_UP:
			rotate_x += 5;
			break;

		case GLUT_KEY_DOWN:
			rotate_x -= 5;
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

void normalKeys(unsigned char key, int x, int y) {
	switch (key) {
		case 43:
			zoom += .02;
			break;
		case 61:
			zoom += .2;
			break;
		
		case 45:
			zoom -= .2;
			break;

		case 113:
			exit(0);
			break;

		case 81:
			exit(0);
			break;

		case 27:
			exit(0);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

// Cria o cenário e desenha as particulas
void criaCenario( ) {
	int i, j;
	float x, y, z;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpar Buffers
	glMatrixMode(GL_MODELVIEW); //Aplica operações sobre a pilha  de matrizes modelview
	glLoadIdentity(); // Substitui matriz pela matriz identidade
	desenhaParticulasNeve(); // Função para desenhar neve
	glutSwapBuffers(); // Troca buffers na janela atual

	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
}

void redimensiona(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h); // Seta a porta de visão, no caso w = width e h = hight
    glMatrixMode(GL_PROJECTION); //Aplica operações sobre a pilha  de matrizes de projeção
    glLoadIdentity(); // Substitui matriz pela matriz identidade

    gluPerspective(45, (float) w / (float) h, .1, 200); // Matriz de perspectiva

    glMatrixMode(GL_MODELVIEW); //Aplica operações sobre a pilha  de matrizes modelview
    glLoadIdentity(); // Substitui matriz pela matriz identidade
}

void janelaInativa( ) {
  	glutPostRedisplay(); // Marca janela para ser mostrada novamente
}

int main (int argc, char** argv) {
	glutInit(&argc, argv); // Inicia Janela
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(TELAX, TELAY); // Inicia a resolução
	glutCreateWindow("Neve");
	inicializa();
	glutDisplayFunc(criaCenario);
	glutReshapeFunc(redimensiona);
	glutKeyboardFunc(normalKeys);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(janelaInativa); // Marca janela para ser mostrada novamente
	glutMainLoop();
	return 0;
}