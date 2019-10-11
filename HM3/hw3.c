/*  Erika Bailon
 *  Scene in 3D
 *The program should allow the scene to be viewed from multiple eye positions directed by user input.
 *The scene should consist of several 3D solid objects, and at least two instances of the same object, but with each instance at a different position, scale and orientation.
 *The program must create at least one generic object, and then create each instance by translation, rotation and scaling of the generic object.
 *You must generate the objects yourself.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

/* used a lot of code from ex6, ex7, and ex8 and more of ex9*/

//  using OpenGL with prototypes for glext to be able to use function symbols as extern declarations
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//global variables same like in ex9
int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world

//  From ex9 to use degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))



/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible - ex9
 */
#define LEN 8192  //  Maximum length of text string
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
 *  Set projection - ex9
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Going to draw Wall-E and Eva (maybe)
 */
static void wallEBody(double x,double y,double z, double scale, double th)
{
   //  Save transformation
   glPushMatrix();

   //  Offset
   glTranslated(x,y+0.5,z);
   glRotated(th,0,1,0);
   glScaled(scale,scale,scale);

   //the body for ex 9
   //  Cube
   glBegin(GL_QUADS);
   //  Front normal yellow
   glColor3f(1, 1, 0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back yello darker
   glColor3f(0.7, 0.7, 0);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right yellow medium
   glColor3f(0.8,0.8,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left yellow lighter
   glColor3f(0.9, 0.9, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top kind of mustard
   glColor3f(1, 0.85, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom dark gray
   glColor3f(0.5, 0.5, 0.5);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();

   // Draw strips on Wall-E
   glBegin(GL_QUADS);
   glColor3f(0.1,0.1,0.1);
   // Top
   glVertex3f(-1,0.9,1.02);
   glVertex3f(-1,0.8,1.02);
   glVertex3f(1,0.9,1.02);
   glVertex3f(1,0.8,1.02);
   // Medium
   glVertex3f(-1,0.7,1.02);
   glVertex3f(-1,0.6,1.02);
   glVertex3f(1,0.7,1.02);
   glVertex3f(1,0.6,1.02);
   // Bottom
   glVertex3f(-1,0.5,1.02);
   glVertex3f(-1,0.4,1.02);
   glVertex3f(1,0.5,1.02);
   glVertex3f(1,0.4,1.02);
   // End
   glEnd();

   //  Undo transofrmations
   glPopMatrix();
}

static void wallE_neck(double x,double y,double z,double r,double d, double th)
{
   int i,k;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y+2,z);
   glRotated(th,1,0,0);
   glScaled(r,r+0.3,d);
   //  top and bottom
   glColor3f(0.4, 0.4, 0.5);
   for (i=1;i>=-1;i-=2)
   {
      glNormal3f(0,i,0);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(1,1);
      glVertex3f(0,i,0);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f( 0.5*Cos(k), 0.5*Sin(k));
         glVertex3f(i*Cos(k), i, Sin(k));
      }
      glEnd();
   }

   //  Edge
   glColor3f(0.4, 0.4, 0.4);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k), +1, Sin(k));
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k), -1, Sin(k));
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}


static void wallE_RightEye(double x,double y,double z,double r,double d, double th, double ph)
{
   int i,k;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x+0.43,y+2.4,z);
   glRotated(th,1,0,0);
   glRotated(ph,0,1,0);
   glScaled(r,r,d);
   //  top and bottom
   glColor3f(0.8, 0.8, 0.8);
   for (i=1;i>=-1;i-=2)
   {
      glNormal3f(0,i,0);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(1,1);
      glVertex3f(0,i,0);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f( 0.5*Cos(k), 0.5*Sin(k));
         glVertex3f(i*Cos(k), i, Sin(k));
      }
      glEnd();
   }

   //  Edge
   glColor3f(0.5, 0.5, 0.5);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k), +1, Sin(k));
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k), -1, Sin(k));
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void wallE_LeftEye(double x,double y,double z,double r,double d, double th, double ph)
{
   int i,k;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x-0.43,y+2.4,z);
   glRotated(th,1,0,0);
   glRotated(ph,0,1,0);
   glScaled(r,r,d);
   //  top and bottom
   glColor3f(0.8, 0.8, 0.8);
   for (i=1;i>=-1;i-=2)
   {
      glNormal3f(0,i,0);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(1,1);
      glVertex3f(0,i,0);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f( 0.5*Cos(k), 0.5*Sin(k));
         glVertex3f(i*Cos(k), i, Sin(k));
      }
      glEnd();
   }

   //  Edge
   glColor3f(0.5, 0.5, 0.5);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k), +1, Sin(k));
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k), -1, Sin(k));
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void rightEyeball(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x+0.43,y+2.4,z+.35);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=0;ph<360;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         glColor3f(0.0, 0.5, 1);
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
        
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

