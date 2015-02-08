PROJECT(sqlite3)
cmake_minimum_required(VERSION 2.8)

set(LIBRARY_OUTPUT_PATH bin)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_library(
	sqlite3 
	STATIC 
	src/sqlite3.c 
	include/sqlite3.h 
	include/sqlite3ext.h
)

add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DSQLITE_ENABLE_FTS4)

install(
	FILES 
	include/sqlite3.h 
	include/sqlite3ext.h 
	DESTINATION include
)

install(
	TARGETS sqlite3 
	LIBRARY DESTINATION lib 
	ARCHIVE DESTINATION lib
)