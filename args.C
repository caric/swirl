#include <stdio.h>
#include <stdlib.h>
#include <grx.h>
#include <math.h>

#include "constant.h"

void get_args ( int argc, char *argv[], int *delay, int *range, double *x, 
	double *y, int *color, int *mode, int *height, int *width, int *numColors )
{
	int i;

	*delay = DEFAULT_DELAY;
	*mode = DEFAULT_MODE;
	*x = DEFAULT_X;
	*y = DEFAULT_Y;
	*numColors = DEFAULT_NUM_COLORS;
	*height = DEFAULT_HEIGHT;
	*width = DEFAULT_WIDTH;
	*color = DEFAULT_COLOR;
	*range = DEFAULT_RANGE;

	i = 1;
	while ( i < argc )
	{
	/* help */
		if ( !strcmp ( argv[i], MODE_FLAG ) )
		{
			*mode = (!strcmp (argv[i+1], "single")) ? SINGLE_MODE : LOOP_MODE;
			i+=2;
		}
		else if ( !strcmp ( argv[i], X_FLAG ) )
		{
			*x = atof ( argv[i+1] );
			i+=2;
		}
		else if ( !strcmp ( argv[i], Y_FLAG ) )
		{
			*y = atof ( argv[i+1] );
			i+=2;
		}
		else if ( !strcmp ( argv[i], DELAY_FLAG ) )
		{
			*delay = atoi ( argv[i+1] );
			i+=2;
		}
		else if ( !strcmp ( argv[i], RANGE_FLAG ) )
		{
			*range = atoi ( argv[i+1] );
			i+=2;
		}
		else if ( !strcmp ( argv[i], WIDTH_FLAG ) )
		{
			*width = atoi ( argv[i+1] );
			i+=2;
		}
		else if ( !strcmp ( argv[i], HEIGHT_FLAG ) )
		{
			*height = atoi ( argv[i+1] );
			i+=2;
		}
		else if ( !strcmp ( argv[i], NUM_COLORS_FLAG ) )
		{
			*numColors = atoi ( argv[i+1] );
			i+=2;
		}
		else if ( !strcmp ( argv[i], COLOR_FLAG ) )
		{
			*color = atoi ( argv[i+1] );
			i+=2;
		}
		else
		{
			puts ("syntax: ");
			printf ("\nswirl ");
			printf ("[%s <mode: loop|single>] ", MODE_FLAG );
			printf ("[%s <x>] ", X_FLAG );
			printf ("[%s <y>] ", Y_FLAG );
			printf ("[%s <time limit>] ", DELAY_FLAG );
			printf ("\n  ");
			printf ("[%s <x & y range>] ", RANGE_FLAG );
			printf ("[%s <screen width>] ", WIDTH_FLAG );
			printf ("[%s <screen height>] ", HEIGHT_FLAG );
			printf ("\n  ");
			printf ("[%s <color depth: 16|256|32768>] ", NUM_COLORS_FLAG );
			printf ("[%s <initial color>] ", COLOR_FLAG );
		
			printf ("\n\ndefaults:\n");
			printf ("[%s %s] ", MODE_FLAG, 
				DEFAULT_MODE == LOOP_MODE ? "loop" : "single" );
			printf ("\n");
			printf ("[%s %d] ", DELAY_FLAG, DEFAULT_DELAY );
			printf ("[%s %d] ", RANGE_FLAG, DEFAULT_RANGE );
			printf ("\n");
			printf ("[%s %f] ", X_FLAG, DEFAULT_X );
			printf ("[%s %f] ", Y_FLAG, DEFAULT_Y );
			printf ("\n");
			printf ("[%s %d] ", WIDTH_FLAG, DEFAULT_WIDTH );
			printf ("[%s %d] ", HEIGHT_FLAG, DEFAULT_HEIGHT );
			printf ("[%s %d] ", NUM_COLORS_FLAG, DEFAULT_NUM_COLORS );
			printf ("[%s %d]", COLOR_FLAG, DEFAULT_COLOR );
	
			printf ("\n\nduring plot, hit RETURN to halt. hit RETURN again to"
				" enter text mode.\n" );
			exit ( 0 );
		}
	}
}
