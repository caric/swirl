# Makefile for g++

DEST=swirl

CC=gcc
CXX=g++
#CXXFLAGS=-I/progra~1/dosapps/djgpp/include/stl
CXXFLAGS=-I/usr/X11R6/include -I/usr/lib/glib/include -I/usr/include
CFLAGS=-I/usr/X11R6/include -I/usr/lib/glib/include -I/usr/include

##########################
FLAGS=$(CXXFLAGS)
COMP=$(CXX)
DFILE=.makedepend
MKFILE=Makefile
EXT=C
DEPEND=-MM
LINKFLAGS=

.SUFFIXES:	.$(EXT)

#LIB=-lcurses
LIB=-lm `gtk-config --libs`

HDR=	constant.H

SRC=	swirl.$(EXT) \
	main.$(EXT) \
	args.$(EXT) \
	scribble-simple.$(EXT)

OBJS=	swirl.o \
	main.o \
	args.o \
	scribble-simple.o

##########################

all:	$(DEST)
	@echo done.

$(DEST):	$(OBJS)
	$(COMP) $(LINKFLAGS) -o $(DEST) $(OBJS) $(LIB)

%.o:	%.$(EXT) $(MKFILE) $(DFILE)
	$(COMP) $(FLAGS) -c $<

$(DFILE):	$(MKFILE)
	$(COMP) $(DEPEND) $(FLAGS) $(SRC) > $(DFILE)
	make

depend:
	$(COMP) $(DEPEND) $(FLAGS) $(SRC) > $(DFILE)

clean:
	rm -f *.o 
	rm -f $(DEST)
	rm -f $(DFILE)

include $(DFILE)
