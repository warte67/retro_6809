# Makefile Retro_6809
CC = g++
TARGET = ./bin/main
FLAGS = -std=c++20 -O2 #-g -V
LIBS = -lSDL2 -ldl
OBJ = obj
OBJ_FILES = Bus.o \
			Device.o \
			Gfx.o \
			C6809.o \
			GfxBitmap.o \
			GfxDebug.o \
			GfxExtended.o \
			GfxMouse.o \
			GfxSprite.o \
			GfxText.o \
			Keyboard.o \
			Gamepad.o \
			FileIO.o \
			Math.o \
			main.o

#IGfxDevice.o GfxBitmap.o GfxDebug.o GfxExtended.o GfxMouse.o GfxSprite.o GfxText.o

all: $(OBJ_FILES)	 
	Bus.o: 			Bus.cpp 		Bus.h
	main.o: 		main.cpp
	Device.o: 		Device.cpp 		Device.h
	Gfx.o: 			Gfx.cpp 		Gfx.h
	C6809.o:		C6809.cpp		C6809.h
	FileIO.o:		FileIO.cpp		FileIO.h
	Math.o:			Math.cpp		Math.h
	Keyboard.o:		Keyboard.cpp	Keyboard.h
	Gamepad.o:		Gamepad.cpp		Gamepad.h
	GfxBitmap.o: 	GfxBitmap.cpp 	GfxBitmap.h
	GfxDebug.o:		GfxDebug.cpp	IGfxDevice.h
	GfxExtended.o:	GfxExtended.cpp	GfxExtended.h
	GfxMouse.o:		GfxMouse.cpp	IGfxDevice.h
	GfxSprite.o:	GfxSprite.cpp	GfxSprite.h
	GfxText.o:		GfxText.cpp		GfxText.h	

all: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(FLAGS) $(LIBS) -o $(TARGET)
	$(TARGET)

clean:
	rm -f $(OBJ_FILES)
	rm -f $(TARGET)

#run:
#	$(TARGET)