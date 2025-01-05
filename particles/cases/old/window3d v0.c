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

// static float Forces[12] = { 4,     0.33,  0.01,  -0.96, -0.73, -0.45,
//                             -0.27, -0.13, -0.06, -0.02, -0.01, 0 };

struct Grid *grid;

static int grille [H][H][H];


void intit_particles(void)
{
    grid=malloc(sizeof(struct Grid));
    // for (int x=0;x<H;x++)
    // {
    //     for (int y=0;y<H;y++)
    //     {
    //         for (int z=0;z<H;z++)
    //         {
    //             grid->c[x][y][z]=-1;
    //         }
    //     }
    // }

    
    int p=1000;
    for (int i = 0; i < NB_PARTICLES; i++)
    {
        int xx = rand() % H ;
        int yy = rand() % H ;
        int zz = rand() % H ;
        int vx = (rand() % p);
        int vy = (rand() % p);
        int vz = (rand() % p);
        // grid->c[xx][yy][zz]=i;
        grille[xx][yy][zz]=i;
        grid->x[i]=xx;
        grid->y[i]=yy;
        grid->z[i]=zz;
        grid->vx[i]=(float)vx/(float)p-0.5;
        grid->vy[i]=(float)vy/(float)p-0.5;
        grid->vz[i]=(float)vz/(float)p-0.5;
        // printf("%i: x=%i   y=%i   z=%i\n",grid->c[xx][yy][zz],xx,yy,zz);
        // printf("%i: vx=%4.3f   vy=%f   vz=%f\n",grid->c[xx][yy][zz],grid->vx[i],grid->vy[i],grid->vz[i]);
    }
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void bounce(void)
{

    for (int i=0; i<NB_PARTICLES;i++)
    {
        float x = grid->x[i];
        float y = grid->y[i];
        float z = grid->z[i];
        // grid->c[(int)x][(int)y][(int)z]=-1;
        float vx = grid->vx[i];
        float vy = grid->vy[i];
        float vz = grid->vz[i];
        x+=vx;
        y+=vy;
        z+=vz;
        grid->x[i] = x;
        grid->y[i] = y;
        grid->z[i] = z;
        if (x <= SIZE_PARTICLES)
        {
            grid->x[i] = SIZE_PARTICLES;
            grid->vx[i] *= -WALL;
        }
        else if (x >= H - SIZE_PARTICLES)
        {
            grid->x[i] = H - SIZE_PARTICLES;
            grid->vx[i] *= -WALL;
        }
        if (y <= SIZE_PARTICLES)
        {
            grid->y[i] = SIZE_PARTICLES;
            grid->vy[i] *= -WALL; // bounces
        }
        else if (y >= H - SIZE_PARTICLES)
        {
            grid->y[i] = H - SIZE_PARTICLES;
            grid->vy[i] *= -WALL;
        }
        if (z <= SIZE_PARTICLES)
        {
            grid->z[i] = SIZE_PARTICLES;
            grid->vz[i] *= -WALL;
        }
        else if (z >= H - SIZE_PARTICLES)
        {
            grid->z[i] = H - SIZE_PARTICLES;
            grid->vz[i] *= -WALL;
        }     
            
        // grid->c[(int)x][(int)y][(int)z]=i;
    }
    glutPostRedisplay(); 

    return;

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
            float x = grid->x[i] * 2 * AREA / H - AREA;
            float y = grid->y[i] * 2 * AREA / H - AREA;
            float z = grid->z[i] * 2 * AREA / H - AREA;
            glPushMatrix();
            if (i==0)
            {
                glColor3f(0, 0, 1);
                glTranslatef(x, y, z);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
            }
            else if (i % 2 == 0)
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
    // glutTimerFunc(1,attraction,1);
    glutIdleFunc(bounce);
    glutMainLoop();
    return 0;
}
