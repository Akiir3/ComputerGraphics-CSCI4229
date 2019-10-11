/*	Erika Bailon
*	Homework 5 - Lighting 
*	The light should move to demonstrate changes to the scene as a result of
*	different light positions.
*	Choose your light positions such that the lighting effects are
*	demonstrated well.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "CSCIx229.h"


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
int turn = 0;     //  Turns the view of the world 

/* First person View */
int fpV = 0, fpA = 0; // first person vie-2, angle
double eyeX = 0, eyeY = 0, eyeZ = 5; // eye level, start back a bit
double camX = 0, camY = 0, camZ = 0; // camera for second

/* Variables to work on lighting from example 13 */
int move=1;       //  Move light
int light=1;      //  Lighting
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light


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
   glNormal3f(0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back yello darker
   glColor3f(1, 1, 0);
   glNormal3f(0, 0, -1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right yellow medium
   glColor3f(1, 1, 0);
   glNormal3f(+1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left yellow lighter
   glColor3f(1, 1, 0);
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top kind of mustard
   glColor3f(1, 1, 0);
   glNormal3f( 0,+1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom dark gray
   glColor3f(0.5, 0.5, 0.5);
   glNormal3f( 0,-one, 0);
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
         glNormal3f(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glNormal3f(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
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
         glNormal3f(Sin(th)*Cos(ph),Sin(ph),Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glNormal3f(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
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
         //glTexCoord2f( 0.5*Cos(k), 0.5*Sin(k));
         glVertex3f(i*Cos(k), i, Sin(k));
      }
      glEnd();
   }

   //  Edge
   glColor3f(0.4, 0.4, 0.4);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      // glNormal3f(Cos(k),Sin(k),0);
      //play with this till it works
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
   glNormal3d(Sin(th)*Cos(ph),Sin(ph),Cos(th)*Cos(ph));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

// /*
//  *  Draw vertex in polar coordinates with normal
//  */
// static void Vertex(double th,double ph)
// {
//    double x = Sin(th)*Cos(ph);
//    double y = Cos(th)*Cos(ph);
//    double z =         Sin(ph);
//    //  For a sphere at the origin, the position
//    //  and normal vectors are the same
//    glNormal3d(x,y,z);
//    glVertex3d(x,y,z);
// }

/* Draw a sphere (version 2) - from ex8 to make Evaa's Head
 * at (x,y,z)
 * radius (r)
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

   //normals might be reversed so multiply for -1
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
   glNormal3f( 0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back gray
   glColor3f(.5, .5, .5);
   glNormal3f( 0, 0, -1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right gray
   glColor3f(.5, .5, .5);
   glNormal3f( +1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left gray
   glColor3f(.5, .5, .5);
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top light gray
   glColor3f(.5, .5, .5);
   glNormal3f( 0, +1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom dark gray
   glColor3f(0.5, 0.5, 0.5);
   glNormal3f( 0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();

   // gonna try to make a roof
   //  triangle front
   //roof edges take the drot product of the vectors that make that roof
   glBegin(GL_TRIANGLES);
   glColor3f(0.0, 0.1, 0.2);
   glNormal3f(0, -0, -1);
   glVertex3f(0,1.3,1);
   glVertex3f(1,1,1);
   glVertex3f(-1,1,1);
   glEnd();
   //right side
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.2);
   glNormal3f(+1, 0, 0);
   glVertex3f(0,1.3,1);
   glVertex3f(1,1,1);
   glVertex3f(1,1,-1);
   glVertex3f(0,1.3,-1);
   glEnd();
   //left side
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.2);
   glNormal3f(-1, 0, 0);
   glVertex3f(0,1.3,1);
   glVertex3f(-1,1,1);
   glVertex3f(-1,1,-1);
   glVertex3f(0,1.3,-1);
   glEnd();

   //  triangle back
   glBegin(GL_TRIANGLES);
   glColor3f(0.0, 0.1, 0.2);
   glNormal3f(0, -0, -1);
   glVertex3f(0,1.3,-1);
   glVertex3f(1,1,-1);
   glVertex3f(-1,1,-1);
   glEnd();

   //gonna make a door
   //right door
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.2);
   glNormal3f( 0, 0, 1);
   glVertex3f(0.02,-0.6,1.1);
   glVertex3f(0.4,-0.6,1.1);
   glVertex3f(0.4,-1,1.1);
   glVertex3f(0.02,-1,1.1);
   glEnd();
   //left door
   glBegin(GL_QUADS);
   glColor3f(0.0, 0.1, 0.2);
   glNormal3f( 0, 0, 1);
   glVertex3f(-0.02,-0.6,1.1);
   glVertex3f(-0.4,-0.6,1.1);
   glVertex3f(-0.4,-1,1.1);
   glVertex3f(-0.02,-1,1.1);
   glEnd();

   //couple of windows
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(0.02,-0.5,1.1);
   glVertex3f(0.2,-0.5,1.1);
   glVertex3f(0.2,-0.3,1.1);
   glVertex3f(0.02,-0.3,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(-0.02,-0.5,1.1);
   glVertex3f(-0.2,-0.5,1.1);
   glVertex3f(-0.2,-0.3,1.1);
   glVertex3f(-0.02,-0.3,1.1);
   glEnd();
   //second floor
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(0.02,-0.2,1.1);
   glVertex3f(0.2,-0.2,1.1);
   glVertex3f(0.2,0,1.1);
   glVertex3f(0.02,0,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(-0.02,-0.2,1.1);
   glVertex3f(-0.2,-0.2,1.1);
   glVertex3f(-0.2,0,1.1);
   glVertex3f(-0.02,0,1.1);
   glEnd();
   //third floor
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(0.02,0.1,1.1);
   glVertex3f(0.2,0.1,1.1);
   glVertex3f(0.2,0.3,1.1);
   glVertex3f(0.02,0.3,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(-0.02,0.1,1.1);
   glVertex3f(-0.2,0.1,1.1);
   glVertex3f(-0.2,0.3,1.1);
   glVertex3f(-0.02,0.3,1.1);
   glEnd();
   //fourth floor
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(0.02,0.4,1.1);
   glVertex3f(0.2,0.4,1.1);
   glVertex3f(0.2,0.6,1.1);
   glVertex3f(0.02,0.6,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(-0.02,0.4,1.1);
   glVertex3f(-0.2,0.4,1.1);
   glVertex3f(-0.2,0.6,1.1);
   glVertex3f(-0.02,0.6,1.1);
   glEnd();
   //fifth floor 
   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
   glVertex3f(0.02,0.7,1.1);
   glVertex3f(0.2,0.7,1.1);
   glVertex3f(0.2,0.9,1.1);
   glVertex3f(0.02,0.9,1.1);
   glEnd();

   glBegin(GL_QUADS);
   glColor3f(0.5, 0.8, 0.9);
   glNormal3f( 0, 0, 1);
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
                 double th)
{
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
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,3);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
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
   
   //   First perspective view
   if(fpV) {
      camX = +2*dim*Sin(fpA); // Change camera dimensions 
      camZ = -2*dim*Cos(fpA);

      gluLookAt(eyeX, eyeY, eyeZ, camX+eyeX,eyeY,camZ+eyeZ, 0,1,0); //  Use gluLookAt
   }

   else{
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
   }
   

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.2);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);

   //  Draw Wall-E's
   //wallEWhole( x, y,  z,  scale,  th,  ph)
   wallEWhole(1.5,-.75,0,.3,0,0);
   //far left wallE
   wallEWhole(-1,-.6,2,.5,0,0);
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
   if (fpV) Print("fpV: %s  Angle: %d   Dim: %.1f        Press f/F to exit FPV", fpV?"ON":"OFF", fpA, dim);
   else  Print("Angle: %d,%d     Dim: %.1f    FOV: %d    Projection: %s\n         Press f/F for FPV",th, ph, dim, fov, mode?"Perpective":"Orthogonal");
   
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}


/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;

   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   // Project();
   Project(mode?fov:0,asp,dim, fpV);
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
   else if ((ch == 'a' || ch == 'A') && !fpV)
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Change field of view angle
   else if (ch == '+' && ch>1)
      fov--;
   else if (ch == '-' && ch<179)
      fov++;
//LIGHT
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
  //  Toggle light movement
   else if (ch == 'g' || ch == 'G')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='h' && ambient>0)
      ambient -= 5;
   else if (ch=='H' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='j' && diffuse>0)
      diffuse -= 5;
   else if (ch=='J' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='k' && specular>0)
      specular -= 5;
   else if (ch=='K' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='u' && emission>0)
      emission -= 5;
   else if (ch=='U' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
//FPV  
   //    Activate First Person
   else if (ch == 'f' || ch == 'F')
      fpV = 1 - fpV;
   if(fpV){
      //   First person view moving forward
      if(ch == 'w' || ch == 'W'){
         if(fpV){
            eyeX += camX * .02;
            eyeZ += camZ * .02;
         }
      }
      //    First person view moving backwards
      else if(ch == 's' || ch == 'S'){
         if(fpV){
            eyeX -= camX * .02;
            eyeZ -= camZ * .02;
         }
      }
      //    First person move left
      else if(ch == 'a' || ch == 'A'){
         if(fpV) fpA -= 1;
      }
      //    First person move right
      else if(ch == 'd' || ch == 'D'){
         if(fpV) fpA += 1;
      }
      //    First person move up 
      else if(ch == 'q' || ch == 'Q'){
         if(fpV) eyeY -= 0.05;
      }
      //    First person move down 
      else if(ch == 'e' || ch == 'E'){
         if(fpV) eyeY += 0.05;
      }
   }

   fpA = fpA % 360;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   // Project();
   Project(mode?fov:0,asp,dim, fpV);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
   // Project();
   Project(mode?fov:0,asp,dim, fpV);
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
   glutCreateWindow("Erika Bailon Hw5");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
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



