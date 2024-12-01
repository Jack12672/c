#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

#define NB_PARTICLES 1
#define SIZE_PARTICLES 0.35
#define W 120 
#define H 100
#define AREA H/3
#define G 9.81
#define PI 3.14159
#define UPDATE_TIME 0.01


// H et W définit pour affichage sur la totalité de l'écran l'écran avec:
// gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.0, 20.0);
// gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
// soit x+/-20 et y+/-11

struct particle
{
    float x;
    float y;
    float z;
    float v0;
    float time;
    float alphax;
    float alphay;
    float alphaz;
};



void intit_particles(void);
void init(void);
void gravity (void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void write(int x, int y, char *string, void *font);


#endif
