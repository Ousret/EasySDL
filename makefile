CC  = gcc -Wall
BIN = libESDL.a
INC = includes/
CFLAGS =`sdl-config --libs --cflags` -lSDL_image -lSDL_ttf -lSDL_mixer #-lGL -lGLU
CSDL = `sdl-config --cflags`

ESDL.o: ESDL.c $(INC)ESDL.h
	$(CC) -c ESDL.c $(CSDL)

install:
	@ar -r $(BIN) ESDL.o
	@cp $(BIN) /opt/local/lib/$(BIN)
	@cp $(INC)ESDL.h /opt/local/include/SDL/ESDL.h
	@rm -f *.o
