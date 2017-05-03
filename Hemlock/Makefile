CC      = g++
AR      = ar
ARFLAGS = rcs

STD     = c++1z
INCLUDE = -Iinclude/ -Iinclude/app/ -Iinclude/camera/ -Iinclude/graphics/ -Iinclude/io/ -Iinclude/script/ -Iinclude/maths/ -Iinclude/procedural/ -Iinclude/ui/ -Iinclude/voxel/  -I../deps/include/
OPT     = -Og
CFLAGS  = -Wall -g -ggdb $(OPT) -std=$(STD) $(INCLUDE)

#LDIR    = -L../deps/libs/
#LIBS    = #-l:liblua.a -ldl
#LFLAGS  = $(LDIR) $(LIBS)

SRCS_GLO = src/stdafx.cpp src/Timing.cpp    
SRCS_APP = src/app/IApp.cpp src/app/IScreen.cpp
SRCS_CAM = src/camera/BasicCamera.cpp src/camera/BasicFPSCamera.cpp
SRCS_GFX = src/graphics/GLSLProgram.cpp src/graphics/Texture.cpp src/graphics/Window.cpp
SRCS_IO  = src/io/FileLoader.cpp src/io/ImageLoader.cpp
SRCS_UI  = src/ui/InputDispatcher.cpp src/ui/InputManager.cpp
SRCS_VOX = src/voxel/Chunk.cpp src/voxel/ChunkCoordSystems.cpp src/voxel/ChunkGrid.cpp src/voxel/ChunkMesher.cpp
SRCS     = $(SRCS_GLO) $(SRCS_APP) $(SRCS_CAM) $(SRCS_GFX) $(SRCS_IO) $(SRCS_UI) $(SRCS_VOX)

OBJDIR  = obj/
OBJS    = $(SRCS:.cpp=.o)


LIB     = libHemlock_$(CC)_$(STD).a
RED_LIB = libHemlock.a

.PHONY: clean

all:    $(LIB)
		@cp $(LIB) ../deps/lib/$(RED_LIB)
		@echo Compiled!

$(LIB): $(OBJS)
		$(AR) $(ARFLAGS) $(LIB) -o $(OBJS)
#		$(CC) $(CFLAGS) -o $(EXE) $(OBJS) $(LFLAGS)

.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
		@$(RM) src/*.o src/app/*.o src/camera/*.o src/graphics/*.o src/io/*.o src/script/*.o src/ui/*.o src/voxel/*.o
		@echo Cleaned!
