########################################
#  __  __       _         __ _ _      
# |  \/  |     | |       / _(_) |     
# | \  / | __ _| | _____| |_ _| | ___ 
# | |\/| |/ _` | |/ / _ \  _| | |/ _ \
# | |  | | (_| |   <  __/ | | | |  __/
# |_|  |_|\__,_|_|\_\___|_| |_|_|\___|
#
# Remmber to use MAKE with multiple cores:
#
#     make -j 16          # i.e. make best use of 16 cores when running make -j = 'jobs'
#
# Special thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)


TARGET_EXEC := main

# use 'make release=1' for release build
#  or 'make' for default
release ?= 1  # debug mode enabled by default
ifeq ($(release), 1)
    CFLAGS = -O3 -DRELEASE          # for RELEASE mode
else
    CFLAGS = -g -DDEBUG             # for DEBUG mode
endif

LIBS :=  -lpthread -lSDL2main -lSDL2 -ldl
COMPILER_FLAGS = -std=c++23 -fexceptions -Wall 
BUILD_DIR := ./build
SRC_DIRS := ./src
INC_DIRS := ./include

# Every folder in ./src will need to be passed to GCC so that it can find header files
# INC_DIRS := $(shell find $(SRC_DIRS) -type d)

#######################################################################################
# 
# Find all the C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these
#    otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP  $(CFLAGS) 
LDFLAGS := $(LIBS) $(COMPILER_FLAGS)


# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@
    
.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
