// gcc window3d.c -o w3d -lGL -lGLU -lglut

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include "window3d.h"



typedef int BOOL;
#define TRUE 1
#define FALSE 0

static GLfloat angle = 0.0;
static BOOL Button1Down = FALSE, b_rotation=TRUE;
static GLfloat lx=0;
static GLfloat ly=0;
static int xclick=0;
static int yclick=0;
static int frame=0;
static int temps=0;
static int timebase=0;
static GLfloat s=0;
static int width=1000;
static int height=1000;
static float size=0.1;

struct particle *particles[5];

void traiter_souris(int bouton, int etat, int x, int y)
{

    if (bouton == GLUT_LEFT_BUTTON)
    {
    Button1Down = (etat == GLUT_DOWN) ? TRUE : FALSE;
    xclick = x;
    yclick = y;

    }
    if (bouton == GLUT_RIGHT_BUTTON)
    {
    if (etat==GLUT_DOWN)
    {
    if (b_rotation) { b_rotation=FALSE; }
    else { b_rotation=TRUE; }
    }

    }

}


void mouvement_souris(int x, int y)
{

    if (Button1Down)
    {
    if (x>xclick){
                  ly=ly+(xclick-x)*.5;
                  xclick=x;}
    if (x<xclick){
                  ly=ly+(xclick-x)*.5;
                  xclick=x;}
    if (y>yclick){
                  lx=lx+(yclick-y)*.5;
                  yclick=y;}
    if (y<yclick){
                  lx=lx+(yclick-y)*.5;
                  yclick=y;}
    glutPostRedisplay();
    }

}



void dessiner()
{
    srand( time( NULL ) );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0,     0, 0, 0,     0, 1, 0);
    glClear (GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    // glRotated(angle,1,1,1);
    // glRotated(lx,1,0,0);    
    // glRotated(ly,0,1,0);

    int division=20;


    // for (int i=0; i<1;i++)
    // {
        float xx= particles[0]->x;
        float yy= particles[0]->y;
        printf("%f     %f\n", xx,yy);
        // glTranslated(-0.15,-0.02,0);
                // glTranslated(xx,yy,0);

        glColor3f(0,0,1);
        glutSolidSphere(size,division,division);   
        glColor3f(1,0,0);
        glutWireSphere(size,division,division);
    // }

    glPopMatrix();
    glEnd();

    glFlush();
    glutSwapBuffers();

}

/********************************************************************************************************/
/* La fonction animer sera appelée dès que le programme est en attende (aucun évènement détecté)        */
/********************************************************************************************************/

void animer (void)
{

    if (b_rotation)
    {
    angle+=1;
    if (angle>360.00f){
    angle = 0.0;}


    frame++;
    temps=glutGet(GLUT_ELAPSED_TIME);
    if (temps - timebase > 1000) {
    s=(frame*1000.0/(temps-timebase));
    timebase = temps;
    frame = 0;
    }
    glutPostRedisplay();
    }

}


int main(int argc, char** argv)
{
        
    // for (int i=0; i<5;i++)
    // {
    //     float xx= (rand()%200-100)/100.0;
    //     float yy= (rand()%200-100)/100.0;

    //     particles[i].x=xx;
    //     particles[i].y=yy;
    //     particles[i].z=0;
    // }



    particles[0] = malloc (sizeof (struct particle));
    particles[0]->x=0.5;
    particles[0]->y=0;
    particles[0]->z=0;





    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize (width,height);
    glutInitWindowPosition (100, 10);
    glutCreateWindow ("Particles interactions Nov 2024");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i=0; i<1; i++)
    {
        glutDisplayFunc(dessiner);
    }

    glutMouseFunc (traiter_souris);
    glutMotionFunc (mouvement_souris);
    glutIdleFunc (animer);
    glutMainLoop();
    return 0;

}




// typedef int BOOL;
// #define TRUE 1
// #define FALSE 0

