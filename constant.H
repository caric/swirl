#ifndef _CONSTANT_H_
#define _CONSTANT_H_

/* constant.h */

/*	Kevin Geiss */
/*	9-6-96	*/

/* constants for the getargs function */
#define LOOP_MODE 0
#define SINGLE_MODE 1

#define COLOR_FLAG "-c"
#define HEIGHT_FLAG "-h"
#define WIDTH_FLAG "-w"
#define NUM_COLORS_FLAG "-nc"
#define X_FLAG "-x"
#define Y_FLAG "-y"
#define DELAY_FLAG "-d"
#define RANGE_FLAG "-r"
#define MODE_FLAG "-m"

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define DEFAULT_NUM_COLORS 16
#define DEFAULT_COLOR 2
#define DEFAULT_DELAY 15
#define DEFAULT_RANGE 100
#define DEFAULT_MODE LOOP_MODE
#define DEFAULT_X -.0004
#define DEFAULT_Y .0009

/* swirl constants */
#define TRANS 2.0
#define SCALE 24.0

#endif _CONSTANT_H_
