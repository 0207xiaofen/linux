cmake_minimum_required(VERSION 3.22)
project(sylar)

set(CMAKE_VERSION_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamix -o3 -g -std-c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)

set(LIB_SRC
    sylar/log.cpp
    sylar/util.cpp
    sylar/config.cpp
    )

add_library(sylar SHARED ${LIB_SRC})
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (sylar_static PROPERTIES OUTPUT_NAME "sylar")

add_executable(test tests/test.cpp)
add_dependencies(test sylar)
target_link_libraries(test sylar)

add_executable(test_config tests/test_config.cpp)
add_dependencies(test_config sylar)
target_link_libraries(test_config sylar)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT-SOURCE_DIR}/lib)
