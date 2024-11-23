// gcc window3d.c -o w3d -lGL -lGLU -lglut


#include <GL/glut.h>

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
// static int time=0;
// static int timebase=0;
// static GLfloat s=0;

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


// void dessiner(void)
// {

//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
//     gluLookAt(0, 0, 0,     0, 0, -1,     0, 1, 0);
//     glClear (GL_COLOR_BUFFER_BIT);
//     glPushMatrix();

//     // glColor3f(1,0,0);
//     glRotated(lx,1,0,0); /* lx est l'angle de rotation en degré */
//     glRotated(ly,0,1,0); /* ly est l'angle de rotation en degré */
//     // glRotated(angle,1,0.9,0.8);
//     // glRotated((angle+50),1,1,0);

//     // glTranslatef(0,0,0);


//     // glFrontFace(GL_CCW - GL_CW);
//     // glEnable(GL_CULL_FACE); 
//     // glCullFace(GL_FRONT - GL_BACK - GL_FRONT_AND_BACK);
    
    
//     glTranslatef(-0.9,0,0);    // coté gauche écran
//     // glColor3f(0,1,0);
//     // glutSolidCube(.5);
//     glColor3f(1,1,0);
//     glutWireCube(.2);

//     glTranslatef(0.3,0,0);
//     glColor3f(1,1,0);
//     glutWireCube(.2);

//     glTranslatef(0.3,0,0);
//     glColor3f(1,1,0);
//     glutWireCube(.2);

//     glTranslatef(0.3,0,0);
//     // glColor3f(0,0,1);
//     // glutSolidCube(.5);
//     glColor3f(1,1,1);
//     glutWireCube(.2);


//     glTranslatef(0,0.3,0);
//     glColor3f(0,1,1);
//     glutWireCube(.2);

//     glTranslatef(0,0.3,0);
//     glColor3f(0,1,1);
//     glutWireCube(.2);

//     glTranslatef(0,0.3,0);
//     glColor3f(0,1,1);
//     glutWireCube(.2);
//     // glBegin(GL_POLYGON);
//     //     glVertex3f(0,0,0);
//     //     glVertex3f(-0.5,0,0.5);
//     //     glVertex3f(0,0.5,0);
//     //     glVertex3f(0.5,0,-0.5);
//     // glEnd();
 
//     // glTranslatef(.5,.5,0);

//     // glColor3f(0,0,1);
//     // glutSolidCube(.5);
//     // glColor3f(0,1,0);
//     // glutWireCube(.5);

//     // glTranslatef(0,-.5,.5);

//     // glColor3f(0,0,1);
//     // glutSolidCube(.5);
//     // glColor3f(0,1,0);
//     // glutWireCube(.5);

//     // glPopMatrix();
//     // glPushMatrix();
//     // glColor3f(1,1,1);
//     // glTranslatef(0,0,0);

//     glPopMatrix();
//     glEnd();

//     glFlush();
//     glutSwapBuffers();

// }


// void animer (void)
// {

//     if (b_rotation)
//     {
//     angle+=1;
//     if (angle>360.00f){
//     angle = 0.0;}


//     frame++;
//     time=glutGet(GLUT_ELAPSED_TIME);
//     if (time - timebase > 1000) {
//     s=(frame*1000.0/(time-timebase));
//     timebase = time;
//     frame = 0;
//     }
//     glutPostRedisplay();
//     }

// }

// int main(int argc, char** argv)
// {

//     glutInit(&argc, argv);
//     glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE);
//     glutInitWindowSize (600, 600);
//     glutInitWindowPosition (100, 100);
//     glutCreateWindow ("Objet 3D avec GLUT !");
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glutDisplayFunc(dessiner);
//     glutMouseFunc (traiter_souris);
//     glutMotionFunc (mouvement_souris);
//     glutIdleFunc (animer);
//     glutMainLoop();
//     return 0; 

// }




//    autre démo



static int year = 0, day = 0;
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void)
{
     /*
			+y     -z (Into the Screen, our eyes' direction)
			|     /
			|    /
			|   /
			|  /
			| /
			+----------------> +x
		   /
		  /
		 /
	   +z (Out of Screen)


	Plane Equation: A ⋅ 𝑥 + B ⋅ 𝑦 + C ⋅ 𝑧 + D = 0

    */

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    {
        glPushMatrix();
        {

            // rotation on its axis or north pole (y-axis)
            glRotatef(-day, 0.0, 1.0, 0.0); 
            glRotatef(year, 0.1, 1.0, 0.01); 
            // 90 degree rotation w.r.t x-axis
            glRotatef(90, 1.0, 0.0, 0.0);
            glColor3f(0.5, 1.0, 0);
            glutWireSphere(1.6, 40, 30); // earth

        }
        glPopMatrix();

        glRotatef((GLfloat) day, 0.0, 1.0, 0.0); // revolution around earth
        glRotatef(year, 0.1, 1.0, 0.01);        
        glTranslatef(3.5, 0.0, 0.0);

        glRotatef(90, 1.0, 0.0, 0.0);
        glColor3f(0.5, 0, 1);
        glutWireSphere(0.2, 16, 16); // our moon

    }
    glPopMatrix();
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) 
    {
        case 'd':
        day = (day + 1) % 360;
        glutPostRedisplay();
        break;
        case 'D':
        day = (day - 1) % 360;
        glutPostRedisplay();
        break;
        case 'y':
        year = (year + 5) % 100000;
        glutPostRedisplay();
        break;
        case 'Y':
        year = (year - 5) % 100000;
        glutPostRedisplay();
        break;
        default:

        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1600, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}