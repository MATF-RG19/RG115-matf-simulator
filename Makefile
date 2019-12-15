PROGRAM    = matfsim
CC	       = gcc
CFLAGS     = -g -Wall
LDLIBS     = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o light.o
	$(CC) $(CFLAGS) -o $(PROGRAM) main.o light.o $(LDLIBS)
	
main.o: main.c matfsim.h
	$(CC) $(CFLAGS) -c main.c -o main.o $(LDLIBS)

light.o: light.c light.h
	$(CC) $(CFLAGS) -c light.c -o light.o $(LDLIBS)

clean: 
	-rm *.o $(PROGRAM)
