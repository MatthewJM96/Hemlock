CC      = g++

STD     = c++1z
INCLUDE = -I/usr/local/include -I../Hemlock/include -I../deps/include
OPT     = -Og
CFLAGS  = -Wall -g -ggdb $(OPT) -std=$(STD) $(INCLUDE)

LDIR    = -L../deps/lib/ -L/usr/lib -L/usr/local/lib
LIBS    = -l:libHemlock.a -lIL -lILUT -lOpenGL -lSDL2 -lSDL2main -lGLEW -lGLU #-l:liblua.a -ldl
LFLAGS  = $(LDIR) $(LIBS)

SRCS    = Game.cpp App.cpp MainMenuScreen.cpp VoxelTestScreen.cpp ChunkGenerator.cpp

OBJDIR  = obj/
OBJS    = $(SRCS:.cpp=.o)

EXE     = Game_$(CC)_$(STD).exe
RED_EXE = Game

.PHONY: clean

all:    $(EXE)
		@cp $(EXE) $(RED_EXE)
		@echo Compiled!

$(EXE): $(OBJS)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJS) $(LFLAGS)

.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
		@$(RM) *.o
		@echo Cleaned!
