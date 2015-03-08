EasySDL lib 0.7.1 [![Build Status](https://travis-ci.org/Ousret/EasySDL.svg)](https://travis-ci.org/Ousret/EasySDL)
===========================

SDL lib, EasySDL permit to create program / game with SDL 1.2 easily.
[Detailed example using EasySDL.](https://github.com/Ousret/EasySDL-Sample)

Binaries available at Sourceforge: http://sourceforge.net/projects/easysdl-sdk/
DevPak for DevCpp included.
If you wish to make target for arch AMD64 (FMODex), please run cmake with :

```
$ cmake -G"Unix Makefiles" -DForceAMD64=ON
```

Author(s): 
- TAHRI Ahmed @Ousret

What new in 0.7.1 ?
- Major improvements.
- Fix mem leaks when using aes save
- Fix unexpected crashs when using data save with NT Kernel (Win)

### EasySDL 0.7

CMake is needed.
Install included.

```
$ git clone https://github.com/Ousret/EasySDL/
$ cd EasySDL
$ cmake -G"Unix Makefiles"
$ make
$ sudo make install
```

```objc
#include <ESDL.h>
```

```
CFLAGS = `sdl-config --libs --cflags` -lESDL
```

Doc: http://spitajoke.com/tahri/EasySDL/doxygen/

--------------------------------
Licenses
--------------------------------
### libSDL, libSDL_ttf, libSDL_image

SDL & SDL_ttf & SDL_image are under zlib license.

### LibFMODex

fmodex license: http://www.fmod.com/files/public/LICENSE.TXT (NO COMMERCIAL USE)

“FMOD Sound System, copyright © Firelight Technologies Pty, Ltd., 1994-2014.”
“Audio engine : FMOD Sound System by Firelight Technologies”
“FMOD by Firelight Technologies”

### OpenSSL, libCrypto, libSSL

OpenSSL license: 
The OpenSSL toolkit stays under a dual license, i.e. both the conditions of
the OpenSSL License and the original SSLeay license apply to the toolkit.
See below for the actual license texts. Actually both licenses are BSD-style
Open Source licenses. In case of any license issues related to OpenSSL
please contact openssl-core@openssl.org.

### libEasySDL

EasySDL is under license GNU GPL v2.0
