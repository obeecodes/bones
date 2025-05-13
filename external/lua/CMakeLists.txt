cmake_minimum_required(VERSION 3.10)
project(lua)

# Add Lua source files
file(GLOB LUA_SOURCES src/*.c)

# Create a static library
add_library(lua STATIC ${LUA_SOURCES})

# Include directories
target_include_directories(lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