static void leftEyeball(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x-0.43,y+2.4,z+.35);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=0;ph<360;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         glColor3f(0.0, 0.5, 1);
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
        
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

static void wallE_RightWheel(double x,double y,double z,double r,double d, double th, double ph)
{
   int i,k;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x+1.3,y-0.2,z);
   glRotated(th,0,0,1);
   glRotated(ph,0,1,0);
   glScaled(r,r-.8,d);
   //  top and bottom
   glColor3f(0.2, 0.1, 0);
   for (i=1;i>=-1;i-=2)
   {
      glNormal3f(0,i,0);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(1,1);
      glVertex3f(0,i,0);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f( 0.5*Cos(k), 0.5*Sin(k));
         glVertex3f(i*Cos(k), i, Sin(k));
      }
      glEnd();
   }

   //  Edge
   glColor3f(0.4, 0.4, 0.4);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k), +1, Sin(k));
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k), -1, Sin(k));
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void wallE_LeftWheel(double x,double y,double z,double r,double d, double th, double ph)
{
   int i,k;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x-1.3,y-0.2,z);
   glRotated(th,0,0,1);
   glRotated(ph,0,1,0);
   glScaled(r,r-.8,d);
   //  top and bottom
   glColor3f(0.2, 0.1, 0);
   for (i=1;i>=-1;i-=2)
   {
      glNormal3f(0,i,0);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(1,1);
      glVertex3f(0,i,0);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f( 0.5*Cos(k), 0.5*Sin(k));
         glVertex3f(i*Cos(k), i, Sin(k));
      }
      glEnd();
   }

   //  Edge
   glColor3f(0.4, 0.4, 0.4);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k), +1, Sin(k));
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k), -1, Sin(k));
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}


