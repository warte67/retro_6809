TARGET = ./bin/main
FLAGS = -std=c++20 #-g
LIBS = -lSDL2 
OBJ_FILES = Bus.o Device.o Gfx.o Memory.o Mouse.o main.o

all: $(OBJ_FILES)	

Bus.o: Bus.cpp
Device.o: Device.cpp
Gfx.o: Gfx.cpp
Memory.o: Memory.cpp
Mouse.o: Mouse.cpp
main.o: main.cpp

all: $(OBJ_FILES)
	g++ $(OBJ_FILES) $(FLAGS) $(LIBS) -o $(TARGET)
	#$(TARGET)

clean:
	rm -f $(OBJ_FILES)
	rm -f $(TARGET)
