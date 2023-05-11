# Makefile
CC = g++
TARGET = ./bin/main
FLAGS = -std=c++20 #-g
LIBS = -lSDL2 
OBJ_FILES = Bus.o Device.o Gfx.o Memory.o Mouse.o main.o

all: $(OBJ_FILES)	

Bus.o: 		Bus.cpp 	Bus.h
Device.o: 	Device.cpp 	Device.h
Gfx.o: 		Gfx.cpp 	Gfx.h
Memory.o: 	Memory.cpp 	Memory.h
Mouse.o: 	Mouse.cpp 	Mouse.h
main.o: 	main.cpp

all: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(FLAGS) $(LIBS) -o $(TARGET)
	#$(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
