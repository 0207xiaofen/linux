cmake_minimum_required(VERSION 3.22)
project(myStl)
set(CMAEK_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

#[[set(SRC_ARR test_array.cpp)
add_executable(array ${SRC_ARR})

set(SRC_VEC test_vector.cpp)
add_executable(vector ${SRC_VEC})

set(SRC_UNI test_unique_ptr.cpp)
add_executable(unique ${SRC_UNI})]]

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC_test /home/wangxian/桌面/up/mySTL/test)
set(SRC_src /home/wangxian/桌面/up/mySTL/src)
#set(SRC_algo /home/wangxian/桌面/up/mySTL/src/algorithm.cpp)
set(SRC_include /home/wangxian/桌面/up/mySTL/include)
set(SRC_alloc /home/wangxian/桌面/up/mySTL/src/alloc.cpp)

aux_source_directory(${SRC_test} test)
aux_source_directory(${SRC_src} src)
aux_source_directory(${SRC_include} include)

add_executable(array ${SRC_test}/test_array.cpp)
add_executable(unique ${SRC_test}/test_unique_ptr.cpp)
add_executable(vector ${SRC_test}/test_vector.cpp ${SRC_alloc})
add_executable(algo ${SRC_test}/test_algo.cpp  ${SRC_alloc})
add_executable(list ${SRC_test}/test_list.cpp ${SRC_alloc})

set(LIBRARY_OUTPUT_PATH /home/wangxian/桌面/up/mySTL/lib)
add_library(mystl SHARED ${src})

set(PATH /home/wangxian/桌面/up/mySTL/TEST)
set(EXECUTABLE_OUTPUT_PATH ${PATH})

set_target_properties(array unique vector algo PROPERTIES LINKER_LANGUAGE CXX)
