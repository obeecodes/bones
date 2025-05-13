cmake_minimum_required(VERSION 3.24.4)
project(Bones)

execute_process(
    COMMAND xcrun --sdk macosx --show-sdk-path
    OUTPUT_VARIABLE OSX_SDK_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(CMAKE_OSX_SYSROOT ${OSX_SDK_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "Suppresses developer warnings.")

add_subdirectory(external/lua)

file(GLOB SOURCES src/*.cpp src/*/*.cpp src/*/*/*.cpp include/*.h include/*/*.h include/*/*/*.h)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# raylib headers and lib
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC /opt/homebrew/include)
link_directories(/opt/homebrew/lib)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    lua
    /opt/homebrew/lib/libraylib.a
    m
    "-framework CoreVideo"
    "-framework IOKit"
    "-framework Cocoa"
    "-framework GLUT"
    "-framework OpenGL"
)
