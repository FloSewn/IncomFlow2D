#***********************************************************
#
#***********************************************************
cmake_minimum_required(VERSION 3.10)
project(IncomFlow2D
        LANGUAGES C
        VERSION   1.0)

# Set C++ flags
#set(CMAKE_C_STANDARD C99)
#set(CMAKE_C_STANDARD_REQUIRED ON)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-variable -std=c99 -O3 -DNDEBUG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-variable -std=c99 -O0 -pg -g")


# Set directories
set( BIN ${CMAKE_SOURCE_DIR}/bin )
set( SRC ${CMAKE_SOURCE_DIR}/src )
set( LIBS ${CMAKE_SOURCE_DIR}/libs )

add_subdirectory( src/utils )
#add_subdirectory( src/solver )
add_subdirectory( src/tests )
