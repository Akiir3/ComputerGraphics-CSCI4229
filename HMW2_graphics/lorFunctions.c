/*  Erika Bailon
 *  Lorenz Atractor homework 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

/* used all code from ex6, ex7, and ex8 and to add features I looked for changing the colors with glColor3f*/

//  using OpenGL with prototypes for glext to be able to use function symbols as extern declarations
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* Exact same global variables taken from  ex6, ex7, and ex8*/
int th = 0; 		// Azimuth of view angle
int ph = 0; 		// Elevation of view angle
double zh = 0;      // Rotation of teapot
double len = 8; 	// Length of axis
double dim = 20; 	// Dimension of ortho box
int axes = 1; 		// Display axes
int mode = 7; 		// What to display - set the mode to 7

/* colors */
int red = 0;
int green = 0;
int blue = 1;

/*  Lorenz Parameters  given in the original lorenz file*/
double s  = 10;
double b  = 2.6666;
double r  = 28;

/* As we talked about during class, we need to convert the degrees like in example 8 */
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Gonna use var_args since it provides a portable way to access the arguments to a function when the function takes a variable number of arguments
 *resource - https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/va-arg-va-copy-va-end-va-start?view=vs-2019
 */
#define LEN 8192  //  Max length of string example 8
void Print(const char* format , ...){
	char    buf[LEN];
	char*   ch=buf;
	va_list args;
	//  Turn the parameters into a character string
	va_start(args,format);
	vsnprintf(buf,LEN,format,args);
	va_end(args);
	while (*ch)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 * To projet Z component like in example 7
 */
double Zp(double x,double y,double z) {
   return -Sin(th)*Cos(ph)*x + Sin(ph)*y + Cos(th)*Cos(ph)*z;
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene - got it from example 7
 */
void display() {
	//  Clear the image and z buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Reset previous transforms
	glLoadIdentity();
	//  Set view angle
	glRotated(ph,1,0,0);
	glRotated(th,0,1,0);
	
	glEnable(GL_DEPTH_TEST);
	
	//lorenz attractor
	int i;
	/*  Coordinates  */
	double x = 1;
	double y = 1;
	double z = 1;
	/*  Time step  */
	double dt = 0.001;
	
	double dx = 0;
	double dy = 0;
	double dz = 0;

	switch(mode){
		case 7:
			glBegin(GL_LINES);
			for (i = 0; i < 50000; i++){
				dx = s*(y-x);
				dy = x*(r-z)-y;
				dz = x*y - b*z;
				x += dt*dx;
				y += dt*dy;
				z += dt*dz;
				glColor3f(red,green,blue);
				glVertex3d(x/3, y/3, z/3);
			}
			glEnd();
			break;
		default:
			break;
	}
	//created axes like in example 6
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(len,0,0);
	glVertex3d(0,0,0);
	glVertex3d(0,len,0);
	glVertex3d(0,0,0);
	glVertex3d(0,0,len);
	glEnd();
	//  Label axes
	glRasterPos3d(len,0,0);
	Print("X");
	glRasterPos3d(0,len,0);
	Print("Y");
	glRasterPos3d(0,0,len);
	Print("Z");

	// Print and dislay the text
	glWindowPos2i(2,2);
	Print("B: %.3f S: %.3f  R: %.3f |\n"
			" X: %.3f Y: %.3f  Z: %.3f",
			b, s, r, x, y, z);

	//  Flush and swap
	glFlush();
	glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed - grabbed from example 6
 */
void special(int key,int x,int y){
	//  Right arrow key - increase azimuth by 5 degrees
	if (key == GLUT_KEY_RIGHT)
		th += 5;
	//  Left arrow key - decrease azimuth by 5 degrees
	else if (key == GLUT_KEY_LEFT)
		th -= 5;
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
		ph += 5;
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 5;
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed - grabbed from example 6 and 7 
 */
void key(unsigned char ch,int x,int y){
	// professor shredder wrote this with if else, can i use a switch?
	switch(ch){
		case 27: // esc exits prog
			exit(0);
			break;
		case '0': // reset view angle
			th = ph = 0;
			break;
		case '1': // reset params to original lorenz
			s  = 10;
			b  = 2.6666;
			r  = 28;
			break;
		case '2': // change color to red
			red = 1;
			green = 0;
			blue = 0;
			break;
		case '3': //change color to green
			red = 0;
			green = 1;
			blue = 0;
			break;
		case '4': //change color to blue
			red = 0;
			green = 0;
			blue = 1;
			break;
		case '7':
			mode = 7;
			break;
		case 's': // increase s val
			s += 0.5;
			break;
		case 'S': // decrease s val
			s -= 0.5;
			break;
		case 'r': // increase r val
			r += 0.5;
			break;
		case 'R': // decrease r val
			r -= 0.5;
			break;
		case 'b': // increasee b val
			b += 0.5;
			break;
		case 'B': // decrease b val
			b -= 0.5;
			break;
		default:
			break;
	}
	// redisplay the scene
	glutPostRedisplay();
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized (ex8) ok
 */
void reshape(int width,int height){
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
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do ok
 */
int main(int argc,char* argv[]){
	//  Initialize GLUT and process user parameters
	glutInit(&argc,argv);
	//  Request double buffered, true color window, Z-buffering
	glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH | GLUT_DOUBLE);
	//  Request 500 x 500 pixel window
	glutInitWindowSize(600,600);
	//  Create the window
	glutCreateWindow("Erika Bailon LorenzHmwk");
	//  Tell GLUT to call "display" when the scene should be drawn
	glutIdleFunc(idle);
	//  Tell GLUT to call "reshape" when the window is resized
	glutDisplayFunc(display);
	//  Tell GLUT to call "idle" when there is nothing else to do
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