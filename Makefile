CC=g++
IPATH=./include
LPATH=-
LIBS=-lglfw -lGLEW -lGL
CFLAGS=-I $(IPATH) -L $(LPATH) $(LIBS)

maincpp:
	$(CC) main.cpp src/*.cpp include/imgui/*.cpp -o bin/out $(CFLAGS) 
