ESDL Software rendering 0.4.9
===========================

SDL lib, EasySDL permit to create program with SDL 1.2 easily.
ESDL version 0.4.x by TAHRI Ahmed, SIMON Jérémy
Low CPU usage !

What new in 0.4.9 ?
- We have changed 'SDL_mixer' by 'fmodex' who is more reliable.
- SDL_playwav become SDL_playSound AND SDL_preloadwav become SDL_loadSound.
- WAV is no longer the only format supported. WAV, OGG, MP3 are supported.
- The next release could be the last to support SDL 1.2.x

-------------------------------------
Make & Install
-------------------------------------

for both Linux and Unix kernel
make

Install script for Unix ONLY
sudo make install

What is needed for linking:
-lSDL -lSDL_image -lSDL_ttf -lESDL -lfmodex
