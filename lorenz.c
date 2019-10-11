/*
 * Simple program to demonstrate generating coordinates
 * using the Lorenz Attractor
 */

#include <stdio.h>

/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;

/*
 *  Main
 */
int main(int argc, char *argv[])
{
   int i;
   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;
   /*  Time step  */
   double dt = 0.001;

   printf("%5d %8.3f %8.3f %8.3f\n",0,x,y,z);
   /*
    *  Integrate 50,000 steps (50 time units with dt = 0.001)
    *  Explicit Euler integration
    */
   for (i=0;i<50000;i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
      printf("%5d %8.3f %8.3f %8.3f\n",i+1,x,y,z);
   }
   return 0;
}
/* COMMENTS FROM OFFICE HOURS

*I have to plot those points wiht a 3d program using openGL manage the rotation and ortographic manipulation. To plot 3d figures on 3 axes
*using the x,y,z coordinates given - command GLvertex we have to have those available for that. 
*
*glBegin(GL_POINTS);                      // Select points as the primitive - After the Begin we need a for statement so we can have lines between the points 
*        glVertex3f(0.0f, 0.0f, 0.0f);    // Specify a point   -   this one is for the points  - Use a for loop here
*        glVertex3f(50.0f, 50.0f, 50.0f); // Specify another point  -- this one is the location 
*glEnd();   
*
*those can only be called thorught he function in open gl in the display function Do the display function and and that function should call drawlorenz and drawlorenz */
