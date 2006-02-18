/* Kevin Geiss kevin@desertsol.com http://www.desertsol.com/~kevin */
/* released under the GPL */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

//#define WINDOW_WIDTH 64
//#define WINDOW_HEIGHT 64

void swirl( const int width, const int height,
            Display *display, GC gc, Window win,
            unsigned long bg, unsigned long fg );

void process_event(XEvent report, int& quit );

long elapsed( timeval& start )
{
  timeval now;
  long diff;
  const long usecs = 1000000;

  gettimeofday( &now, NULL );
  diff = usecs * (now.tv_sec - start.tv_sec) +
                 now.tv_usec - start.tv_usec;

  start = now;
  return diff;
}

long delay( long target )
{
  const long usecs = 1000000;

  // Handle system suspend.
  if ( target / usecs > 1 )
    target = -1;

  if ( target > 0 )
  {
    timeval t;
    t.tv_usec = target % usecs;
    t.tv_sec = target / usecs;
    select( 0, NULL, NULL, NULL, &t );
  }
  return target > 0 ? target : 0;
}

struct ColorTable
{
  int pixel;
  int r, g, b;
};

void alloc_color( int i, int r, int g, int b,
                  Display *display,
                  Colormap &cmap,
                  ColorTable* idx,
                  const int ctableSize,
                  int screen,
                  Window win )
{
  XColor col;

  col.red = r; col.green = g; col.blue = b;
  col.flags = DoRed | DoGreen | DoBlue;
  if (XAllocColor(display, cmap, &col))
  {
    idx[i].pixel = col.pixel;
  }else
  {
    if (cmap == DefaultColormap(display, screen))
    {
      cmap = XCopyColormapAndFree(display, cmap);
      XSetWindowColormap(display, win, cmap);
      col.red = r; col.green = g; col.blue = b;
      col.flags = DoRed | DoGreen | DoBlue;
      if (XAllocColor(display, cmap, &col))
      {
        idx[i].pixel = col.pixel;
      }
    }
  }
}

unsigned long get_color(Display *display, char *color_name)
{
  XColor color;
  XWindowAttributes winattr;

  XGetWindowAttributes(display,
    RootWindow(display, DefaultScreen(display)), &winattr);

  color.pixel = 0;
  XParseColor(display, winattr.colormap, color_name, &color);

  color.flags = DoRed | DoGreen | DoBlue;
  XAllocColor(display, winattr.colormap, &color);

  return color.pixel;
}
int get_color(char *col,
              ColorTable* idx,
              const int ctableSize,
              Display *display,
              Colormap cmap )
{
  int i, cindx;
  double rd, gd, bd, dist, mindist;
  XColor color;
  XColor def_colrs[256];

  // create a color from the input string
  XParseColor(display, cmap, col, &color);

  // find closest match
  cindx = -1;
  mindist = 196608.0;             // 256.0 * 256.0 * 3.0
  for (i=0; i<256; i++)
  {
    rd = (idx[i].r - color.red) / 256.0;
    gd = (idx[i].g - color.green) / 256.0;
    bd = (idx[i].b - color.blue) / 256.0;
    dist = (rd * rd) + (gd * gd) + (bd * bd);
    if (dist < mindist)
    {
      mindist = dist;
      cindx = idx[i].pixel;
      if (dist == 0.0) break;
    }
  }
  return cindx;
}

