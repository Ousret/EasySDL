UNAME := $(shell gcc -dumpmachine)
CC  = gcc -Wall -Wextra
BIN = libESDL.a
INC = include/
CSDL = `sdl-config --cflags`

ESDL.o: ESDL.c $(INC)ESDL.h
	$(CC) -c ESDL.c $(CSDL)
	ar -r $(BIN) ESDL.o
	@echo Your OS is $(UNAME)

clean:
	@rm -f *.o

install:
	@cp $(BIN) /opt/local/lib/$(BIN)
	@cp $(INC)ESDL.h /opt/local/include/SDL/ESDL.h

uninstall:
	@rm /opt/local/lib/$(BIN)
	@rm /opt/local/include/SDL/ESDL.h
