/* Kevin Geiss kevin@desertsol.com http://www.desertsol.com/~kevin */
/* released under the GPL */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

double param ()
{
  const int range = 15;
  const int fpRange = 2;
  int sign = rand();
  double result;
  double decimal;

  result = (double) ( (-1 * (sign%2)) * ((rand() % range) + 1) );
  decimal = pow( 10, -1 * (rand() % fpRange) );
  return result * decimal;
}

void swirl( const int width, const int height,
            Display *display, GC gc, Window win,
            unsigned long bg, unsigned long fg )
{
  static double i = 0.0, j = 0.0;
  static int num = 0;
  static double x = -0.000400, y = 0.000900;
  const int max = 400;

  double ratio;
  double x_trans, y_trans, x_scale, y_scale;
  int X, Y;
  int counter;
  
  const double s1 = 7.0, s2 = 2.0;
  const int numIterations = 1000;
  const double TRANS = 2.0;
  const double SCALE = 24.0;

  if ( ++num > max )
  {
    do {
      x = param();
      y = param();
    } while ( (x == 0.0) || (y == 0.0) || (fabs(x - y) <= .001) );
 //printf( "x: %lf; y: %lf\n", x, y );
    num = 0;
    // Clear the window.
    XSetForeground( display, gc, bg );
    XFillRectangle( display, win, gc, 0, 0, width, height );
    XSetForeground( display, gc, fg );
  }

  /* calculate scaling and translation factors */
  ratio =  ((double)width) / ((double)height) ;
  x_trans = width / TRANS;
  y_trans = height / TRANS;
  x_scale = width / SCALE;
  y_scale = height / (SCALE / ratio);

  /* plot */
  for ( counter = 0; counter < numIterations; counter++, i += x, j += y )
  {
    X = (int)(x_scale * (s1*sin(i) + s2*cos(j)) + x_trans);
    Y = (int)(y_scale * (s1*cos(i) + s2*sin(j)) + y_trans);

    XDrawPoint( display, win, gc, X, Y );
  }
}