static void wallEWhole(double x,double y, double z, double scale, double th, double ph)
{
   // Save transformation
   glPushMatrix();

   // Offset and scale
   glTranslated(x, y+1, z);
   glRotated(th, 1, 0, 0);
   glRotated(ph, 0, 0, 1);
   glScaled(scale, scale, scale);

   // Draw WallE
   wallEBody(0,0,0, 1, 0);
   wallE_neck(0,0,0, 0.2, 0.1, 0);
   wallE_RightEye(0, 0, 0, 0.4 ,0.3, 90, -18);
   wallE_LeftEye(0, 0, 0, 0.4 ,0.3, 90, 18);
   rightEyeball(0, 0, 0, 0.15);
   leftEyeball(0, 0, 0, 0.15);
   wallE_RightWheel(0, 0, 0, 1.2, 0.7, 90, 90);
   wallE_LeftWheel(0, 0, 0, 1.2, 0.7, 90, 90);

   // Undo transformations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates - ex8
 */
static void Vertex(double th,double ph)
{
   //glColor3f(0.7, 0.7, 0.7);
   glColor3f(.9, .9, 1);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

/*
 *  Draw a sphere (version 2) - from ex8 to make Evaa's Head
 *     at (x,y,z)
 *     radius (r)
 */
static void EvaaHead(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=0;ph<180;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=180;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw a sphere (version 2) - from ex8 to make Evaa's eyes
 *     at (x,y,z)
 *     radius (r)
 */
static void EvaaLeftEye(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x-0.07,y+0.1,z+0.23);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=0;ph<360;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         glColor3f(0.0, 0.5, 1);
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
        
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw a sphere (version 2) - from ex8 to make Evaa's eyes
 *     at (x,y,z)
 *     radius (r)
 */
static void EvaaRightEye(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x+0.07,y+0.1,z+0.23);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=0;ph<360;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         glColor3f(0.0, 0.5, 1);
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
        
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw a sphere (version 2) - from ex8 to make Evaa's Body
 *     at (x,y,z)
 *     radius (r)
 */
static void EvaaBody(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y-0.1,z);
   glScaled(r,-.9,r);

   //  Latitude bands
   for (ph=0;ph<180;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=180;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }
   //Need to make a top for Evaa's body

   //  Undo transformations
   glPopMatrix();}


static void evaWhole(double x, double y, double z, double th, double ph, double kh, double scale)
{
   glPushMatrix();
   glTranslated(x, y+1, z);
   glRotated(th, 1, 0, 0);
   glRotated(ph, 0, 0, 1);
   glScaled(scale, scale, scale);
   // EvaaHead(1,1,2,0.3);
   // EvaaBody(1,.9,2,0.3);
   // EvaaEyes(.92,1.15,2.2,0.05);
   // EvaaEyes(1.08,1.15,2.2,0.05);
   //EvaaArms(2,.8,2, 0, 35, 25, 0.3);
   EvaaHead(0,0,0,0.3);
   EvaaBody(0,0,0,0.3);
   EvaaLeftEye(0,0,0,0.05);
   EvaaRightEye(0,0,0,0.05);
   glPopMatrix();
}
/*
 *  Going to draw buildings as the third object
 */
static void building(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();

   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //t building structure
   //  Cube
   glBegin(GL_QUADS);
   //  Front normal gray
   glColor3f(.5, .5, .5);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back gray
   glColor3f(0.7, 0.7, 0.7);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right gray
   glColor3f(0.8,0.8,0.8);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left gray
   glColor3f(0.9, 0.9, 0.9);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top light gray
   glColor3f(0.9, 0.9, 0.9);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom dark gray
   glColor3f(0.5, 0.5, 0.5);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();

   // gonna try to make a roof
   //  triangle front
   glBegin(GL_TRIANGLES);
   glColor3f(0.0, 0.1, 0.2);
   glVertex3f(0,1.3,1);
   glVertex3f(1,1,1);
   glVertex3f(-1,1,1);
   glEnd();
   //right side
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.2);
   glVertex3f(0,1.3,1);
   glVertex3f(1,1,1);
   glVertex3f(1,1,-1);
   glVertex3f(0,1.3,-1);
   glEnd();
   //left side
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.2);
   glVertex3f(0,1.3,1);
   glVertex3f(-1,1,1);
   glVertex3f(-1,1,-1);
   glVertex3f(0,1.3,-1);
   glEnd();

   //  triangle back
   glBegin(GL_TRIANGLES);
   glColor3f(0.0, 0.1, 0.2);
   glVertex3f(0,1.3,-1);
   glVertex3f(1,1,-1);
   glVertex3f(-1,1,-1);
   glEnd();

   //gonna make a door
   //right door
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.2);
   glVertex3f(0.02,-0.6,1.1);
   glVertex3f(0.4,-0.6,1.1);
   glVertex3f(0.4,-1,1.1);
   glVertex3f(0.02,-1,1.1);
   glEnd();
   //left door
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.2);
   glVertex3f(-0.02,-0.6,1.1);
   glVertex3f(-0.4,-0.6,1.1);
   glVertex3f(-0.4,-1,1.1);
   glVertex3f(-0.02,-1,1.1);
   glEnd();

   //couple of windows
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(0.02,-0.5,1.1);
   glVertex3f(0.2,-0.5,1.1);
   glVertex3f(0.2,-0.3,1.1);
   glVertex3f(0.02,-0.3,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(-0.02,-0.5,1.1);
   glVertex3f(-0.2,-0.5,1.1);
   glVertex3f(-0.2,-0.3,1.1);
   glVertex3f(-0.02,-0.3,1.1);
   glEnd();
   //second floor
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(0.02,-0.2,1.1);
   glVertex3f(0.2,-0.2,1.1);
   glVertex3f(0.2,0,1.1);
   glVertex3f(0.02,0,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(-0.02,-0.2,1.1);
   glVertex3f(-0.2,-0.2,1.1);
   glVertex3f(-0.2,0,1.1);
   glVertex3f(-0.02,0,1.1);
   glEnd();
   //third floor
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(0.02,0.1,1.1);
   glVertex3f(0.2,0.1,1.1);
   glVertex3f(0.2,0.3,1.1);
   glVertex3f(0.02,0.3,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(-0.02,0.1,1.1);
   glVertex3f(-0.2,0.1,1.1);
   glVertex3f(-0.2,0.3,1.1);
   glVertex3f(-0.02,0.3,1.1);
   glEnd();
   //fourth floor
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(0.02,0.4,1.1);
   glVertex3f(0.2,0.4,1.1);
   glVertex3f(0.2,0.6,1.1);
   glVertex3f(0.02,0.6,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(-0.02,0.4,1.1);
   glVertex3f(-0.2,0.4,1.1);
   glVertex3f(-0.2,0.6,1.1);
   glVertex3f(-0.02,0.6,1.1);
   glEnd();
   //fifth floor 
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(0.02,0.7,1.1);
   glVertex3f(0.2,0.7,1.1);
   glVertex3f(0.2,0.9,1.1);
   glVertex3f(0.02,0.9,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glVertex3f(-0.02,0.7,1.1);
   glVertex3f(-0.2,0.7,1.1);
   glVertex3f(-0.2,0.9,1.1);
   glVertex3f(-0.02,0.9,1.1);
   glEnd();


   //  Undo transofrmations
   glPopMatrix();
}

static void theFloor(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){
   //  Save transformation
   glPushMatrix();

   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   // gonna try to make a floor
   //  plane
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.0);
   glVertex3f(-17,-17,-17);
   glVertex3f(+17,-17,-17);
   glVertex3f(+17,-17,+17);
   glVertex3f(-17,-17,+17);
   glEnd();

   //  Undo transofrmations
   glPopMatrix();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
  
   const double len=2.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //  Draw Wall-E's
   //wallEWhole( x, y,  z,  scale,  th,  ph)
   wallEWhole(1.5,-.75,0,.3,0,0);
   //far left wallE
   wallEWhole(-4,-.6,2,.5,0,0);
   //wallE on top of building
   wallEWhole(-0.7,3.4,-2,.2,0, 25);

   // Draw Evaa
   //evaWhole( x,  y,  z,  th,  ph,  kh,  scale)
   evaWhole(2.5, 0, 3, 0, 20, 0, 1);
   evaWhole(4, -.5, 3, 0, -30, 0, .5);
   //big Eva behind building
   evaWhole(-2.5, 1, -3, 0, -20, 0, 2);

   //Draw the building
   building(5,3,-1,2,3,1, -45);
   building(-4,2,-1,1.2,2,.5, 45);
   building(0,2,-2.2,1.2,2,.5, 0);
   

   // Draw a plane to simulate floor 
   theFloor(1,0,1,1,0,1,0);

   //tryRoof(3.5,4,-1,2,1,1, -45);

   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}


/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Change field of view angle
   else if (ch == '+' && ch>1)
      fov--;
   else if (ch == '-' && ch<179)
      fov++;
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(900,900);
   glutCreateWindow("Erika Bailon Hw3");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}




//Unused functions that might be useful 

/*
 *  Draw a sphere (version 2) - from ex8 to make Evaa's Arms - NEXT HOMEWORK
 *     at (x,y,z)
 *     radius (r)
 */
// static void EvaaArms(double x,double y,double z,double th, double ph, double kh, double r)
// {
//    const int d=5;
//    int i,k;

//    //  Save transformation
//    glPushMatrix();
//    //  Offset and scale
//    glTranslated(th,0,0);
//    glScaled(r*0.5,r,r);
//    glRotated(ph, 0, 1, 0);
//    glRotated(kh, 0, 0, 1);

//    // //  Latitude bands
//    // for (ph=0;ph<180;ph+=d)
//    // {
//    //    glBegin(GL_QUAD_STRIP);
//    //    for (th=0;th<=90;th+=d)
//    //    {
//    //       Vertex(th,ph);
//    //       Vertex(th,ph+d);
//    //    }
//    //    glEnd();
//    // }
//    glColor3f(0.5, 1, 0);
//    for (i=1;i>=-1;i-=3)
//    {
//       glNormal3f(0,0,i); //- for horizontal
//       // glNormal3f(0,i,0);
//       glBegin(GL_TRIANGLE_FAN);
//       glTexCoord2f(0.2,0.2);
//       glVertex3f(0,0,i); //- for horizontal
//       // glVertex3f(0,i+1,0);
//       for (k=0;k<=360;k+=10)
//       {
//          //cilinder horizontally
//          glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
//          glVertex3f(i*Cos(k),Sin(k),i+2);

//          // glTexCoord2f(0.5*Sin(k)+0.5, 0.5*Cos(k)+0.5); - no working
//          // glVertex3f(Sin(k),i*Cos(k),i);

//          // //cilinder vertically
//          // glTexCoord2f( 0.2*Cos(k)+0.2, 0.2*Sin(k)+0.2);
//          // glVertex3f(i*Cos(k), i, Sin(k));

//       }
//       glEnd();
//    }

//    //  Undo transformations
//    glPopMatrix();
// }



//cilinder vertical
// // Wall E head from example 19
   // glColor3f(0.5, 0.5, 0.5);
   // for (i=1;i>=-1;i-=2)
   // {
   //    // glNormal3f(0,0,i); - for horizontal
   //    glNormal3f(0,i,0);
   //    glBegin(GL_TRIANGLE_FAN);
   //    glTexCoord2f(0.2,0.2);
   //    // glVertex3f(0,0,i); - for horizontal
   //    glVertex3f(0,i,0);
   //    for (k=0;k<=360;k+=10)
   //    {
   //       //cilinder horizontally
   //       // glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
   //       // glVertex3f(i*Cos(k),Sin(k),i);

   //       // glTexCoord2f(0.5*Sin(k)+0.5, 0.5*Cos(k)+0.5); - no working
   //       // glVertex3f(Sin(k),i*Cos(k),i);

   //       //cilinder vertically
   //       glTexCoord2f( 0.1*Cos(k)+0.1, 0.1*Sin(k)+0.1);
   //       glVertex3f(i*Cos(k), i, Sin(k));

   //    }
   //    glEnd();
   // }
   // //  Edge
   // glColor3f(0.5, 0.5, 0.5);
   // glBegin(GL_QUAD_STRIP);
   // for (k=0;k<=360;k+=10)
   // {
   //    // glNormal3f(Cos(k),Sin(k),0);   - for horizontal
   //    // glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
   //    // glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   //    //for vertical
   //    glNormal3f(Cos(k),0,Sin(k));
   //    glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),+1, Sin(k));
   //    glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),-1, Sin(k));
   // }
   // glEnd();

   // // Draw Wall-E arms





// static void coin(double x,double y,double z,double r,double d, double th)
// {
//    int i,k;
//    //  Save transformation
//    glPushMatrix();
//    //  Offset and scale
//    glTranslated(x,y,z);
//    glRotated(th,1,0,0);
//    glScaled(r,r,d);
//    //  Head & Tail
//    glColor3f(0.5, 0.5, 0.5);
//    for (i=1;i>=-1;i-=2)
//    {
//       glNormal3f(0,0,i);
//       glBegin(GL_TRIANGLE_FAN);
//       glTexCoord2f(0.5,0.5);
//       glVertex3f(0,0,i);
//       for (k=0;k<=360;k+=10)
//       {
//          glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
//          glVertex3f(i*Cos(k),Sin(k),i);
//       }
//       glEnd();
//    }
//    //  Edge
//    glColor3f(0.5, 0.5, 0.5);
//    glBegin(GL_QUAD_STRIP);
//    for (k=0;k<=360;k+=10)
//    {
//       glNormal3f(Cos(k),Sin(k),0);
//       glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
//       glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
//    }
//    glEnd();
//    //  Undo transformations
//    glPopMatrix();
// }




// static void tryRoof(double x,double y,double z,
//                  double dx,double dy,double dz,
//                  double th)
// {
//    //  Save transformation
//    glPushMatrix();

//    //  Offset
//    glTranslated(x,y,z);
//    glRotated(0,0,th,0);
//    glScaled(dx,dy,dz);

//    // gonna try to make a roof
//    //  triangle front
//    glBegin(GL_TRIANGLES);
//    glColor3f(0.0, 0.1, 0.2);
//    glVertex3f(.5,1,1);
//    glVertex3f(1,0,1);
//    glVertex3f(0,0,1);
//    glEnd();
//    //right side
//    glBegin(GL_QUADS);
//    glColor3f(0.0, 0.1, 0.2);
//    glVertex3f(.5,1,1);
//    glVertex3f(1,0,1);
//    glVertex3f(1,0,0);
//    glVertex3f(.5,1,0);
//    glEnd();
//    //left side
//    glBegin(GL_QUADS);
//    glColor3f(0.0, 0.1, 0.2);
//    glVertex3f(.5,1,0);
//    glVertex3f(.5,1,1);
//    glVertex3f(0,0,1);
//    glVertex3f(0,0,0);
//    glEnd();

//    //  triangle back
//    glBegin(GL_TRIANGLES);
//    glColor3f(0.0, 0.1, 0.2);
//    glVertex3f(.5,1,0);
//    glVertex3f(1,0,0);
//    glVertex3f(0,0,0);
//    glEnd();

//    //  Undo transofrmations
//    glPopMatrix();
// }



