set (CMAKE_POSITION_INDEPENDENT_CODE TRUE)
cmake_minimum_required(VERSION 2.6)
project(libESDL)

option(ForceAMD64 "ForceAMD64" OFF)

#Use defined FMODex header
file(
        GLOB_RECURSE
        fmodex_header
        include/fmodex/*.h
)

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

set(LIBRARY_OUTPUT_PATH bin)
set(CMAKE_MODULE_PATH "cmake_modules")
add_definitions(-Wall -Wextra)

include_directories(include)

#Find local SQLite 3
set(sqlite3_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(sqlite3)

if (NOT SQLITE3_FOUND)
	MESSAGE("EasySDL: SQLite3 is not installed, will use integrated package !")
	ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/deps/sqlite3)
	include_directories(${CMAKE_SOURCE_DIR}/deps/sqlite3/include)
	add_dependencies(ESDL sqlite3)
	target_link_libraries(ESDL sqlite3)
else()
	target_link_libraries(ESDL ${SQLITE3_LIBRARIES})
endif()

#Find SDL base package
set(SDL_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL REQUIRED)

if (SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIR})
	message("EasySDL: SDL Header found : ${SDL_INCLUDE_DIR}")
endif()

#Find SDL_ttf module
set(SDL_ttfDIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL_ttf REQUIRED)

if (SDLTTF_FOUND)
	include_directories(${SDLTTF_INCLUDE_DIR})
	message(${SDLTTF_INCLUDE_DIR})
else (SDLTTF_FOUND)
	message("EasySDL: SDL_ttf header is not found !")
endif(SDLTTF_FOUND)

#Find SDL_image module
set(SDL_imageDIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL_image REQUIRED)

if (SDLIMAGE_FOUND)
	include_directories(${SDLIMAGE_INCLUDE_DIR})
	message(${SDLIMAGE_INCLUDE_DIR})
endif()

set(opensslDIR "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(openssl)

if (NOT OPENSSL_FOUND)
	message("EasySDL: OpenSSL not found, will use internal package.")
	ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/deps/openssl)
	include_directories(${CMAKE_SOURCE_DIR}/deps/openssl/include)
	add_dependencies(ESDL crypto)
	target_link_libraries(ESDL crypto)
else()
	target_link_libraries(ESDL ${OPENSSL_LIBRARIES})
endif()

#Find local FMODex lib
message("This lib use FMODex, licence at: http://www.fmod.com/files/public/LICENSE.TXT (NO COMMERCIAL USE)")
message("FMOD Sound System, copyright © Firelight Technologies Pty, Ltd., 1994-2014.")
message("Audio engine : FMOD Sound System by Firelight Technologies")
message("FMOD by Firelight Technologies")

EXEC_PROGRAM("uname -m" OUTPUT_VARIABLE UNAME_MACHINE)
MESSAGE(${UNAME_MACHINE})

if (ForceAMD64)
	set(FMODEXAMD_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
	find_package(FMODEXAMD REQUIRED)
else()
	set(FMODEX_DIR "${CMAKE_SOURCE_DIR}/cmake_modules")
	find_package(FMODEX REQUIRED)
endif()

if (NOT LIBFMODEX_FOUND)
	message(FATAL "EasySDL: FMODex lib cannot be found, re clone EasySDL may solve this !")
endif()

target_link_libraries(

        ESDL
        ${SDL_LIBRARY}
        ${SDLTTF_LIBRARY}
        ${SDLIMAGE_LIBRARY}
        ${LIBFMODEX_LIBRARIES}

)

INSTALL_TARGETS(/lib ESDL)
INSTALL(FILES include/ESDL.h
  DESTINATION include)

INSTALL(FILES ${LIBFMODEX_LIBRARIES} DESTINATION lib)
INSTALL(FILES ${fmodex_header}
  DESTINATION include/fmodex)
