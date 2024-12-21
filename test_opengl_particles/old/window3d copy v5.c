// gcc window3d.c -o w3d -lGL -lGLU -lglut -lm

#include "window3d.h"
#include <pthread.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int start = 1;
static int xclick = 0;
static int yclick = 0;
static int xclickright = 0;
static int yclickright = 0;
static int leftclick = 0;
static int rightclick = 0;
static float cameraAngleX = 0;
static float cameraAngleY = 0;
static float totalRotationX = 0;
static float totalRotationY = 0;
static float pointOfView = 0;

static float temp1 = 0;
static float temp2 = 0;
static float temp3 = 0;
static float temp4 = 0;
static char txt1[5];
static char txt2[5];
static char txt3[5];
static char txt4[5];
static float maximum_force = 0;

static float Forces[12] = { 4,     0.33,  0.01,  -0.96, -0.73, -0.45,
                            -0.27, -0.13, -0.06, -0.02, -0.01, 0 };

struct particle *particles[NB_PARTICLES];

void intit_particles(void)
{
    srand(time(NULL));
    float p = 1000;
    for (int i = 0; i < NB_PARTICLES; i++)
    {
        float xx = ((rand() % (int)p)) * H / p;
        float yy = ((rand() % (int)p)) * H / p;
        float zz = ((rand() % (int)p)) * H / p;
        float vx = ((rand() % (int)p)) * EXCITATION / p - EXCITATION / (2 * p);
        float vy = ((rand() % (int)p)) * EXCITATION / p - EXCITATION / (2 * p);
        float vz = ((rand() % (int)p)) * EXCITATION / p - EXCITATION / (2 * p);
        particles[i] = malloc(sizeof(struct particle));
        particles[i]->x = xx;
        particles[i]->y = yy;
        particles[i]->z = zz;
        particles[i]->vx = vx;
        particles[i]->vy = vy;
        particles[i]->vz = vz;
        particles[i]->att[0]=0;
    }

    for (int a=0; a<NB_PARTICLES;a++)
    {
        float max1=H*W;
        float max2=H*W;
        float max3=H*W;
        float xa = particles[a]->x;
        float ya = particles[a]->y;
        float za = particles[a]->z;
        for (int b=0; b<NB_PARTICLES;b++)
        {
            if (a!=b) 
            {
                float xb = particles[b]->x;        
                float yb = particles[b]->y;
                float zb = particles[b]->z;
                float distance = sqrtf(powf((xa - xb), 2) + powf((ya - yb), 2)
                                + powf((za - zb), 2));
                if (max3>distance) 
                {
                    max3=distance;
                    particles[a]->att[3]=b;
                    if (max2>max3) 
                    {
                        max2=max3;
                        particles[a]->att[2]=b;
                        if (max1>max2) 
                        {
                            max1=max2;
                            particles[a]->att[1]=b;
                        }
                    }
                }
            }
        }
    }
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void bounce(int i)
{
    if (particles[i]->y < SIZE_PARTICLES)
    {
        particles[i]->y = SIZE_PARTICLES;
        particles[i]->vy = -particles[i]->vy * FLOOR; // bounces
        particles[i]->vx += -particles[i]->vx * (1 - FLOOR); // frictions
        particles[i]->vz += -particles[i]->vz * (1 - FLOOR); // frictions
    }
    if (particles[i]->y > H - SIZE_PARTICLES)
    {
        particles[i]->y = H - SIZE_PARTICLES;
        particles[i]->vy = -particles[i]->vy * WALL;
    }
    if (particles[i]->x < SIZE_PARTICLES)
    {
        particles[i]->x = SIZE_PARTICLES;
        particles[i]->vx = -particles[i]->vx * WALL;
    }
    if (particles[i]->x > H - SIZE_PARTICLES)
    {
        particles[i]->x = H - SIZE_PARTICLES;
        particles[i]->vx = -particles[i]->vx * WALL;
    }

    if (particles[i]->z < SIZE_PARTICLES)
    {
        particles[i]->z = SIZE_PARTICLES;
        particles[i]->vz = -particles[i]->vz * WALL;
    }
    if (particles[i]->z > H - SIZE_PARTICLES)
    {
        particles[i]->z = H - SIZE_PARTICLES;
        particles[i]->vz = -particles[i]->vz * WALL;
    }
}

void attraction2(int a)
{
    particles[a]->att[0]=1;
    for (int i=1; i<4; i++)
    {
        int b=particles[a]->att[i];
        particles[b]->att[0]=1;
        float *xa = &particles[a]->x;
        float *xb = &particles[b]->x;
        float *vxa = &particles[a]->vx;
        float vmax = *vxa;
        float *vxb = &particles[b]->vx;
        if (*vxb > vmax)
            vmax = *vxb;

        float *ya = &particles[a]->y;
        float *yb = &particles[b]->y;
        float *vya = &particles[a]->vy;
        if (*vya > vmax)
            vmax = *vya;
        float *vyb = &particles[b]->vy;
        if (*vyb > vmax)
            vmax = *vyb;

        float *za = &particles[a]->z;
        float *zb = &particles[b]->z;
        float *vza = &particles[a]->vz;
        if (*vza > vmax)
            vmax = *vza;
        float *vzb = &particles[b]->vz;
        if (*vzb > vmax)
            vmax = *vzb;

        float distance = sqrtf(powf((*xa - *xb), 2) + powf((*ya - *yb), 2)
                            + powf((*za - *zb), 2));

        if (distance < SIZE_PARTICLES * 2 * vmax)
        {
            float nx = 0;
            if (*xa != *xb)
                nx = (*xa - *xb) / fabs(*xa - *xb);
            float ny = 0;
            if (*ya != *yb)
                ny = (*ya - *yb) / fabs(*ya - *yb);
            float nz = 0;
            if (*za != *zb)
                nz = (*za - *zb) / fabs(*za - *zb);

            float vxnormal = (*vxa - *vxb) * powf(nx, 2);
            float vynormal = (*vya - *vyb) * powf(ny, 2);
            float vznormal = (*vza - *vzb) * powf(nz, 2);
            *vxa -= vxnormal;
            *vxb += vxnormal;
            *vya -= vynormal;
            *vyb += vynormal;
            *vza -= vznormal;
            *vzb += vznormal;
        }

        *xa += *vxa * UPDATE_TIME;
        *xb += *vxb * UPDATE_TIME;

        *ya += *vya * UPDATE_TIME;
        *yb += *vyb * UPDATE_TIME;

        *za += *vza * UPDATE_TIME;
        *zb += *vzb * UPDATE_TIME;
    }
}   


void attraction(void)
{
    for (int i=0; i<NB_PARTICLES;i++)
    {
        if (particles[i]->att[0]==0)
            attraction2(i);
        for (int j=1;j<4;j++)
            bounce(particles[i]->att[j]);
        bounce(i);
    }
    glutPostRedisplay();
    for (int a=0; a<NB_PARTICLES;a++)
        {
            float max1=H*W;
            float max2=H*W;
            float max3=H*W;
            float xa = particles[a]->x;
            float ya = particles[a]->y;
            float za = particles[a]->z;
            particles[a]->att[0]=0;
            // for (int j=1; j<4;j++)
            // {
            //     int b=particles[a]->
            //     float xb = particles[b]->x;        
            //     float yb = particles[b]->y;
            //     float zb = particles[b]->z;
            //     float distance = sqrtf(powf((xa - xb), 2) + powf((ya - yb), 2)
            //                     + powf((za - zb), 2));
            //     if (max3>distance) 
            //     {
            //         max3=distance;
            //         particles[a]->att[3]=b;
            //         if (max2>max3) 
            //         {
            //             max2=max3;
            //             particles[a]->att[2]=b;
            //             if (max1>max2) 
            //             {
            //                 max1=max2;
            //                 particles[a]->att[1]=b;
            //             }
            // }
        }
}


void stopRotation(void)
{
    cameraAngleX = 0;
    cameraAngleY = 0;
    totalRotationX = 0;
    totalRotationY = 0;
    reshape(WINDOW_W, WINDOW_H);
}

void stopRotationX(void)
{
    glRotated(-totalRotationX, 0, 1, 0);
    cameraAngleX = 0;
    totalRotationX = 0;
    glutPostRedisplay();
}

void stopRotationY(void)
{
    glRotated(-totalRotationY, 1, 0, 0);
    cameraAngleY = 0;
    totalRotationY = 0;
    glutPostRedisplay();
}

void zoom(void)
{
    // printf("start2\n");
    glLoadIdentity();
    reshape(WINDOW_W, WINDOW_H);
    glRotated(totalRotationY, 1, 0, 0);
    glRotated(totalRotationX, 0, 1, 0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    totalRotationX += cameraAngleX;
    totalRotationY += cameraAngleY;
    totalRotationX = fmod(totalRotationX, 360.0);
    totalRotationY = fmod(totalRotationY, 360.0);
    glRotated(cameraAngleX, 0, 1, 0);
    glRotated(cameraAngleY, 1, 0, 0);
    glPushMatrix();
    {
        glColor3f(1, 1, 1);
        char str[130];
        char str1[30];
        char str2[30];
        char str3[30];
        char str4[30];

        sprintf(str, "%s", txt1);
        strcat(str, ": ");
        sprintf(str1, "%f", temp1);
        strcat(str, str1);
        strcat(str, " | ");
        strcat(str, txt2);
        strcat(str, ": ");
        sprintf(str2, "%f", temp2);
        strcat(str, str2);
        strcat(str, " | ");
        strcat(str, txt3);
        strcat(str, ": ");
        sprintf(str3, "%f", temp3);
        strcat(str, str3);
        strcat(str, " | ");
        strcat(str, txt4);
        strcat(str, ": ");
        sprintf(str4, "%f", temp4);
        strcat(str, str4);

        write(-W / 3, H / 2, str, GLUT_BITMAP_9_BY_15);

        glBegin(GL_LINE_STRIP);
        glColor4f(1, 0.0, 0.0, 1);
        glVertex3f(-AREA, -AREA, AREA);
        glVertex3f(AREA, -AREA, AREA);
        glVertex3f(AREA, -AREA, -AREA);
        glVertex3f(-AREA, -AREA, -AREA);
        glVertex3f(-AREA, -AREA, AREA);
        glVertex3f(-AREA, AREA, AREA);
        glVertex3f(AREA, AREA, AREA);
        glVertex3f(AREA, AREA, -AREA);
        glVertex3f(-AREA, AREA, -AREA);
        glVertex3f(-AREA, AREA, AREA);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(AREA, -AREA, AREA);
        glVertex3f(AREA, AREA, AREA);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(AREA, -AREA, -AREA);
        glVertex3f(AREA, AREA, -AREA);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(-AREA, -AREA, -AREA);
        glVertex3f(-AREA, AREA, -AREA);
        glEnd();

        for (int i = 0; i < NB_PARTICLES; i++)
        {
            float x = particles[i]->x * 2 * AREA / H - AREA;
            float y = particles[i]->y * 2 * AREA / H - AREA;
            float z = particles[i]->z * 2 * AREA / H - AREA;
            glPushMatrix();
            if (i % 2 == 0)
            {
                glColor3f(0.7, 0.5, 0.1);
                glTranslatef(x, y, z);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
            }
            else
            {
                glColor3f(0.1, 0.9, 0.5);
                glTranslatef(x, y, z);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
            }
            glPopMatrix();
        }
        glColor3f(0, 0, 1);
        glTranslatef(-AREA * 1.45, 0, AREA);
        glRotatef(90, 1.0, 0.0, 0.0);
        glutWireSphere(10, 20, 20);
        glFlush();
    }
    glPopMatrix();

    glutSwapBuffers();

    // temp1 = pointOfView;
    // sprintf(txt1, "%s", "pof");
    // temp2 = 0;
    // sprintf(txt2, "%s", "--");
    temp3 = totalRotationX;
    sprintf(txt3, "%s", "A x");
    temp4 = totalRotationY;
    sprintf(txt4, "%s", "A y");
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.0, H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, pointOfView, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void write(int x, int y, char *string, void *font)
{
    glRasterPos2f(x, y);
    int len = (int)strlen(string);
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(font, string[i]);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':
        pointOfView = H;
        stopRotation();
        break;
    case 'x': // stop rotation x
        if (cameraAngleX - 0.05 > -5)
        {
            cameraAngleX -= 0.05;
            glutPostRedisplay();
        }
        break;
    case 'X':
        if (cameraAngleX + 0.05 < 5)
        {
            cameraAngleX += 0.05;
            glutPostRedisplay();
        }
        break;
    case 'y': // stop rotation y
        if (cameraAngleY - 0.05 > -5)
        {
            cameraAngleY -= 0.05;
            glutPostRedisplay();
        }
        break;
    case 'Y':
        if (cameraAngleY + 0.05 < 5)
        {
            cameraAngleY += 0.05;
            glutPostRedisplay();
        }
        break;
    case 'a':
        particles[1]->vx += 0.2;
        glutPostRedisplay();
        break;
    case 'q':
        particles[1]->vx -= 0.2;
        glutPostRedisplay();
        break;
    case 'w':
        particles[0]->vx = 0;
        glutPostRedisplay();
        break;

    default:
        break;
    }
}

void mouse(int bouton, int etat, int x, int y)
{
    if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN)
    {
        xclick = x;
        yclick = y;
        leftclick = 1;
    }
    else if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_UP)
    {
        xclick = 0;
        yclick = 0;
        leftclick = 0;
        cameraAngleX = 0;
        cameraAngleY = 0;
    }
    if (bouton == GLUT_RIGHT_BUTTON && etat == GLUT_DOWN)
    {
        xclickright = x;
        yclickright = y;
        rightclick = 1;
    }
    else
    {
        xclickright = 0;
        rightclick = 0;
        yclickright = 0;
    }
}

