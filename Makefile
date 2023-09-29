# Makefile Retro_6809
CC = g++
TARGET = ./bin/main
FLAGS = -std=c++20 -v #-g 
LIBS = -lSDL2 -ldl
OBJ = obj
OBJ_FILES = Bus.o Device.o Gfx.o Memory.o Mouse.o main.o

all: $(OBJ_FILES)	

Bus.o: 		Bus.cpp 	Bus.h
Device.o: 	Device.cpp 	Device.h
Gfx.o: 		Gfx.cpp 	Gfx.h
Memory.o: 	Memory.cpp 	Memory.h
Mouse.o:	Mouse.cpp 	Mouse.h
main.o: 	main.cpp

all: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(FLAGS) $(LIBS) -o $(TARGET)
	$(TARGET)

clean:
	rm -f $(OBJ_FILES)
	rm -f $(TARGET)

run:
	$(TARGET)