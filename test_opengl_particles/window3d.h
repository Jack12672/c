#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include "window3d.h"


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
