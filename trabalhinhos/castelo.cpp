// gcc castelo.cpp -lglut -lGL -lGLU -lm -o castelo
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>

#define ESCAPE 27

int window;
GLfloat dx, y, z;
double rotate_y=0; 

void inicia(){
    glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	GLfloat especular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess[] = { 50.0 };
	GLfloat lightPosition[] = { 0.0, 2.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, especular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // glEnable ( GL_COLOR_MATERIAL );
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable ( GL_COLOR_MATERIAL );

	glColor3f(0., 0., 0.);
	dx = 0.0;
	y = 0.0;
	z = 0.0;
}

void torreDir(){
    int ztranslate = -5;
    glPushMatrix();
        glColor3f(1.0, 0.7, 0.0);
        glTranslatef(1.7, 0.0,ztranslate);
        glRotatef(-90., 1.0, 0.0, 0.0);
        gluCylinder(gluNewQuadric(),0.45, 0.40, 1.35, 32, 32);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.65, 0.0, 0.0);
        glTranslatef(1.7,1.35,ztranslate);
        glRotatef(-90., 1.0, 0.0, 0.0);
        glutSolidCone(0.60,0.40,20,16);
    glPopMatrix();
}

void paredeDir(){
    glPushMatrix();
        glColor3f(1.0, 0.7, 0.0);
        glTranslatef(1,0.5,-0.4);
        glRotatef(50, 0.0, 1.0, 0.0);
        glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
        glColor3f(1.0, 0.7, 0.0);
        glTranslatef(0.4,0.5,0.32);
        glRotatef(50, 0.0, 1.0, 0.0);
        glutSolidCube(1.0);
    glPopMatrix();
}

void torreCentral() {
    int ztranslate = 1.0;
    glPushMatrix();
        glColor3f(1.0, 0.7, 0.0);
        glTranslatef(0.0, 0.0,ztranslate);
        glRotatef(-90., 1.0, 0.0, 0.0);
        gluCylinder(gluNewQuadric(),0.5, 0.45, 1.5, 32, 32);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.65, 0.0, 0.0);
        glTranslatef(0,1.5,ztranslate);
        glRotatef(-90., 1.0, 0.0, 0.0);
        glutSolidCone(0.7,0.5,20,16);
    glPopMatrix();
}

void paredeEsq(){
    glPushMatrix();
        glColor3f(1.0, 0.7, 0.0);
        glTranslatef(-1,0.5,-0.4);
        glRotatef(50, 0.0, 1.0, 0.0);
        glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
        glColor3f(1.0, 0.7, 0.0);
        glTranslatef(-0.4,0.5,0.32);
        glRotatef(50, 0.0, 1.0, 0.0);
        glutSolidCube(1.0);
    glPopMatrix();
}

void torreEsq(){
    int ztranslate = -5;
    glPushMatrix();
        glColor3f(1.0, 0.7, 0.0);
        glTranslatef(-1.7, 0.0,ztranslate);
        glRotatef(-90., 1.0, 0.0, 0.0);
        gluCylinder(gluNewQuadric(),0.45, 0.40, 1.35, 32, 32);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.65, 0.0, 0.0);
        glTranslatef(-1.7,1.35,ztranslate);
        glRotatef(-90., 1.0, 0.0, 0.0);
        glutSolidCone(0.60,0.40,20,16);
    glPopMatrix();
}

void castelo(){
    int ztranslate = -1;

    glPushMatrix();
        glColor3f(0.8, 0.8, 0.8);
        glTranslatef(0,1.3,-1);
        glRotatef(20, 0.0, 1.0, 0.0);
        glutSolidCube(1.8);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.8, 0.8, 0.8);
        glTranslatef(1.0, 1.7,ztranslate);
        glRotatef(-90., 1.0, 0.0, 0.0);
        gluCylinder(gluNewQuadric(),0.5, 0.5, 1.0, 32, 32);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.65, 0.0, 0.0);
        glTranslatef(1.0,2.7,ztranslate);
        glRotatef(-90., 1.0, 0.0, 0.0);
        glutSolidCone(0.70,0.60,20,16);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.8, 0.8, 0.8);
        glTranslatef(1.0,1.7,ztranslate);
        glRotatef(90, 1.0, 0.0, 0.0);
        glutSolidCone(0.5,0.5,20,12);
    glPopMatrix();
}

void desenha(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

    glRotatef(rotate_y, 0.0, 1.0, 0.0); //rotacao
    glPushMatrix();
	glTranslatef (0.0, -2.0, -5.0);
	glRotatef(dx, 1.0, 0.0, 0.0);
	glRotatef(y , 0.0, 1.0, 0.0);
	glRotatef(z, 0.0, 0.0, 1.0);
    // inicia
        torreDir();
        paredeDir();
        torreCentral();
        paredeEsq();
        torreEsq();
        castelo();
    // end
        glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void reshape (int w, int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho (-2.5, 2.5, -2.5*(GLfloat)h/(GLfloat)w,2.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
	else
		glOrtho (-2.5*(GLfloat)w/(GLfloat)h,2.5*(GLfloat)w/(GLfloat)h, -2.5, 2.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case ESCAPE:
		glutDestroyWindow(window);
		exit(0);
		break;
	}
}

void teclasEspeciais(int key, int x, int y){
    switch(key){
        case GLUT_KEY_RIGHT: //dir
            rotate_y += 2;
            break;

        case GLUT_KEY_LEFT: //esq
            rotate_y -= 2;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700,700);
	glutInitWindowPosition(0,0);
	window = glutCreateWindow("Castelo");
	inicia();
	glutDisplayFunc(desenha);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
    glutSpecialFunc(teclasEspeciais);
	glutMainLoop();
	return 0;
}