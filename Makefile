# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++23 -Iinclude -fexceptions -Wall 
LDFLAGS := -lncurses -lSDL2 -lSDLmain -ldl -lpthread

# Project name
TARGET := build/main

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build

# Build configuration (default to debug)
BUILD := debug
DEBUG_FLAGS := -g -DDEBUG
RELEASE_FLAGS := -O2 -DNDEBUG

# Source files and object files
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))

# Dependency files
DEPS := $(OBJS:.o=.d)

# Select flags based on build type
ifeq ($(BUILD), debug)
    CXXFLAGS += $(DEBUG_FLAGS)
else ifeq ($(BUILD), release)
    CXXFLAGS += $(RELEASE_FLAGS)
endif

# Build the target executable
$(TARGET): $(OBJS) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	# Generate dependency file for each object file
	$(CXX) -MM $(CXXFLAGS) $< > $(BUILDDIR)/$*.d

# Create the build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

# Include the dependency files (if they exist)
-include $(DEPS)

# Set build types
.PHONY: debug release
debug:
	$(MAKE) BUILD=debug

release:
	$(MAKE) BUILD=release

# Run the program
.PHONY: run
run: $(TARGET)
	./$(TARGET)
