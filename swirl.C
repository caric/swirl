/* 
	neat swirly patterns

	Kevin Geiss
	4-5-96

*/

#include <stdio.h>
#include <stdlib.h>
extern "C" {
#include <gtk/gtk.h>
}
#include <math.h>

#include "constant.H"

//extern int kbhit();
extern int delay, range, height, width, numColors, mode;
extern double x, y, s1, s2;
extern GdkPixmap *pixmap;

/* Draw a rectangle on the screen */
gint draw_brush (GtkWidget *widget, gdouble mouseX, gdouble mouseY)
{
  GdkRectangle update_rect;

	static double i = 0.0, j = 0.0;
	double ratio;
	double x_trans, y_trans, x_scale, y_scale;
  double X, Y;
  int counter;
  const int numIterations = 1000;

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

    update_rect.x = X;
    update_rect.y = Y;
    update_rect.width = 1;
    update_rect.height = 1;
    gdk_draw_rectangle (pixmap,
            widget->style->black_gc,
            //widget->style->green_gc,
            TRUE,
            update_rect.x, update_rect.y,
            update_rect.width, update_rect.height);
    gtk_widget_draw (widget, &update_rect);
	}
  return 1;
}

extern GtkWidget *vbox;
gint swirl( gpointer data )
{
  return draw_brush( vbox, 0, 0 );
}
