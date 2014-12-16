UNAME := $(shell gcc -dumpmachine)
CC  = gcc -Wall
BIN = libESDL.a
INC = includes/
CFLAGS =`sdl-config --libs --cflags` -lSDL_image -lSDL_ttf -lSDL_mixer #-lGL -lGLU
CSDL = `sdl-config --cflags`

ESDL.o: ESDL.c $(INC)ESDL.h
	$(CC) -c ESDL.c $(CSDL)
	ar -r $(BIN) ESDL.o
	@echo Your OS is $(UNAME)

clean:
	@echo Cleanup EasySDL
	@rm -f *.o

install:
	@cp $(BIN) /opt/local/lib/$(BIN)
	@cp $(INC)ESDL.h /opt/local/include/SDL/ESDL.h

uninstall:
	@rm /opt/local/lib/$(BIN)
	@rm /opt/local/include/SDL/ESDL.h
