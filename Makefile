PROGRAM    = matfsim
CC	       = gcc
CFLAGS     = -g -Wall
LDLIBS     = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o
	$(CC) $(CFLAGS) -o $(PROGRAM) main.o $(LDLIBS)
	
main.o: main.c matfsim.h
	$(CC) $(CFLAGS) -c main.c -o main.o $(LDLIBS)

clean: 
	-rm *.o $(PROGRAM)
