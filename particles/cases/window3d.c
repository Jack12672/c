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
static float temp_time_max = 0;
static float temp_time_min = 100000;
static float temp1 = 0;
static float temp2 = 0;
static float temp3 = 0;
static float temp4 = 0;
static char txt1[5];
static char txt2[5];
static char txt3[5];
static char txt4[5];
static int xxx[NB_THREAD];
static int yyy[NB_THREAD];
static int zzz[NB_THREAD];

static int vxx[NB_THREAD];
static int vyy[NB_THREAD];
static int vzz[NB_THREAD];

static int particles[NB_THREAD][NB_PARTICLES];
static int velocity[NB_THREAD][NB_PARTICLES];

// static float Forces[12] = { 4,     0.33,  0.01,  -0.96, -0.73, -0.45,
//                             -0.27, -0.13, -0.06, -0.02, -0.01, 0 };

int coords_to_int(int th)
{
    return ((xxx[th] + AREA) * H + yyy[th] + AREA) * H + zzz[th] + AREA;
}

void int_to_coords(int th, int val)
{
    zzz[th] = val % H - AREA;
    int temp = val / H;
    yyy[th] = temp % H - AREA;
    xxx[th] = temp / H - AREA;
}

int velocity_to_int(int th)
{
    return ((vxx[th] + 10) * 100 + vyy[th] + 10) * 100 + vzz[th] + 10;
}

void int_to_velocity(int th, int val)
{
    vzz[th] = val % 100 - 10;
    int temp = val / 100;
    vyy[th] = temp % 100 - 10;
    vxx[th] = temp / 100 - 10;
}

void intit_particles(int th)
{
    srand(time(NULL) + th * 10);
    for (int i = 0; i < NB_PARTICLES; i++)
    {
        xxx[th] = (rand() % H);
        yyy[th] = (rand() % H);
        zzz[th] = (rand() % H);
        vxx[th] = (rand() % (2 * EXCITATION));
        vyy[th] = (rand() % (2 * EXCITATION));
        vzz[th] = (rand() % (2 * EXCITATION));

        xxx[th] -= AREA;
        yyy[th] -= AREA;
        zzz[th] -= AREA;
        vxx[th] -= EXCITATION;
        vyy[th] -= EXCITATION;
        vzz[th] -= EXCITATION;

        // printf("--thread %i--  %i   %i   %i\n", th, xxx[th], yyy[th],
        // zzz[th]);

        particles[th][i] = coords_to_int(th);
        velocity[th][i] = velocity_to_int(th);
        int_to_coords(th, particles[th][i]);

        // printf(" %i    %i\n", i, particles[th][i]);
        // printf("----  %i   %i   %i\n", xxx[th], yyy[th], zzz[th]);
        // printf("\n");
    }
}

// void bounce(void)
// {
//     for (int i = 0; i < NB_PARTICLES; i++)
//     {
//         int_to_coords(particles[i]);
//         int_to_velocity(velocity[i]);
//         xxx += vxx;
//         yyy += vyy;
//         zzz += vzz;
//         if (xxx <= -AREA)
//         {
//             xxx = -AREA;
//             vxx *= -WALL;
//         }
//         else if (xxx >= AREA - SIZE_PARTICLES)
//         {
//             xxx = AREA - SIZE_PARTICLES;
//             vxx *= -WALL;
//         }
//         if (yyy <= -AREA)
//         {
//             yyy = -AREA;
//             vyy *= -WALL;
//         }
//         else if (yyy >= AREA - SIZE_PARTICLES)
//         {
//             yyy = AREA - SIZE_PARTICLES;
//             vyy *= -WALL;
//         }
//         if (zzz <= -AREA)
//         {
//             zzz = -AREA;
//             vzz *= -WALL;
//         }
//         else if (zzz >= AREA - SIZE_PARTICLES)
//         {
//             zzz = AREA - SIZE_PARTICLES;
//             vzz *= -WALL;
//         }
//         particles[i] = coords_to_int();
//         velocity[i] = velocity_to_int();
//     }
//     glutPostRedisplay();
// }

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
        // write_info();
        display_PARTICLES();
        display_AREA();
        display_SPHERE();

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

void display_PARTICLES(void)
{
    for (int i = 0; i < NB_PARTICLES; i++)
    {
        for (int t = 0; t < NB_THREAD; t++)
            int_to_coords(t, particles[t][i]);

        for (int t = 0; t < NB_THREAD; t++)
        {
            glPushMatrix();
            if (i == 0)
            {
                glColor3f(0, 0, 1);
                glTranslatef(xxx[t], yyy[t], zzz[t]);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
                // printf("%i   x=%i\n",t,xxx[t]);
            }
            else if (i % 2 == 0)
            {
                glColor3f(0.7, 0.5, 0.1);
                glTranslatef(xxx[t], yyy[t], zzz[t]);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
            }
            else
            {
                glColor3f(0.1, 0.9, 0.5);
                glTranslatef(xxx[t], yyy[t], zzz[t]);
                glutWireSphere(SIZE_PARTICLES, 40, 30);
            }
            glPopMatrix();
        }
    }
}

void display_SPHERE(void)
{
    glColor3f(0, 0, 1);
    glTranslatef(-AREA * 1.45, 0, AREA);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutWireSphere(300, 20, 20);
}

void display_AREA(void)
{
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
}

void write(int x, int y, char *string, void *font)
{
    glRasterPos2f(x, y);
    int len = (int)strlen(string);
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(font, string[i]);
}

