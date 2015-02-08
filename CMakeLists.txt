cmake_minimum_required(VERSION 2.6)

project(libESDL)

set(LIBRARY_OUTPUT_PATH bin)
set(CMAKE_MODULE_PATH "cmake_modules")
add_definitions(-Wall -Wextra)

include_directories(include)

set(sqlite3_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(sqlite3)

if (NOT SQLITE3_FOUND)
	MESSAGE("EasySDL: SQLITE NOT FOUND, WILL USE INTEGRATED PACKAGE..")
	ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/deps/sqlite3)
	add_dependencies(ESDL sqlite3)
endif()
	
set(SDL_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL REQUIRED)

if (SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIR})
	message("EasySDL: SDL Header found !")
endif()

set(SDL_ttfDIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL_ttf REQUIRED)

if (SDLTTF_FOUND)
	include_directories(${SDLTTF_INCLUDE_DIR})
	message(${SDLTTF_INCLUDE_DIR})
else (SDLTTF_FOUND)
	message("EasySDL: SDL_ttf header is not found !")
endif(SDLTTF_FOUND)

set(SDL_imageDIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL_image REQUIRED)

if (SDLIMAGE_FOUND)
	include_directories(${SDLIMAGE_INCLUDE_DIR})
	message(${SDLIMAGE_INCLUDE_DIR})
endif()
	
find_package(openssl)

if (NOT OPENSSL_FOUND)
	message("EasySDL: OpenSSL not found, will use internal package.")
	ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/deps/openssl)
	add_dependencies(ESDL crypto)
endif()

#find fmodex (manual + print licence)

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
        ${OPENSSL_LIBRARIES}

)

INSTALL_TARGETS(/lib ESDL)
INSTALL(FILES include/ESDL.h 
  DESTINATION include)
