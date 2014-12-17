CC  = gcc -Wall
BIN = libESDL.a
INC = includes/
CFLAGS =`sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -lSDL2_mixer #-lGL -lGLU
CSDL = `sdl2-config --cflags`

ESDL.o: ESDL.c $(INC)ESDL.h
	$(CC) -c ESDL.c $(CSDL)
	ar -r $(BIN) ESDL.o

install:
	@cp $(BIN) /opt/local/lib/$(BIN)
	@cp $(INC)ESDL.h /opt/local/include/SDL/ESDL.h
	
uninstall:
	@rm /opt/local/lib/$(BIN)
	@rm /opt/local/include/SDL/ESDL.h
