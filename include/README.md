ESDL Software rendering
===========================

EasySDL 0.4.X
This librarie will permit you to create your game without handling SDL base code !
CPU cool usage, no excessive CPU load.

Author(s): SIMON JŽrŽmy, TAHRI Ahmed.

How to install EasySDL:

Open terminal and go to ESDL dir then:

`> git pull`
`> make`
`> sudo make install`

Then you are ready to use it, now include:

`#include <ESDL.h>`

And add to your makefile after -lSDL
`-lESDL`
