// gcc window3d.c -o w3d -lGL -lGLU -lglut

#include "window3d.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int year = 0, day = 0;
static int start = 1;

struct particle *particles[5];

void intit_particles(void) {
  for (int i = 0; i < 5; i++) {
    float xx = (rand() % 200 - 100) / 100.0;
    float yy = (rand() % 200 - 100) / 100.0;
    particles[i] = malloc (sizeof (struct particle));
    particles[i]->x = xx;
    particles[i]->y = yy;
    particles[i]->z = 0;
  }
}

void init(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void display(void) {
  /*
                     +y     -z (Into the Screen, our eyes' direction)
                     |     /
                     |    /
                     |   /
                     |  /
                     | /
                     +----------------> +x
                /
               /
              /
        +z (Out of Screen)


     Plane Equation: A ⋅ 𝑥 + B ⋅ 𝑦 + C ⋅ 𝑧 + D = 0

 */

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);

  glPushMatrix();
  {
    glPushMatrix();
    {

      // rotation on its axis or north pole (y-axis)
      glRotatef(-day, 0.0, 1.0, 0.0);
      glRotatef(year, 0.1, 1.0, 0.01);
      // 90 degree rotation w.r.t x-axis
      glRotatef(90, 1.0, 0.0, 0.0);
      glColor3f(0.5, 1.0, 0);
      glutWireSphere(1.6, 40, 30); // earth
    }
    glPopMatrix();

    glRotatef((GLfloat)day, 0.0, 1.0, 0.0); // revolution around earth
    glRotatef(year, 0.1, 1.0, 0.01);
    glTranslatef(3.5, 0.0, 0.0);

    glRotatef(90, 1.0, 0.0, 0.0);
    glColor3f(0.5, 0, 1);
    glutWireSphere(0.2, 16, 16); // our moon
  }
  glPopMatrix();

  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'd':
    day = (day + 1) % 360;
    glutPostRedisplay();
    break;
  case 'D':
    day = (day - 1) % 360;
    glutPostRedisplay();
    break;
  case 'y':
    year = (year + 5) % 100000;
    glutPostRedisplay();
    break;
  case 'Y':
    year = (year - 5) % 100000;
    glutPostRedisplay();
    break;
  default:

    break;
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
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  if (start == 2) {
    start = 0;

    for (int i = 0; i < 5; i++) {
      float xx = particles[i]->x;
      float yy = particles[i]->y;
      printf("%i      %f     %f\n", i, xx, yy);
    }
  }

  glutMainLoop();
  return 0;
}