void help()
{
  printf("usage: swirl [-w]\n\n-w: Create X window in withdrawn state.\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  bool withdrawn = false;
  const char *const withdrawnFlag = "-w";
  int quit = 0;

  char *window_name = "Swirl";
  char *icon_name = "Swirl";

  const int dockWidth = 64, dockHeight = 64;
  const int winWidth = 400, winHeight = 400;

  srand( time(NULL) );

  // read args.
  if ( argc >= 2 )
  {
    for ( int i = 1; i < argc; i++ )
    {
      if ( !strcmp( argv[i], withdrawnFlag ) )
        withdrawn = true;
      else
        help();
    }
  }
  unsigned int width, height;
  if ( withdrawn )
  {
    width = dockWidth;
    height = dockHeight;
  }
  else
  {
    width = winWidth;
    height = winHeight;
  }

  // create

  // Open the DISPLAY.
  Display *display = XOpenDisplay(NULL);
  if (display == NULL)
  {
    fprintf(stderr, "Failed to open display\n");
    return 0;
  }

  int screen = DefaultScreen(display);
  int depth = DefaultDepth(display, screen);
  Colormap cmap = DefaultColormap(display, screen);
  Window rootwin = RootWindow(display, screen);

  static XSizeHints size_hints;
  size_hints.flags = USSize|USPosition;
  size_hints.x = 0;
  size_hints.y = 0;
  size_hints.width = width;
  size_hints.height = height;

  unsigned long bg = BlackPixel(display, screen);
  unsigned long fg = get_color(display, "blue" );

  Window win = XCreateSimpleWindow(display, rootwin, 0, 0, width, height, 0,
    fg, bg );
  Window iconwin = XCreateSimpleWindow(display, win, 0, 0, width, height, 0,
    fg, bg );

  XSetWMNormalHints(display, win, &size_hints);
  XClassHint classhint;
  classhint.res_name = window_name;
  classhint.res_class = window_name;
  XSetClassHint(display, win, &classhint);

  // Tell X what events we are interested in.
  XSelectInput(display, win, ButtonPressMask | KeyPressMask);
  XSelectInput(display, iconwin, ButtonPressMask | KeyPressMask);

  XTextProperty wname;
  XStringListToTextProperty(&window_name, 1, &wname);
  XSetWMName(display, win, &wname);

#if 0
  // Set up the colormap.
  XSetWindowColormap(display, win, cmap);

  int i, j, k;
  int count = 0;

  const int ctableSize = 256;
  ColorTable idx[ ctableSize ];
  for (i=0; i<256; i++) idx[i].pixel = 0;

  for (i=0; i<5; i++)
    for (j=0; j<5; j++)
      for (k=0; k<5; k++)
      {
        idx[count].r = 65535 - (i*16384); if(idx[count].r<0) idx[count].r=0;
        idx[count].g = 65535 - (j*16384); if(idx[count].g<0) idx[count].g=0;
        idx[count].b = 65535 - (k*16384); if(idx[count].b<0) idx[count].b=0;
        alloc_color(count++, idx[count].r, idx[count].g, idx[count].b,
                    display, cmap, idx, ctableSize, screen, win );
      }

  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
      for (k=0; k<4; k++)
      {
        idx[count].r = 60415 - (i*16384); if(idx[count].r<0) idx[count].r=0;
        idx[count].g = 60415 - (j*16384); if(idx[count].g<0) idx[count].g=0;
        idx[count].b = 60415 - (k*16384); if(idx[count].b<0) idx[count].b=0;
        alloc_color(count++, idx[count].r, idx[count].g, idx[count].b,
                    display, cmap, idx, ctableSize, screen, win );
        idx[count].r = 55295 - (i*16384); if(idx[count].r<0) idx[count].r=0;
        idx[count].g = 55295 - (j*16384); if(idx[count].g<0) idx[count].g=0;
        idx[count].b = 55295 - (k*16384); if(idx[count].b<0) idx[count].b=0;
        alloc_color(count++, idx[count].r, idx[count].g, idx[count].b,
                    display, cmap, idx, ctableSize, screen, win );
      }

  XGCValues values;
  fg = get_color("#0000ff", idx, ctableSize, display, cmap );
  values.foreground = fg;
  values.background = bg;
  values.graphics_exposures = 0;
#endif
  XGCValues gcval;
  gcval.foreground = fg;
  gcval.background = bg;
  gcval.graphics_exposures = 0;
  // Create graphics context.
  unsigned long valuemask = GCForeground|GCBackground|GCGraphicsExposures;
  GC gc = XCreateGC(display, win, valuemask, &gcval);
  //copygc = XCreateGC(display, win, valuemask, &values);
  
#if 0
  XSetForeground(display, gc, fg );

  // Set up hints and properties.
  size_hints.flags = PSize | PMinSize | PMaxSize;
  size_hints.min_width = width;
  size_hints.max_width = width;
  size_hints.min_height = height;
  size_hints.max_height = height;

  XSetStandardProperties(display, win, window_name, icon_name, None,
    0, 0, &size_hints); 
#endif

  XWMHints    mywmhints;
  if ( withdrawn )
  {
    //XShapeCombineMask(display, win, ShapeBounding, 0, 0, pixmask, ShapeSet);
    //XShapeCombineMask(display, iconwin, ShapeBounding, 0, 0, pixmask, ShapeSet);
    mywmhints.initial_state = WithdrawnState;
    mywmhints.icon_window = iconwin;
    mywmhints.icon_x = size_hints.x;
    mywmhints.icon_y = size_hints.y;
    mywmhints.window_group = win;
    mywmhints.flags = StateHint | IconWindowHint | IconPositionHint | WindowGroupHint;
    XSetWMHints(display, win, &mywmhints);
  }

  // Get the window displayed.
  XMapWindow(display, win);
// Set background to parent's background.
// XSetWindowBackgroundPixmap( display, win, ParentRelative );

  //Pixmap buffer = XCreatePixmap(display, win, WINDOW_WIDTH, WINDOW_HEIGHT, depth);

  timeval start;
  long howLongAgo, lastPause;
  const long framelength = 50000; // useconds
  gettimeofday( &start, NULL );
  // eventloop
  while ( !quit )
  {
    // Calculate how long it's been since last time.
    howLongAgo = elapsed( start );
    // Delay for framelength - ( howLongAgo + lastPause ) usecs
    lastPause = delay( framelength - ( howLongAgo + lastPause) );

    if ( withdrawn )
      swirl( width, height, display, gc, iconwin, bg, fg );
    else
      swirl( width, height, display, gc, win, bg, fg );
    
    //XFlush(display);
    XEvent xev;
    int num_events = XPending(display);
    while((num_events != 0))
    {
      num_events--;
      XNextEvent(display, &xev);
      process_event(xev, quit );
    }
  }

  // close
  XCloseDisplay(display);

  return 0;
}


void process_event(XEvent report, int& quit )
{
  KeySym key;

  switch(report.type)
  {
    case KeyPress:
      key = XLookupKeysym(&report.xkey, 0);
      switch(key)
      {
        case(XK_KP_4):
        case(XK_Left):
        case(XK_KP_Left):
          printf("Left\n");
          break;
        case(XK_KP_6):
        case(XK_Right):
        case(XK_KP_Right):
          printf("Right\n");
          break;
        case(XK_KP_8):
        case(XK_Up):
        case(XK_KP_Up):
          printf("Up\n");
          break;
        case(XK_KP_2):
        case(XK_Down):
        case(XK_KP_Down):
          printf("Down\n");
          break;
        case(XK_q):
          printf("q\n");
          quit = 1;
          break;
        default: break;
      }
      break;

    case ButtonPressMask:
      printf("You pressed button %d\n", report.xbutton.button);
      break;

    default:
      break;
  }
}
