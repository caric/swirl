# Makefile for g++

DEST=swirl

CC=gcc
CXX=g++
#CXXFLAGS=-I/progra~1/dosapps/djgpp/include/stl
CXXFLAGS=-g
CFLAGS=

##########################
FLAGS=$(CFLAGS)
COMP=$(CC)
DFILE=.makedepend
MKFILE=Makefile
EXT=c
DEPEND=-MM
LINKFLAGS=

.SUFFIXES:	.$(EXT)

#LIB=-lcurses
LIB=-lm

HDR=	constants.h

SRC=	swirl.$(EXT) \
	main.$(EXT) \
	args.$(EXT)

OBJS=	swirl.o \
	main.o \
	args.o

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
