#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

#define NB_PARTICLES 5000
#define SIZE_PARTICLES 0.02

struct position
{
    int pos[5];
};

struct particle
{
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;
};


#endif
