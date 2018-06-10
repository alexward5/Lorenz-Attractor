/*
 *  Alexander Ward
 *  CSCI4229
 *  Homework #1
 *
 *  Key bindings:
 *  1      chagnge s
 *  2      change b
 *  3      change r
 *  +/-    Increase/decrease s, b, or r
 *  arrows Change view angle
 *  0      Reset view angle
 *  ESC    Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
int mode=0;     // Dimension (1-4)
double dim=60;   // Dimension of orthogonal box
char* text[] = {"press 1, 2, or 3 to change parameters","S = ","B = ","R = "};  // Dimension display text

//initial variable values
double s = 10;
double b = 2.66;
double r = 28;

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Display the scene
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   //  Draw points from lorenz attractor
   glColor3f(1,1,0);
   //glPointSize(20);
   glBegin(GL_LINE_STRIP);
   glColor3f(0,0,0);

   // Set the initial vertex to (1, 1, 1)
   double x = 0.5;
   double y = 0.5;
   double z = 0.5;
   double dt = 0.001;
   double color = 0.0;
   int counter = 0;
   // Go through the algorithm to generate all desired Lorenz points/lines
   for(int i = 1; i < 50000; i++)
   {
      glColor3f(0, 1 - color, 1);
      double dx = s*(y - x);
      double dy = x*(r - z) - y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
      glVertex3d(x, y, z);
      counter++;
      //change line color
      if( counter > 1 )
      {
         color += 0.01;
         counter = 0;
      }
      if ( color > 1.0 ) 
      {
         color = 0.0;
      }
   }
   glColor3f(1, 1, 1);
   glEnd();

   //  Draw axes in white
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(50,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,50,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,50);
   glEnd();
   //  Label axes
   glRasterPos3d(50,0,0);
   Print("X");
   glRasterPos3d(0,50,0);
   Print("Y");
   glRasterPos3d(0,0,50);
   Print("Z");
   //  Display parameters
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d  %s",th,ph,text[mode]);
   if (mode==1)
      Print("%.1f",s);
   else if (mode==2)
      Print("%.1f",b);
   else if (mode==3)
      Print("%.1f",r);
   //  Flush and swap
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0') {
      th = ph = 0;
      s = 10;
      b = 2.66;
      r = 28;
   }
   else if (ch == '1') {
      mode = 1;
   }
   else if (ch == '2') {
      mode = 2;
   }
   else if (ch == '3') {
      mode = 3;
   }
   else if (ch == '+') {
      if (mode == 1) {
         s += 0.05;
      }
      else if (mode == 2) {
         b += 0.05;
      }
      else if (mode == 3) {
         r += 0.05;
      }
   }
   else if (ch == '-') {
      if (mode == 1) {
         s -= 0.05;
      }
      else if (mode == 2) {
         b -= 0.05;
      }
      else if (mode == 3) {
         r -= 0.05;
      }
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 3;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 3;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 3;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 3;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Coordinates");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
