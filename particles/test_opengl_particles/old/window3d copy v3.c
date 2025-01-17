// gcc window3d.c -o w3d -lGL -lGLU -lglut -lm

#include "window3d.h"

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
        float vy = ((rand() % (int)p)) * EXCITATION / (5 * p);
        float vz = ((rand() % (int)p)) * EXCITATION / p - EXCITATION / (2 * p);
        particles[i] = malloc(sizeof(struct particle));
        particles[i]->x = xx;
        particles[i]->y = yy;
        particles[i]->z = zz;
        particles[i]->vx = vx;
        particles[i]->vy = vy;
        particles[i]->vz = vz;
        particles[i]->att = 1;
        if (i == 0)
        {
            // printf("%f     %f     %f",xx,yy,zz);
            particles[i]->x = 40;
            particles[i]->y = 50;
            particles[i]->z = 100;
            particles[i]->vx = 0;
            particles[i]->vy = 0;
            particles[i]->vz = 0;
            particles[i]->att1 = 1;
        }
        if (i == 1)
        {
            // printf("%f     %f     %f",xx,yy,zz);
            particles[i]->x = 20;
            particles[i]->y = 55;
            particles[i]->z = 100;
            particles[i]->vx = 0;
            particles[i]->vy = 3;
            particles[i]->vz = 0;
            particles[i]->att1 = 0;
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
    if (particles[i]->y < 0)
    {
        particles[i]->y = 0;
        particles[i]->vy = -particles[i]->vy * FLOOR; // bounces
        particles[i]->vx += -particles[i]->vx * (1 - FLOOR); // frictions
        particles[i]->vz += -particles[i]->vz * (1 - FLOOR); // frictions
    }
    if (particles[i]->y > H)
    {
        particles[i]->y = H;
        particles[i]->vy = -particles[i]->vy * WALL;
    }
    if (particles[i]->x < 0)
    {
        particles[i]->x = 0;
        particles[i]->vx = -particles[i]->vx * WALL;
    }
    if (particles[i]->x > H)
    {
        particles[i]->x = H;
        particles[i]->vx = -particles[i]->vx * WALL;
    }

    if (particles[i]->z < 0)
    {
        particles[i]->z = 0;
        particles[i]->vz = -particles[i]->vz * WALL;
    }
    if (particles[i]->z > H)
    {
        particles[i]->z = H;
        particles[i]->vz = -particles[i]->vz * WALL;
    }
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

void attraction1(int a, int b)
{
    float *xa;
    float *xb;
    float *vxa;
    float *vxb;

    for (int i = 0; i < 2; i++)
    {
        if (i == 0) // x
        {
            xa = &particles[a]->x;
            xb = &particles[b]->x;
            vxa = &particles[a]->vx;
            vxb = &particles[b]->vx;
        }
        else if (i == 1) // y
        {
            xa = &particles[a]->y;
            xb = &particles[b]->y;
            vxa = &particles[a]->vy;
            vxb = &particles[b]->vy;
        }

        float direction = 0;
        if (*xa <= *xb)
            direction = -1;
        else
            direction = 1;

        float att = 0;
        int dist = abs((int)(*xb - *xa));
        if (dist > 11)
            att = Forces[11];
        else
            att = Forces[abs(dist)];

        direction *= att * UPDATE_TIME;

        if (abs(*vxa) < SIZE_PARTICLES)
        {
            *vxa += direction;
            *xa += *vxa * UPDATE_TIME;
        }
        if (*vxa < -SIZE_PARTICLES)
            *vxa = -SIZE_PARTICLES;
        if (*vxa > SIZE_PARTICLES)
            *vxa = SIZE_PARTICLES;

        if (abs(*vxb) < SIZE_PARTICLES)
        {
            *vxb += -direction;
            *xb += *vxb * UPDATE_TIME;
        }
        if (*vxb < -SIZE_PARTICLES)
            *vxb = -SIZE_PARTICLES;
        if (*vxb > SIZE_PARTICLES)
            *vxb = SIZE_PARTICLES;
    }
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

void attraction(void)
{
    // for (int i = 0; i < NB_PARTICLES; i++)
    // {
    //     if (i < 100)
    //     {
    //         float att = checkAttraction(particles[i]->x,
    //                                     particles[particles[i]->att1]->x);

    //         if (abs(particles[i]->vx) < SIZE_PARTICLES)
    //         {
    //             particles[i]->vx += att;
    //             particles[i]->x += particles[i]->vx * UPDATE_TIME;
    //         }
    //         if (particles[i]->vx < -SIZE_PARTICLES)
    //             particles[i]->vx = -SIZE_PARTICLES;
    //         if (particles[i]->vx > SIZE_PARTICLES)
    //             particles[i]->vx = SIZE_PARTICLES;

    //         temp1 = particles[i]->vx;
    //         sprintf(txt1, "%s", "vx");
    //         temp2 = particles[i]->x;
    //         sprintf(txt2, "%s", "x");
    //         temp3 = att;
    //         sprintf(txt3, "%s", "att");
    //         temp4 = maximum_force;
    //         sprintf(txt4, "%s", "maxF");
    //     }

    attraction1(0, 1);
    bounce(0);

    bounce(1);
    glutPostRedisplay();
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

        glBegin(GL_QUADS);
        glColor4f(0.1, 0.0, 0.0, 0.5);
        glVertex3f(-AREA, -AREA, -AREA);
        glVertex3f(AREA, -AREA, -AREA);
        glVertex3f(AREA, AREA, -AREA);
        glVertex3f(-AREA, AREA, -AREA);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(0.1, 0.0, 0.0, 0.5);
        glVertex3f(-AREA, -AREA, AREA);
        glVertex3f(AREA, -AREA, AREA);
        glVertex3f(AREA, AREA, AREA);
        glVertex3f(-AREA, AREA, AREA);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(0.0, 0.1, 0.0, 0.5);
        glVertex3f(-AREA, -AREA, -AREA);
        glVertex3f(-AREA, AREA, -AREA);
        glVertex3f(-AREA, AREA, AREA);
        glVertex3f(-AREA, -AREA, AREA);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(0.0, 0.1, 0.0, 0.5);
        glVertex3f(AREA, -AREA, -AREA);
        glVertex3f(AREA, AREA, -AREA);
        glVertex3f(AREA, AREA, AREA);
        glVertex3f(AREA, -AREA, AREA);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(0.0, 0.0, 0.1, 0.5);
        glVertex3f(-AREA, -AREA, -AREA);
        glVertex3f(AREA, -AREA, -AREA);
        glVertex3f(AREA, -AREA, AREA);
        glVertex3f(-AREA, -AREA, AREA);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(0.0, 0.0, 0.1, 0.5);
        glVertex3f(-AREA, AREA, -AREA);
        glVertex3f(AREA, AREA, -AREA);
        glVertex3f(AREA, AREA, AREA);
        glVertex3f(-AREA, AREA, AREA);
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
        glColor3f(0, 0, 0.5);
        glTranslatef(-20, 8, 0);
        glRotatef(90, 1.0, 0.0, 0.0);
        glutWireSphere(3, 20, 20);
        glFlush();
    }
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
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
        if (cameraAngleX + 0.05 < 1)
        {
            cameraAngleX += 0.05;
            // totalRotationX+=cameraAngleX;
            glutPostRedisplay();
        }
        break;
    case 'Y':
        stopRotationX();
        if (cameraAngleY + 0.05 < 1)
        {
            cameraAngleY += 0.05;
            // totalRotationY+=cameraAngleY;
            glutPostRedisplay();
        }
        break;
    case 'a':
        particles[1]->x += 1;
        glutPostRedisplay();
        break;
    case 'q':
        particles[1]->x -= 1;
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
    else
    {
        xclick = 0;
        yclick = 0;
        leftclick = 0;
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
        // if (x < xclick)
        //   xshift = -0.03;
        // else if (x > xclick)
        //   xshift = 0.03;
        // if (y < yclick)
        //   yshift = +0.03;
        // else if (y > yclick)
        //   yshift = -0.1;

        xshift = (x - xclick) / 100;
        yshift = (y - yclick) / 100;

        // temp1=x;
        // temp2=y;
        // temp3=xclick;
        for (int i = 0; i < NB_PARTICLES; i++)
        {
            if (particles[i]->y == 0)
            {
                particles[i]->x += xshift;
                particles[i]->y += yshift;
                particles[i]->y += (xshift + yshift) / 2;
                particles[i]->vx += xshift;
                particles[i]->vy += yshift;
                particles[i]->vy += (xshift + yshift) / 2;
            }

            glutPostRedisplay();
        }
    }
    if (rightclick == 1)
    {
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    if (start == 1)
    {
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
    // glutIdleFunc(gravity);
    glutIdleFunc(attraction);

    glutMainLoop();
    return 0;
}
