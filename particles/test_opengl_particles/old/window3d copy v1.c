// gcc window3d.c -o w3d -lGL -lGLU -lglut -lm

#include "window3d.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
#include <math.h>

static int start = 1;
static int xclick = 0;
static int yclick = 0;
static int xclickright = 0;
static int yclickright = 0;
static int leftclick = 0;
static int rightclick=0;
static float cameraAngleX =0;
static float cameraAngleY =0;
static float totalRotationX=0;
static float totalRotationY=0;
static float temp1=0;
static float temp2=0;
static float temp3=0;

struct particle *particles[NB_PARTICLES];

void intit_particles(void) {
  srand(time(NULL));
  float p = 1000;
  for (int i = 0; i < NB_PARTICLES; i++) {
    // float xx = ((rand() % (int)p - p / 2)) * 1.8*AREA / p;
    // float yy = ((rand() % (int)p - p / 2)) * 1.8*AREA / p;
    // float zz = ((rand() % (int)p - p / 2)) * 1.8*AREA / p;
    float xx = ((rand() % (int)p )) * H / p;
    float yy = ((rand() % (int)p )) * H / p;
    float zz = ((rand() % (int)p )) * H / p;
    if (i==0)
    {
      printf("%f     %f     %f",xx,yy,zz);
      xx=10;
      yy=80;
      zz=100;

    }
    particles[i] = malloc(sizeof(struct particle));
    particles[i]->x = xx;
    particles[i]->y = yy;
    particles[i]->z = zz;
    particles[i]->v0 = 1.5;
    particles[i]->time = UPDATE_TIME;
    particles[i]->alphax = PI/4;
    particles[i]->alphay = 0;
    particles[i]->alphaz = 0;

  }
}

void init(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void gravity(void) {
  for (int i = 0; i < NB_PARTICLES; i++) {
    float x=particles[i]->v0*cos(particles[i]->alphax)*particles[i]->time;
    float y=-G/2*pow(particles[i]->time,2)+particles[i]->v0*sin(particles[i]->alphax)*particles[i]->time;
    float speed=sqrtf(pow(x,2)+pow(y,2))/UPDATE_TIME;
    // if (speed>20)
    // {
    //   x/=2;
    //   y/=2;
    // }

    particles[i]->time+=UPDATE_TIME;
    particles[i]->x+=x;
    particles[i]->y+=y;
    temp1=speed;
    temp2=y;
    temp3=particles[i]->y;
    if (particles[i]->y <0)
    {
      particles[i]->time=UPDATE_TIME;
      particles[i]->alphax=PI/2;
      particles[i]->v0/=3;
    }

    }
    glutPostRedisplay();

}

void stopRotationX (void)
{
  glRotated(-totalRotationX,0,1,0);
  cameraAngleX=0;
  totalRotationX=0;
  glutPostRedisplay();
}

void stopRotationY (void)
{
  glRotated(-totalRotationY,1,0,0);
  cameraAngleY=0;
  totalRotationY=0;
  glutPostRedisplay();
}


void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  totalRotationX+=cameraAngleX;  
  totalRotationY+=cameraAngleY;
  totalRotationX=fmod(totalRotationX,360.0);
  totalRotationY=fmod(totalRotationY,360.0);
  glRotated(cameraAngleX,0,1,0);  
  glRotated(cameraAngleY,1,0,0);
  glPushMatrix();
  {
    // temp1=cameraAngleY;
    // temp2=totalRotationY;
    // temp3=totalRotationX;
    
    glColor3f(1,1,1);
    char str[80];
    char str1[30];
    char str2[30];
    sprintf(str, "%f", temp1);
    strcat (str, " = temp1 | ");    
    sprintf(str1, "%f", temp2);
    strcat (str, str1);
    strcat (str, " = temp2 | ");
    sprintf(str2, "%f", temp3);
    strcat (str, str2);
    strcat (str, " = temp3");
    write(-W/3,H/2,str,GLUT_BITMAP_9_BY_15);   

    glBegin(GL_QUADS);
    glColor4f(  0.1,  0.0,  0.0 , 0.5);
    glVertex3f(-AREA, -AREA, -AREA);
    glVertex3f( AREA, -AREA, -AREA);
    glVertex3f( AREA,  AREA, -AREA);
    glVertex3f(-AREA,  AREA, -AREA);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(  0.1,  0.0,  0.0 , 0.5);
    glVertex3f(-AREA, -AREA,  AREA);
    glVertex3f( AREA, -AREA,  AREA);
    glVertex3f( AREA,  AREA,  AREA);
    glVertex3f(-AREA,  AREA,  AREA);
    glEnd();


    glBegin(GL_QUADS);
    glColor4f(  0.0,  0.1,  0.0, 0.5);
    glVertex3f(-AREA, -AREA, -AREA);
    glVertex3f(-AREA,  AREA, -AREA);
    glVertex3f(-AREA,  AREA,  AREA);
    glVertex3f(-AREA, -AREA,  AREA);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(  0.0,  0.1,  0.0, 0.5);
    glVertex3f( AREA, -AREA, -AREA);
    glVertex3f( AREA,  AREA, -AREA);
    glVertex3f( AREA,  AREA,  AREA);
    glVertex3f( AREA, -AREA,  AREA);
    glEnd();


    glBegin(GL_QUADS);
    glColor4f(  0.0,  0.0,  0.1, 0.5);
    glVertex3f(-AREA, -AREA, -AREA);
    glVertex3f( AREA, -AREA, -AREA);
    glVertex3f( AREA, -AREA,  AREA);
    glVertex3f(-AREA, -AREA,  AREA);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(  0.0,  0.0,  0.1, 0.5);
    glVertex3f(-AREA,  AREA, -AREA);
    glVertex3f( AREA,  AREA, -AREA);
    glVertex3f( AREA,  AREA,  AREA);
    glVertex3f(-AREA,  AREA,  AREA);
    glEnd();
    
    for (int i = 0; i < NB_PARTICLES; i++) {
      float x = particles[i]->x*2*AREA/H-AREA;
      float y = particles[i]->y*2*AREA/H-AREA;
      float z = particles[i]->z*2*AREA/H-AREA;
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
  gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.0, H);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, H, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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
  case 'x': // stop rotation x
    stopRotationX();
    break;
  case 'y': // stop rotation y
    stopRotationY();
    break;
  case 'X':
    stopRotationY();
    if (cameraAngleX+0.05<1) 
    {
      cameraAngleX+=0.05;
      // totalRotationX+=cameraAngleX;
      glutPostRedisplay();
    }
    break;
  case 'Y':
    stopRotationX();
    if (cameraAngleY+0.05<1) 
    {
      cameraAngleY+=0.05;
      // totalRotationY+=cameraAngleY;
      glutPostRedisplay();
    }
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
    yclickright = y;
    rightclick = 1;
  } else {
    xclickright = 0;
    rightclick = 0;
    yclickright = 0;

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