// static GLfloat angle = 0.0;
// static BOOL Button1Down = FALSE, b_rotation=TRUE;
// static GLfloat lx=0;
// static GLfloat ly=0;
// static int xclick=0;
// static int yclick=0;
// static int frame=0;
// static int temps=0;
// static int timebase=0;
// static GLfloat s=0;
// static int width=1000;
// static int height=1000;
// static float size=0.1;

// struct particle *particles[5];

// void traiter_souris(int bouton, int etat, int x, int y)
// {

//     if (bouton == GLUT_LEFT_BUTTON)
//     {
//     Button1Down = (etat == GLUT_DOWN) ? TRUE : FALSE;
//     xclick = x;
//     yclick = y;

//     }
//     if (bouton == GLUT_RIGHT_BUTTON)
//     {
//     if (etat==GLUT_DOWN)
//     {
//     if (b_rotation) { b_rotation=FALSE; }
//     else { b_rotation=TRUE; }
//     }

//     }

// }

// void mouvement_souris(int x, int y)
// {

//     if (Button1Down)
//     {
//     if (x>xclick){
//                   ly=ly+(xclick-x)*.5;
//                   xclick=x;}
//     if (x<xclick){
//                   ly=ly+(xclick-x)*.5;
//                   xclick=x;}
//     if (y>yclick){
//                   lx=lx+(yclick-y)*.5;
//                   yclick=y;}
//     if (y<yclick){
//                   lx=lx+(yclick-y)*.5;
//                   yclick=y;}
//     glutPostRedisplay();
//     }

// }

// void dessiner()
// {
//     srand( time( NULL ) );
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
//     gluLookAt(0, 0, 0,     0, 0, 0,     0, 1, 0);
//     glClear (GL_COLOR_BUFFER_BIT);
//     glPushMatrix();
//     // glRotated(angle,1,1,1);
//     // glRotated(lx,1,0,0);
//     // glRotated(ly,0,1,0);

//     int division=20;

//     // for (int i=0; i<1;i++)
//     // {
//         float xx= particles[0]->x;
//         float yy= particles[0]->y;
//         printf("%f     %f\n", xx,yy);
//         // glTranslated(-0.15,-0.02,0);
//                 // glTranslated(xx,yy,0);

//         glColor3f(0,0,1);
//         glutSolidSphere(size,division,division);
//         glColor3f(1,0,0);
//         glutWireSphere(size,division,division);
//     // }

//     glPopMatrix();
//     glEnd();

//     glFlush();
//     glutSwapBuffers();

// }

// /********************************************************************************************************/
// /* La fonction animer sera appelée dès que le programme est en attende (aucun
// évènement détecté)        */
// /********************************************************************************************************/

// void animer (void)
// {

//     if (b_rotation)
//     {
//     angle+=1;
//     if (angle>360.00f){
//     angle = 0.0;}

//     frame++;
//     temps=glutGet(GLUT_ELAPSED_TIME);
//     if (temps - timebase > 1000) {
//     s=(frame*1000.0/(temps-timebase));
//     timebase = temps;
//     frame = 0;
//     }
//     glutPostRedisplay();
//     }

// }

// int main(int argc, char** argv)
// {

//     // for (int i=0; i<5;i++)
//     // {
//     //     float xx= (rand()%200-100)/100.0;
//     //     float yy= (rand()%200-100)/100.0;

//     //     particles[i].x=xx;
//     //     particles[i].y=yy;
//     //     particles[i].z=0;
//     // }

//     particles[0] = malloc (sizeof (struct particle));
//     particles[0]->x=0.5;
//     particles[0]->y=0;
//     particles[0]->z=0;

//     glutInit(&argc, argv);
//     glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE);
//     glutInitWindowSize (width,height);
//     glutInitWindowPosition (100, 10);
//     glutCreateWindow ("Particles interactions Nov 2024");
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     for (int i=0; i<1; i++)
//     {
//         glutDisplayFunc(dessiner);
//     }

//     glutMouseFunc (traiter_souris);
//     glutMotionFunc (mouvement_souris);
//     glutIdleFunc (animer);
//     glutMainLoop();
//     return 0;

// }
