/********************************************/
/* Rotation d'un solide 3D en OpenGL        */
/* Utilise GLUT (Open GL Utility Toolkit)   */
/* Réalisé par Jean-Christophe MICHEL       */
/* Février 2011                             */
/* www.gecif.net                            */
/********************************************/

// gcc window3d.c -o w3d -lGL -lGLU -lglut


#include <GL/glut.h>

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
static int time=0;
static int timebase=0;
static GLfloat s=0;

/***************************************************************************************************/
/* La fonction traiter_souris sera appelée chaque fois que GLUT détecte un évènement sur la souris */
/***************************************************************************************************/

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

/***************************************************************************************************/
/* La fonction mouvement_souris sera appelée lorsque la souris est déplacée en cliquant            */
/***************************************************************************************************/

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

/********************************************************************************************************/
/* La fonction dessiner sera appelée chaque fois que GLUT détermine que la fenêtre doit être actualisée */
/********************************************************************************************************/

void dessiner(void)
{

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
    glClear (GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    glColor3f(0,0,1);
    glRotated(lx,1,0,0); /* lx est l'angle de rotation en degré */
    glRotated(ly,0,1,0); /* ly est l'angle de rotation en degré */
    glRotated(angle,0,0,1);
    glRotated((angle+5),1,1,0);

    glTranslated(0,0,0);

    glColor3f(0,0,1);
    glutSolidCube(.5);
    glColor3f(0,1,0);
    glutWireCube(.5);

    glTranslated(.5,0,0);

    glColor3f(0,0,1);
    glutSolidCube(.5);
    glColor3f(0,1,0);
    glutWireCube(.5);

    glTranslated(-1,0,0);

    glColor3f(0,0,1);
    glutSolidCube(.5);
    glColor3f(0,1,0);
    glutWireCube(.5);

    glTranslated(.5,.5,0);

    glColor3f(0,0,1);
    glutSolidCube(.5);
    glColor3f(0,1,0);
    glutWireCube(.5);

    glTranslated(0,-.5,.5);

    glColor3f(0,0,1);
    glutSolidCube(.5);
    glColor3f(0,1,0);
    glutWireCube(.5);

    glPopMatrix();
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslated(0,0,0);

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
    time=glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
    s=(frame*1000.0/(time-timebase));
    timebase = time;
    frame = 0;
    }
    glutPostRedisplay();
    }

}

/*************************************************************************************************/
/*                           P R O G R A M M E              P R I N C I P A L                    */
/*************************************************************************************************/

int main(int argc, char** argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize (400, 400);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Objet 3D avec GLUT !");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutDisplayFunc(dessiner);
    glutMouseFunc (traiter_souris);
    glutMotionFunc (mouvement_souris);
    glutIdleFunc (animer); /* définit la fonction qui sera exécutée en arrière plan */
    glutMainLoop();
    return 0; /* la norme C ANSI exige que la fonction main retourne un entier */

}