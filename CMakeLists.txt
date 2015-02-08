cmake_minimum_required(VERSION 2.6)

project(libESDL)

option(LIUMBuild "LIUMBuild" OFF)

set(LIBRARY_OUTPUT_PATH bin)
set(CMAKE_MODULE_PATH "cmake_modules")
add_definitions(-Wall -Wextra)

include_directories(include)
if (LIUMBuild)
	message("EasySDL: LIUM include directories added !")
	include_directories(/usr/local/include) #LIUM Fix
	include_directories(/opt/local/include)
endif()

set(sqlite3_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(sqlite3)

if (NOT SQLITE3_FOUND)
	MESSAGE("EasySDL: SQLITE NOT FOUND, WILL USE INTEGRATED PACKAGE..")
	ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/deps/sqlite3)
endif()
	
set(SDL_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL)
		
set(SDL_ttfDIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL_ttf)
		
set(SDL_imageDIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL_image)
		
#find_package(crypto)

if (SDL_FOUND)
	include_directories(SDL_INCLUDE_DIR)
	include_directories(SDLIMAGE_INCLUDE_DIR)
	include_directories(SDLTTF_INCLUDE_DIR)
	message("EasySDL            : yes")
else (SDL_FOUND)
	message("EasySDL            : no (install libSDL)")
endif (SDL_FOUND)

###### FrameWork ######
#Will look into it later..

file(
        GLOB_RECURSE
        source_files
        src/*.c
)

add_library(
        ESDL
        SHARED
        ${source_files}
)

target_link_libraries(

        ESDL
        ${SDL_LIBRARY}
        ${SDLTTF_LIBRARY}
        ${SDLIMAGE_LIBRARY}
        ${SQLITE3_LIBRARIES}
        fmodex
        crypto

)

INSTALL_TARGETS(/lib ESDL)
INSTALL(FILES include/ESDL.h 
  DESTINATION include)
