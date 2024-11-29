#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

#define NB_PARTICLES 2000
#define SIZE_PARTICLES 0.05
#define W 40 
#define H 22
#define AREA H/3

// H et W définit pour affichage sur la totalité de l'écran l'écran avec:
// gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.0, 20.0);
// gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
// soit x+/-20 et y+/-11

struct particle
{
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;
};



void intit_particles(void);
void init(void);
void gravity (void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void write(int x, int y, char *string, void *font);


#endif
