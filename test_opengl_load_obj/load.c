//headers
#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
//globals
GLuint elephant;
float elephantrot;
char ch='1';
//other functions and main
//wavefront .obj loader code begins
void loadObj(char *fname)
{
    FILE *fp;
    int read;
    GLfloat x, y, z;
    char ch;
    elephant=glGenLists(1);
    fp=fopen(fname,"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(2.0);
    glNewList(elephant, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_POINTS);
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v')
            {
                glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    fclose(fp);
}
//wavefront .obj loader code ends here
void reshape(int w,int h)
{
    w=1800;
    h=1100;
    // glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
        glViewport(0,0,w,h);

    glLoadIdentity();
    gluPerspective (30, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    //glOrtho(-25,25,-2,2,0.1,100);
    glMatrixMode(GL_MODELVIEW);
}
void drawElephant()
{
    glPushMatrix();
    glTranslatef(0,-20,-105);
    glColor3f(1.0,0.23,0.27);
    // glScalef(0.1,0.1,0.1);
    float a = 1.5;
    glScalef(a,a,a);

    glRotatef(elephantrot,0,-1,0);
    glCallList(elephant);
    glPopMatrix();
    elephantrot=elephantrot+0.6;
    if(elephantrot>360)elephantrot=0;
}
void display(void)
{
    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawElephant();
    glutSwapBuffers(); //swap the buffers
}
int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1800,1100);
    glutInitWindowPosition(20,20);
    glutCreateWindow("ObjLoader");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    loadObj(argv[1]);//replace elepham.obj withp orsche.obj or radar.obj or any other .obj to display it
    glutMainLoop();
    return 0;
}

