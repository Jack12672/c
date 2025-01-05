// gcc window3d.c -o w3d -lGL -lGLU -lglut -lm

#include "window3d.h"

#include <GL/glut.h>
#include <math.h>
#include <pthread.h>
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
static int xxx = 0;
static int yyy = 0;
static int zzz = 0;

static int vxx = 0;
static int vyy = 0;
static int vzz = 0;

static int particles[NB_PARTICLES];
static int velocity[NB_PARTICLES];

// static float Forces[12] = { 4,     0.33,  0.01,  -0.96, -0.73, -0.45,
//                             -0.27, -0.13, -0.06, -0.02, -0.01, 0 };

// static int grid [H][H][H];
//    for (int x=0;x<H;x++)
//     {
//         for (int y=0;y<H;y++)
//         {
//             for (int z=0;z<H;z++)
//             {
//                 grid[x][y][z]=-1;
//             }
//         }
//     }
// grid[xxx][yyy][zzz]=i;

int coords_to_int()
{
    return ((xxx + AREA) * H + yyy + AREA) * H + zzz + AREA;
}

void int_to_coords(int val)
{
    zzz = val % H - AREA;
    int temp = val / H;
    yyy = temp % H - AREA;
    xxx = temp / H - AREA;
}

int velocity_to_int()
{
    return ((vxx + 10) * 100 + vyy + 10) * 100 + vzz + 10;
}

void int_to_velocity(int val)
{
    vzz = val % 100 - 10;
    int temp = val / 100;
    vyy = temp % 100 - 10;
    vxx = temp / 100 - 10;
}

void intit_particles(void)
{
    srand(time(NULL));
    for (int i = 0; i < NB_PARTICLES; i++)
    {
        xxx = (rand() % H);
        yyy = (rand() % H);
        zzz = (rand() % H);
        vxx = (rand() % (2 * EXCITATION));
        vyy = (rand() % (2 * EXCITATION));
        vzz = (rand() % (2 * EXCITATION));

        xxx -= AREA;
        yyy -= AREA;
        zzz -= AREA;
        vxx -= EXCITATION;
        vyy -= EXCITATION;
        vzz -= EXCITATION;

        printf("----  %i   %i   %i\n", xxx, yyy, zzz);

        particles[i] = coords_to_int();
        velocity[i] = velocity_to_int();
        xxx = -27;
        int_to_coords(particles[i]);

        printf(" %i    %i\n", i, particles[i]);
        printf("----  %i   %i   %i\n", xxx, yyy, zzz);
        printf("\n");
    }
}

void bounce(void)
{
    for (int i = 0; i < NB_PARTICLES; i++)
    {
        int_to_coords(particles[i]);
        int_to_velocity(velocity[i]);
        xxx += vxx;
        yyy += vyy;
        zzz += vzz;
        if (xxx <= -AREA)
        {
            xxx = -AREA;
            vxx *= -WALL;
        }
        else if (xxx >= AREA - SIZE_PARTICLES)
        {
            xxx = AREA - SIZE_PARTICLES;
            vxx *= -WALL;
        }
        if (yyy <= -AREA)
        {
            yyy = -AREA;
            vyy *= -WALL;
        }
        else if (yyy >= AREA - SIZE_PARTICLES)
        {
            yyy = AREA - SIZE_PARTICLES;
            vyy *= -WALL;
        }
        if (zzz <= -AREA)
        {
            zzz = -AREA;
            vzz *= -WALL;
        }
        else if (zzz >= AREA - SIZE_PARTICLES)
        {
            zzz = AREA - SIZE_PARTICLES;
            vzz *= -WALL;
        }
        particles[i] = coords_to_int();
        velocity[i] = velocity_to_int();
    }
    glutPostRedisplay();
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void stopRotation(void)
{
    cameraAngleX = 0;
    cameraAngleY = 0;
    totalRotationX = 0;
    totalRotationY = 0;
    reshape(WINDOW_W, WINDOW_H);
}

void zoom(void)
{
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
            int_to_coords(particles[i]);
            glPushMatrix();
            if (i == 0)
            {
                glColor3f(0, 0, 1);
                glTranslatef(xxx, yyy, zzz);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
                // printf("----  %i   %i   %i\n", xxx, yyy, zzz);
            }
            else if (i % 2 == 0)
            {
                glColor3f(0.7, 0.5, 0.1);
                glTranslatef(xxx, yyy, zzz);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
            }
            else
            {
                glColor3f(0.1, 0.9, 0.5);
                glTranslatef(xxx, yyy, zzz);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
            }
            glPopMatrix();
        }
        glColor3f(0, 0, 1);
        glTranslatef(-AREA * 1.45, 0, AREA);
        glRotatef(90, 1.0, 0.0, 0.0);
        glutWireSphere(300, 20, 20);
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
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.0, AREA);
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
        pointOfView = AREA * 3;
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
            yshift = 10;
        if (y < yclickright)
            yshift = -10;

        pointOfView += yshift;
        if (pointOfView < AREA / 2)
            pointOfView = AREA / 2;
        else if (pointOfView > AREA * 10)
            pointOfView = AREA * 10;
        zoom();
    }
}

int main(int argc, char **argv)
{
    if (start == 1)
    {
        start = 0;
        intit_particles();
        pointOfView = AREA * 3;
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
    glutIdleFunc(bounce);
    glutMainLoop();
    return 0;
}
