9mm: 9mm.o draw.o
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -o 9mm *.o -lglut -lGLU -lGL
9mm.o: 9mm.cpp headers/draw.hpp 
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c 9mm.cpp -lglut -lGLU -lGL
draw.o: sources/draw.cpp headers/draw.hpp 
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c sources/draw.cpp -lglut -lGLU -lGL
