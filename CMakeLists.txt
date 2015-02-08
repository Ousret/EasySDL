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
endif()

#find fmodex (manual + print licence)
message("This lib use FMODex, licence at: http://www.fmod.com/files/public/LICENSE.TXT (NO COMMERCIAL USE)")
message("FMOD Sound System, copyright © Firelight Technologies Pty, Ltd., 1994-2014.")
message("Audio engine : FMOD Sound System by Firelight Technologies")
message("FMOD by Firelight Technologies")

message("Your kernel is: ${CMAKE_SYSTEM_NAME}")

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	message("Linux FMODex added")
	set(FMODEXLib "${CMAKE_SOURCE_DIR}/deps/fmodex/linux/libfmodex64.so")
endif()

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
	message("Unix FMODex added")
	set(FMODEXLib "${CMAKE_SOURCE_DIR}/deps/fmodex/unix/libfmodex.dylib")
endif()

#Todo..
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Cygwin")
	message("Unix FMODex added")
	#set(FMODEXLib "${CMAKE_SOURCE_DIR}/deps/fmodex/unix/libfmodex.dylib")
endif()

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

if (NOT OPENSSL_FOUND)
	add_dependencies(ESDL crypto)
endif()

if (NOT SQLITE3_FOUND)
	add_dependencies(ESDL sqlite3)
endif()

target_link_libraries(

        ESDL
        ${SDL_LIBRARY}
        ${SDLTTF_LIBRARY}
        ${SDLIMAGE_LIBRARY}
        ${SQLITE3_LIBRARIES}
        ${FMODEXLib}
        ${OPENSSL_LIBRARIES}

)

INSTALL_TARGETS(/lib ESDL)
INSTALL(FILES include/ESDL.h 
  DESTINATION include)
