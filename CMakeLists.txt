cmake_minimum_required(VERSION 2.6)

project(libESDL)
set(LIBRARY_OUTPUT_PATH bin)

include_directories(include)
if(UNIX)
    include_directories(/opt/local/include/SDL)
    link_directories(/opt/local/lib)
    #set(CMAKE_C_FLAGS "-lSDLmain -Wl,-framework,AppKit -lSDL -Wl,-framework,Cocoa")
endif()

file(
        GLOB_RECURSE
        source_files
        src/*
)

add_library(
        ESDL
        SHARED
        ${source_files}
)

target_link_libraries(

        ESDL
        SDLmain
        SDL
        SDL_ttf
        SDL_image
        fmodex
        crypto
        sqlite3

)

INSTALL_TARGETS(/lib ESDL)
INSTALL(FILES include/ESDL.h 
  DESTINATION include)
