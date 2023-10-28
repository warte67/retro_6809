# Makefile Retro_6809
CC = g++
TARGET = ./bin/main
FLAGS = -std=c++20 -O2 #-g -V
LIBS = -lSDL2 -ldl
OBJ = obj
OBJ_FILES = Bus.o Device.o Gfx.o C6809.o GfxDebug.o GfxMouse.o Keyboard.o FileIO.o main.o

all: $(OBJ_FILES)	 
	Bus.o: 		Bus.cpp 	Bus.h
	main.o: 	main.cpp
	Device.o: 	Device.cpp 	Device.h
	Gfx.o: 		Gfx.cpp 	Gfx.h
	C6809.o:	C6809.cpp	C6809.h
	FileIO.o:	FileIO.cpp	FileIO.h
	Keyboard.o:	Keyboard.cpp	Keyboard.h
	GfxDebug.o:	GfxDebug.cpp	IGfxDevice.h
	GfxMouse.o:	GfxMouse.cpp	IGfxDevice.h

all: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(FLAGS) $(LIBS) -o $(TARGET)
	$(TARGET)

clean:
	rm -f $(OBJ_FILES)
	rm -f $(TARGET)

#run:
#	$(TARGET)