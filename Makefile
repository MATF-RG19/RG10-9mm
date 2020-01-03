9mm: 9mm.o animate.o draw.o game.o alphabeta.o pravila.o heuristike.o
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -o 9mm *.o -lglut -lGLU -lGL -lpthread	
9mm.o: 9mm.cpp headers/animate.hpp headers/game.hpp
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c 9mm.cpp -lglut -lGLU -lGL	
game.o: sources/game.cpp headers/game.hpp headers/animate.hpp headers/draw.hpp headers/alphabeta.hpp
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c sources/game.cpp -lglut -lGLU -lGL -lpthread
animate.o: sources/animate.cpp headers/animate.hpp headers/draw.hpp
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c sources/animate.cpp -lglut -lGLU -lGL	
draw.o: sources/draw.cpp headers/draw.hpp
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c sources/draw.cpp -lglut -lGLU -lGL
alphabeta.o: sources/alphabeta.cpp headers/alphabeta.hpp headers/heuristike.hpp
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c sources/alphabeta.cpp -lglut -lGLU -lGL
pravila.o: sources/pravila.cpp headers/pravila.hpp headers/heuristike.hpp
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c sources/pravila.cpp -lglut -lGLU -lGL
heuristike.o: sources/heuristike.cpp headers/heuristike.hpp
	g++ -L/usr/X11R6/lib -L/usr/pkg/lib -c sources/heuristike.cpp -lglut -lGLU -lGL
	
