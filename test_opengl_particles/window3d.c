// gcc window3d.c -o w3d -lGL -lGLU -lglut

#include "window3d.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

static int start = 1;
static int xclick = 0;
static int yclick = 0;
static int xclickright = 0;
// static int yclickright = 0;
static int leftclick = 0;
static int rightclick=0;
static float cameraAngle =0;
static float totalRotation=0;


struct particle *particles[NB_PARTICLES];

void intit_particles(void) {
  srand(time(NULL));
  float p = 1000;
  for (int i = 0; i < NB_PARTICLES; i++) {
    float xx = ((rand() % (int)p - p / 2)) * 1.8*AREA / p;
    float yy = ((rand() % (int)p - p / 2)) * 2*AREA / p;
    float zz = ((rand() % (int)p - p / 2)) * 1.8*AREA / p;
    particles[i] = malloc(sizeof(struct particle));
    particles[i]->x = xx;
    particles[i]->y = yy;
    particles[i]->z = zz;
  }
}

void init(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void gravity(void) {
  for (int i = 0; i < NB_PARTICLES; i++) {
    particles[i]->y -= (float)H / 1000;
    if (particles[i]->y < -AREA)
      particles[i]->y = AREA;
    }
    glutPostRedisplay();

}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  totalRotation+=cameraAngle;
  glRotated(cameraAngle,0,1,0);
  glPushMatrix();
  {
    glColor3d(1,1,1);
    float i = cameraAngle;
    char str[10];
    sprintf(str, "%f", i);
    write(-W/3,H/2,str,GLUT_BITMAP_9_BY_15);   

    glBegin(GL_QUADS);
    glColor4f(  0.2,  0.0,  0.0 , 0.5);
    glVertex3f(-AREA, -AREA, -AREA);
    glVertex3f( AREA, -AREA, -AREA);
    glVertex3f( AREA,  AREA, -AREA);
    glVertex3f(-AREA,  AREA, -AREA);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(  0.2,  0.0,  0.0 , 0.5);
    glVertex3f(-AREA, -AREA,  AREA);
    glVertex3f( AREA, -AREA,  AREA);
    glVertex3f( AREA,  AREA,  AREA);
    glVertex3f(-AREA,  AREA,  AREA);
    glEnd();


    glBegin(GL_QUADS);
    glColor4f(  0.0,  0.2,  0.0, 0.5);
    glVertex3f(-AREA, -AREA, -AREA);
    glVertex3f(-AREA,  AREA, -AREA);
    glVertex3f(-AREA,  AREA,  AREA);
    glVertex3f(-AREA, -AREA,  AREA);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(  0.0,  0.2,  0.0, 0.5);
    glVertex3f( AREA, -AREA, -AREA);
    glVertex3f( AREA,  AREA, -AREA);
    glVertex3f( AREA,  AREA,  AREA);
    glVertex3f( AREA, -AREA,  AREA);
    glEnd();


    glBegin(GL_QUADS);
    glColor4f(  0.0,  0.0,  0.2, 0.5);
    glVertex3f(-AREA, -AREA, -AREA);
    glVertex3f( AREA, -AREA, -AREA);
    glVertex3f( AREA, -AREA,  AREA);
    glVertex3f(-AREA, -AREA,  AREA);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(  0.0,  0.0,  0.2, 0.5);
    glVertex3f(-AREA,  AREA, -AREA);
    glVertex3f( AREA,  AREA, -AREA);
    glVertex3f( AREA,  AREA,  AREA);
    glVertex3f(-AREA,  AREA,  AREA);
    glEnd();
    
    for (int i = 0; i < NB_PARTICLES; i++) {
      float x = particles[i]->x;
      float y = particles[i]->y;
      float z = particles[i]->z;
      glPushMatrix();
      if (i % 2 == 0) {
        glColor3f(0.7, 0.5, 0.1);
        glTranslatef(x, y, z);
        glutWireSphere(SIZE_PARTICLES, 40, 30);
      } else {
        glColor3f(0.1, 0.9, 0.5);
        glTranslatef(x, y, z);
        glutWireSphere(SIZE_PARTICLES, 40, 30);
      }
      glPopMatrix();
    }
    glColor3f(0, 0, 0.5);
    glTranslatef(-20, 8, 0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutWireSphere(3, 20, 20);
    glFlush();


  }
  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void write(int x, int y, char *string, void *font)
{
	glRasterPos2f(x,y); 
	int len = (int) strlen(string); 
	for (int i = 0; i < len; i++) glutBitmapCharacter(font,string[i]); 
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case ' ':
    gravity();
    glutPostRedisplay();
    break;
  case 'r':
    totalRotation+=cameraAngle;
    glRotated(-totalRotation,0,1,0);
    cameraAngle=0;
    totalRotation=0;
    glutPostRedisplay();
    break;

  default:
    break;
  }
}

void mouse(int bouton, int etat, int x, int y) {
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN) {
    xclick = x;
    yclick = y;
    leftclick = 1;
  } else {
    xclick = 0;
    yclick = 0;
    leftclick = 0;
  }
  if (bouton == GLUT_RIGHT_BUTTON && etat == GLUT_DOWN) {
    xclickright = x;
    rightclick = 1;
  } else {
    xclickright = 0;
    rightclick = 0;
  }
}

void mousemotion(int x, int y) {
  if (leftclick == 1) {
    float xshift = 0;
    float yshift = 0;
    if (x < xclick)
      xshift = -0.03;
    else if (x > xclick)
      xshift = 0.03;
    if (y < yclick)
      yshift = +0.1;
    else if (y > yclick)
      yshift = -0.03;
    for (int i = 0; i < NB_PARTICLES; i++) {
      particles[i]->x += xshift;
      particles[i]->y += yshift;
      glutPostRedisplay();
    }
  }
  if (rightclick==1)
  {
    if (x < xclickright)
      cameraAngle-=0.01;
    else if (x>xclickright)
      cameraAngle+=0.01;
    if (cameraAngle<=-1) cameraAngle=-1;
    if (cameraAngle>=1) cameraAngle=1;
    glutPostRedisplay();
  }

}

int main(int argc, char **argv) {
  if (start == 1) {
    start = 2;
    intit_particles();
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1600, 900);
  glutInitWindowPosition(500, 20);
  glutCreateWindow(argv[1]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  glutIdleFunc(gravity);  
  glutMainLoop();
  return 0;
}
