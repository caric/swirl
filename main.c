#include <stdio.h>
#include <stdlib.h>
#include <grx.h>
#include <math.h>

#include "constant.h"

extern int kbhit();
extern void swirl ( int, int, double, double, double, double, int, int () );
extern void get_args ( int argc, char *argv[], int *delay, int *range,
	double *x, double *y, int *color, int *mode, 
	int *height, int *width, int *numColors );

int n = 0; /* num colors */
int color = 0; /* drawing color */
int w, h, delay, range, height, width, numColors, mode;
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

void change ( int width, int height, int numColors, int *n, int *w, int *h )
{
/* change to graphics mode */
/* #define OLD */
	#ifdef OLD
		GrSetMode ( GR_default_graphics );
	#else
		GrSetMode ( GR_width_height_color_graphics, width, height, numColors );
	#endif

	*n = GrNumColors ( );
	*w = GrScreenX();
	*h = GrScreenY();
}

void loop()
{
	srandom ( time ( NULL ) );
	while ( !kbhit() )
	{
		start = time ( NULL );
		do
		{
			x = param();
			y = param();
		} while ( (x == y) || (!x) || (!y) );

		GrClearScreen(0);
		color = (random() % (n-1)) + 1;
		swirl ( w, h, x, y, s1, s2, color, elapsed );
	}
}

void single()
{
	swirl ( w, h, x, y, s1, s2, color, kbhit );
}

void main(int argc, char *argv[])
{

/* get command line arguments */
	get_args ( argc, argv, &delay, &range, &x, &y, &color, &mode, &height, 
		&width, &numColors );

/* change into graphics mode */
	change ( width, height, numColors, &n, &w, &h );

/* do the plotting */
	if ( mode == LOOP_MODE )
		loop();
	else
		single();

	getchar ();
	while (!kbhit());

/* change back to text mode and print info */
	GrSetMode ( GR_80_25_text );
	printf ("color depth: %d\n", n);
	printf ("last color used: %d\n", color );
	printf ("screen resolution (width x height): (%4d x %4d)\n", w, h );
	printf ("final plot parameters (x,y): (%f, %f)\n", x, y );
}
