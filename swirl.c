/* 
	neat swirly patterns

	Kevin Geiss
	4-5-96

	this program requires a math coprocessor. if you don't
	have one, you have to use the emulator (emu387).

	It should find it automatically if you have go32.exe and emu387
	in the same directory as this program. If it doesn't, tell it where
	to look with

set go32=emu <path>/emu387

	replace <path> with the pathname of the directory where emu387 is. Note
	that you must use the ``/'' character between directory names instead
	of the standard ``\''.
*/

#include <stdio.h>
#include <stdlib.h>
#include <grx.h>
#include <math.h>

#include "constant.h"

extern int kbhit();

void swirl( int w, int h, double a, double b, double s1, 
	double s2, int c, int func() )
{
	register double i, j;
	double ratio;
	double x, y;
	double x_trans, y_trans, x_scale, y_scale;

/* calculate scaling and translation factors */
	ratio =  ((double)w) / ((double)h) ;
	x_trans = w / TRANS;
	y_trans = h / TRANS;
	x_scale = w / SCALE;
	y_scale = h / (SCALE / ratio);

/* plot */
	for ( i = j = 0.0; !kbhit(); i += a, j += b )
	{
		x = s1*sin(i) + s2*cos(j);
		y = s1*cos(i) + s2*sin(j);

		GrPlot ( (int)(x_scale * (x) + x_trans), 
			(int)(y_scale * (y) + y_trans), c );
		if ( !( ((int)i) % 4 ) )
		{
			if ( func() )
				break;
		}	
	}
}
