CC  = gcc -Wall
BIN = libESDL.a
INC = includes/
CFLAGS =`sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -lSDL2_mixer #-lGL -lGLU
CSDL = `sdl2-config --cflags`

ESDL.o: ESDL.c $(INC)ESDL.h
	$(CC) -c ESDL.c $(CSDL)

install:
	@ar -r $(BIN) ESDL.o
	@echo Welcome to ESDL installer
	@echo cp libESDL.a to /opt/local/lib
	@cp $(BIN) /opt/local/lib/$(BIN)
	@echo cp ESDL.h to /opt/local/include/SDL
	@cp $(INC)ESDL.h /opt/local/include/SDL/ESDL.h
	@echo Removing *.o 
	@rm -f *.o
	@echo Enjoy libESDL with -lESDL
	@echo TAHRI Ahmed
	
uninstall:
	@echo libESDL being deleted..!
	@rm /opt/local/lib/$(BIN)
	@rm /opt/local/include/SDL/ESDL.h
	@echo libESDL should be deleted now..