void mousemotion(int x, int y)
{
    if (leftclick == 1)
    {
        float xshift = 0;
        float yshift = 0;
        xshift = (x - xclick) / 100;
        yshift = (y - yclick) / 100;
        cameraAngleX = xshift;
        cameraAngleY = yshift;
        glutPostRedisplay();
    }
    if (rightclick == 1)
    {
        float yshift = 0;
        if (y > yclickright)
            yshift = 0.5;
        if (y < yclickright)
            yshift = -0.5;

        pointOfView += yshift;
        if (pointOfView < H / 2)
            pointOfView = H / 2;
        else if (pointOfView > H * 2)
            pointOfView = H * 2;
        zoom();
    }
}

int main(int argc, char **argv)
{
    if (start == 1)
    {
        start = 0;
        intit_particles();
        pointOfView = H;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutInitWindowPosition(500, 20);
    glutCreateWindow(argv[1]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemotion);
    // glutIdleFunc(gravity);
    glutIdleFunc(attraction);
    glutMainLoop();
    return 0;
}

// int dist=(int)distance;
// if (dist > 11)
//         att = Forces[11]*UPDATE_TIME;
//     else
//         att = Forces(dist]*UPDATE_TIME;

//     float att = 0;
//     int dist = abs((int)(*xb - *xa));
//     if (dist > 11)
//         att = Forces[11];
//     else
//         att = Forces[abs(dist)];

//     direction *= att * UPDATE_TIME;

// *vxa += direction;


void attraction1(int a, int b)
{
    float *xa = &particles[a]->x;
    float *xb = &particles[b]->x;
    float *vxa = &particles[a]->vx;
    float vmax = *vxa;
    float *vxb = &particles[b]->vx;
    if (*vxb > vmax)
        vmax = *vxb;

    float *ya = &particles[a]->y;
    float *yb = &particles[b]->y;
    float *vya = &particles[a]->vy;
    if (*vya > vmax)
        vmax = *vya;
    float *vyb = &particles[b]->vy;
    if (*vyb > vmax)
        vmax = *vyb;

    float *za = &particles[a]->z;
    float *zb = &particles[b]->z;
    float *vza = &particles[a]->vz;
    if (*vza > vmax)
        vmax = *vza;
    float *vzb = &particles[b]->vz;
    if (*vzb > vmax)
        vmax = *vzb;

    float distance = sqrtf(powf((*xa - *xb), 2) + powf((*ya - *yb), 2)
                           + powf((*za - *zb), 2));

    if (distance < SIZE_PARTICLES * 2 * vmax)
    {
        float nx = 0;
        if (*xa != *xb)
            nx = (*xa - *xb) / fabs(*xa - *xb);
        float ny = 0;
        if (*ya != *yb)
            ny = (*ya - *yb) / fabs(*ya - *yb);
        float nz = 0;
        if (*za != *zb)
            nz = (*za - *zb) / fabs(*za - *zb);

        float vxnormal = (*vxa - *vxb) * powf(nx, 2);
        float vynormal = (*vya - *vyb) * powf(ny, 2);
        float vznormal = (*vza - *vzb) * powf(nz, 2);
        *vxa -= vxnormal;
        *vxb += vxnormal;
        *vya -= vynormal;
        *vyb += vynormal;
        *vza -= vznormal;
        *vzb += vznormal;

        temp1 = a;
        sprintf(txt1, "%s", "col");
        temp2 = b;
        sprintf(txt2, "%s", "col");
        // temp3 = shift;
        // sprintf(txt3, "%s", "sh<");
        // temp4 = shift;
        // sprintf(txt4, "%s", "sh>");

        // if ((fabs(vxnormal)!=0) & (fabs(vynormal)==0))
        // {
        //     if (*ya<*yb)
        //     {
        //         *vya=-vxnormal/4;
        //         *vyb=vxnormal/4;
        //     }
        //     if (*ya>*yb)
        //     {
        //         *vya=vxnormal/4;
        //         *vyb=-vxnormal/4;
        //     }
        // }
        // if ((fabs(vxnormal)!=0) & (fabs(vznormal)==0))
        // {
        //     if (*za<*zb)
        //     {
        //         *vza=-vxnormal/4;
        //         *vzb=vxnormal/4;
        //     }
        //     if (*za>*zb)
        //     {
        //         *vza=vxnormal/4;
        //         *vzb=-vxnormal/4;
        //     }
        // }
    }

    *xa += *vxa * UPDATE_TIME;
    *xb += *vxb * UPDATE_TIME;

    *ya += *vya * UPDATE_TIME;
    *yb += *vyb * UPDATE_TIME;

    *za += *vza * UPDATE_TIME;
    *zb += *vzb * UPDATE_TIME;
}


float checkAttraction(float a, float b)
{
    float direction = 0;
    if (a <= b)
        direction = -1;
    else
        direction = 1;

    int dist = abs((int)(b - a));
    float att = 0;

    if (dist > 11)
        att = Forces[11];
    else
        att = Forces[abs(dist)];

    if (att > maximum_force)
        maximum_force = att;
    direction *= att * UPDATE_TIME;

    return direction;
}

void gravity(void)
{
    for (int i = 0; i < NB_PARTICLES; i++)
    {
        particles[i]->x += particles[i]->vx * UPDATE_TIME;
        particles[i]->z += particles[i]->vz * UPDATE_TIME;
        particles[i]->y += -G / 2 * pow(UPDATE_TIME, 2) + particles[i]->vy;
        particles[i]->vy -= G / 2 * pow(UPDATE_TIME, 2);
        bounce(i);
    }
    glutPostRedisplay();
}

void gravity1(int part)
    {
        particles[part]->y += -G / 2 * pow(UPDATE_TIME, 2) + particles[part]->vy;
        particles[part]->vy -= G / 2 * pow(UPDATE_TIME, 2);
        if (particles[part]->vy >0.5) particles[part]->vy =0.5;
        if (particles[part]->vy <-0.5) particles[part]->vy =-0.5;
        bounce(part);
    }

