#include <stdio.h>
#include <stdlib.h>
//#include <grx.h>
#include <math.h>
#include <time.h>

#include "constant.H"

//extern int kbhit();
//extern void swirl ( int, int, double, double, double, double, int );
extern void get_args ( int argc, char *argv[], int *delay, int *range,
	double *x, double *y, int *color, int *mode, 
	int *height, int *width, int *numColors );

int n = 0; /* num colors */
int color = 0; /* drawing color */
int delay, range, height, width, numColors, mode;
time_t start;
double x, y, s1 = 7.0, s2 = 2.0;

int elapsed()
{
	time_t now;
	
	now = time ( NULL );

	if ( now - start >= delay )
		return 1;
	else
		return 0;
}

double param ()
{
	int i = random();

	return (double) ( (-1 * (i%2)) * ((random() % range) + 1) ); 
}

void loop()
{
	srandom ( time ( NULL ) );
	while ( 1 )
	{
		start = time ( NULL );
		do
		{
			x = param();
			y = param();
		} while ( (x == y) || (!x) || (!y) );

		//GrClearScreen(0);
		color = (random() % (n-1)) + 1;
		//swirl ( width, height, x, y, s1, s2, color );
	}
}

void single()
{
	//swirl ( width, height, x, y, s1, s2, color );
}

void swirlmain(int argc, char *argv[])
{

/* get command line arguments */
	get_args ( argc, argv, &delay, &range, &x, &y, &color, &mode, &height, 
		&width, &numColors );

	printf ("color depth: %d\n", n);
	printf ("last color used: %d\n", color );
	printf ("screen resolution (width x height): (%4d x %4d)\n", width, height );
	printf ("final plot parameters (x,y): (%f, %f)\n", x, y );
}
