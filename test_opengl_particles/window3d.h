#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>

#define NB_THREAD 3
#define NB_PARTICLES  3
#define SIZE_PARTICLES 2
#define W 120
#define H 100
#define WINDOW_W 1600
#define WINDOW_H 900
#define AREA H / 3 
#define G 9.81
#define PI 3.14159
#define UPDATE_TIME 1
#define WALL 1    // 0.95   // rebonds
#define FLOOR 1    // 0.8   // rebonds
#define EXCITATION 0.3 // vitesse max


// H et W définit pour affichage sur la totalité de l'écran l'écran avec:
// gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.0, 20.0);
// gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
// soit x+/-100 et y+/-100

struct particle 
{
  float x;
  float y;
  float z;
  float vx;
  float vy;
  float vz;
  int att[4];
};

struct duo
{
  int a;
  int b;
  int thread;
};

void intit_particles(void);
void init(void);
void bounce(int i);
void gravity(void);
float checkAttraction(float a, float b);
void attraction(void);
void stopRotationX(void);
void stopRotationY(void);
void display(void);
void reshape(int w, int h);
void write(int x, int y, char *string, void *font);
void keyboard(unsigned char key, int x, int y);
void mouse(int bouton, int etat, int x, int y);
void mousemotion(int x, int y);

#endif
