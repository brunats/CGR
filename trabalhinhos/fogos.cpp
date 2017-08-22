// gcc fogos.cpp -lglut -lGL -lGLU -lm -o fogos
#include <GL/glut.h>  
#include <GL/gl.h>	  
#include <GL/glu.h>	  
#include <math.h>
#include <unistd.h>

#define ESCAPE 27
#define N_Part 3000
#define GRAVIDADE 0.0003

struct s_part{
  float x, y, veloc_x, veloc_y;
  unsigned t_vida;
} particulas[N_Part];

int window; 

void iniciaParticulas(int pause){
	int i, ang;
	float v;

	if(pause) usleep(200000 + rand() % 2000000);

	for(i=0;i<N_Part;i++) {
		v = (float)(rand() % 100)/5000.0;
		ang = rand() % 360;
		particulas[i].veloc_x = cos( (M_PI * ang/180.0) ) * v;
		particulas[i].veloc_y = sin( (M_PI * ang/180.0) ) * v;
		particulas[i].x = 0.0;
		particulas[i].y = 0.0;
		particulas[i].t_vida = rand() % 100;
	}
}

void iniciaCena(int Width, int Height){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		//Fundo preto
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f); //calcula perspectivas

	glMatrixMode(GL_MODELVIEW);

	iniciaParticulas(0); //primeiro
}

void redimensiona(int Width, int Height){
	if (Height==0) //tirar divisao por zero
  		Height = 1;

  	glViewport(0, 0, Width, Height); //resetar janela

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void desenha(){
	int i, ative_particulas=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa buffers de memoria
	glLoadIdentity();				

	glTranslatef(0.0f,0.0f,-6.0f); //inicializa particulas no centro

	glBegin(GL_POINTS);
	for(i=0;i<N_Part;i++) {
		if(particulas[i].t_vida) {
			ative_particulas++;
			particulas[i].veloc_y -= GRAVIDADE;
			particulas[i].x += particulas[i].veloc_x;
			particulas[i].y += particulas[i].veloc_y;
			particulas[i].t_vida--;

			glVertex3f( particulas[i].x, particulas[i].y, 0.0f);
		}
	}
	glEnd();

	glutSwapBuffers();
	if(!ative_particulas) //se acabaram
		iniciaParticulas(1); //reinicia particulas
}

void keyPressed(unsigned char key, int x, int y) {
	usleep(100);

	if (key == ESCAPE){ 
		glutDestroyWindow(window); 
		exit(0);                   
	}
}

int main(int argc, char **argv){  
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
	glutInitWindowSize(640, 480);  
	glutInitWindowPosition(0, 0);  
	window = glutCreateWindow("Fogos de artificio");  
	glutDisplayFunc(&desenha);  
	glutFullScreen();
	glutIdleFunc(&desenha);
	glutReshapeFunc(&redimensiona);
	glutKeyboardFunc(&keyPressed);
	iniciaCena(640, 480);
	glutMainLoop();  

	return 0;
}
