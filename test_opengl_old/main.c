#include <GL/glut.h>

// gcc main.c -o opengl -lGL -lGLU -lglut


// test modif

/********************************************************************************************************/
/* La fonction dessiner sera appelée chaque fois que GLUT détermine que la fenêtre doit être actualisée */
/********************************************************************************************************/

void dessiner(void)
{
 glClearColor(0,0,0,0); /* définit la couleur de fond de la fenêtre */
 glClear(GL_COLOR_BUFFER_BIT); /* efface la fenêtre en utilisant la couleur de fond */

 glColor3f(0.2, 0.8, 1.0); /* définit la couleur RVB des objets */

/* glVertex3f crée un nouveau sommet dans l'espace */
/* Le point de coordonnées 0,0,0 est le centre de l'écran */
/* Les coordonnées vont de -1.0 à 1.0 en valeurs réelles */
/* Le système de coordonnées à 3 réels (3f) peut être considéré comme un système
de coordonnée relatives par rapport au centre de l'écran et non absolue dans
la fenêtre : si on redimentionne la fenêtre l'objet sera redimensionné */

 glBegin(GL_POLYGON); /* prépare dans le tampon un objet tracé sommet par sommet (ici un polygone) */
   glVertex3f(-0.25, 0.25, 0.0);
   glVertex3f(0.75, 0.25, 0.0);
   glVertex3f(0.75, 0.75, 0.0);
   glVertex3f(-0.25, 0.75, 0.0);
 glEnd(); /* fin de l'objet */

 glFlush(); /* vide le tampon dans la fenêtre en y ajoutant tous les objets préparés */
}

/*************************************************************************************************/
/*                          P R O G R A M M E         P R I N C I P A L                          */
/*************************************************************************************************/

int main(int argc, char **argv)
{

 glutInit(&argc, argv); /* initialise GLUT (Open GL Utility Toolkit) */

 /* prépare la fenêtre */
 glutInitDisplayMode(GLUT_RGB);
 glutInitWindowSize(1200,900);
 glutInitWindowPosition(100,10);
 glutCreateWindow("Mon premier programme OpenGL");
 glutDisplayFunc(dessiner); /* connecte la fonction de dessin */

 glutMainLoop(); /* affiche à l'écran toutes les fenêtres préparées */
 return 0; /* la norme C ANSI exige que la fonction main retourne un entier */
}

