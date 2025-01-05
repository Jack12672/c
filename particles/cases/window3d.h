#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <GL/glut.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_THREAD 3
#define NB_PARTICLES 500
#define SIZE_PARTICLES 10
#define W 1200
#define H 1000
#define WINDOW_W 1600
#define WINDOW_H 900
#define AREA H / 2
#define G 9.81
#define PI 3.14159
#define UPDATE_TIME 1
#define WALL 1 // 0.95   // rebonds
#define FLOOR 1 // 0.8   // rebonds
#define EXCITATION 5 // vitesse max et doit être plus petit que 100

// H et W définit pour affichage sur la totalité de l'écran l'écran avec:
// gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.0, 20.0);
// gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
// soit x+/-100 et y+/-100

typedef struct inputThread
{
    int a;
    int b;
    int thread;
} inputThread_t;

void intit_particles(void);

void init(void);
void bounce(void);
void stopRotationX(void);
void stopRotationY(void);
void display(void);
void reshape(int w, int h);
void write(int x, int y, char *string, void *font);
void keyboard(unsigned char key, int x, int y);
void mouse(int bouton, int etat, int x, int y);
void mousemotion(int x, int y);

#endif