void write_info(void)
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
        for (int i = 0; i < NB_THREAD; i++)
            intit_particles(i);
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

void bounce(void)
{
    float t0 = clock();

    pthread_t thr[NB_THREAD];
    int thr_number[NB_THREAD];
    for (int i = 0; i < NB_THREAD; i++)
    {
        thr_number[i] = i;
        pthread_create(&thr[i], NULL, bounce3, &thr_number[i]);
    }

    void *return_value;
    for (int i = 0; i < NB_THREAD; i++)
        pthread_join(thr[i], (void *)&return_value);

    free(return_value);

    float t1 = (clock() - t0) / CLOCKS_PER_SEC;
    if (t1 < temp_time_min)
    {
        temp_time_min = t1;
        printf("==> time min = %f\n", temp_time_min);
        printf("    time max = %f\n\n", temp_time_max);
    }

    if (t1 > temp_time_max)
    {
        temp_time_max = t1;
        printf("    time min = %f\n", temp_time_min);
        printf("==> time max = %f\n\n", temp_time_max);
    }
}

void *bounce3(void *arguments)
{
    int *arg = (int *)arguments;
    for (int i = 0; i < NB_PARTICLES; i++)
    {
        int_to_coords(*arg, particles[*arg][i]);
        int_to_velocity(*arg, velocity[*arg][i]);

        xxx[*arg] += vxx[*arg];
        yyy[*arg] += vyy[*arg];
        zzz[*arg] += vzz[*arg];
        if (xxx[*arg] <= -AREA)
        {
            xxx[*arg] = -AREA;
            vxx[*arg] *= -WALL;
        }
        else if (xxx[*arg] >= AREA - SIZE_PARTICLES)
        {
            xxx[*arg] = AREA - SIZE_PARTICLES;
            vxx[*arg] *= -WALL;
        }
        if (yyy[*arg] <= -AREA)
        {
            yyy[*arg] = -AREA;
            vyy[*arg] *= -WALL;
        }
        else if (yyy[*arg] >= AREA - SIZE_PARTICLES)
        {
            yyy[*arg] = AREA - SIZE_PARTICLES;
            vyy[*arg] *= -WALL;
        }
        if (zzz[*arg] <= -AREA)
        {
            zzz[*arg] = -AREA;
            vzz[*arg] *= -WALL;
        }
        else if (zzz[*arg] >= AREA - SIZE_PARTICLES)
        {
            zzz[*arg] = AREA - SIZE_PARTICLES;
            vzz[*arg] *= -WALL;
        }
        particles[*arg][i] = coords_to_int(*arg);
        velocity[*arg][i] = velocity_to_int(*arg);
    }
    glutPostRedisplay();
    return (void *)NULL;
}

// void bounce1(void)
// {
//     for (int i = 0; i < NB_PARTICLES; i++)
//     {
//         for (int t=0; t<NB_THREAD; t++)
//         {
//             int_to_coords(t,particles[t][i]);
//             int_to_velocity(t,velocity[t][i]);

//             xxx[t] += vxx[t];
//             yyy[t] += vyy[t];
//             zzz[t] += vzz[t];
//             if (xxx[t] <= -AREA)
//             {
//                 xxx[t] = -AREA;
//                 vxx[t] *= -WALL;
//             }
//             else if (xxx[t] >= AREA - SIZE_PARTICLES)
//             {
//                 xxx[t] = AREA - SIZE_PARTICLES;
//                 vxx[t] *= -WALL;
//             }
//             if (yyy[t] <= -AREA)
//             {
//                 yyy[t] = -AREA;
//                 vyy[t] *= -WALL;
//             }
//             else if (yyy[t] >= AREA - SIZE_PARTICLES)
//             {
//                 yyy[t] = AREA - SIZE_PARTICLES;
//                 vyy[t] *= -WALL;
//             }
//             if (zzz[t] <= -AREA)
//             {
//                 zzz[t] = -AREA;
//                 vzz[t] *= -WALL;
//             }
//             else if (zzz[t] >= AREA - SIZE_PARTICLES)
//             {
//                 zzz[t] = AREA - SIZE_PARTICLES;
//                 vzz[t] *= -WALL;
//             }
//             particles[t][i] = coords_to_int(t);
//             velocity[t][i] = velocity_to_int(t);
//         }
//     }
//     glutPostRedisplay();
// }

//    pthread_t thr0;
//     // pthread_t thr1;

//     int thr0_number=0;
//     // int thr1_number=1;

//     pthread_create(&thr0,NULL,display_th, &thr0_number);

//     void *return_value1;
//     pthread_join(thr0,(void *) &return_value1);

//     free (return_value1);

// pthread_t yo[NB_THREAD];
// for (int i=0; i<NB_THREAD; i++)
// {
//     thread[i].thread=i;
//     pthread_create(&yo[i], NULL, display_th, &thread[i]);
// }
// inputThread_t *rep[NB_THREAD];
// for (int i=0; i<NB_THREAD; i++)
// {
//     pthread_join(yo[i], (void *) &rep[i]);
//     free (rep[i]);
// }

// void *display_th(void *arguments)
// {
//     int *arg = (int *) arguments;

//     if (*arg==0)
//     {
//         printf("-==000000==--  %i  ----\n",*arg);

//     }
//     else printf("---  %i  ----\n",*arg);

//     return (void *) NULL;
// }

// float t0=clock();
// float t1=clock()-t0;
// printf("time = %f\n",t1/CLOCKS_PER_SEC);