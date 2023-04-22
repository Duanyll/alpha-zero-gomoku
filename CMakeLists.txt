cmake_minimum_required(VERSION 3.8.0)
project(library)

# option
option(WRAP_LIB "wrap library" ON)
option(UNIT_TEST "unit test" OFF)

set(CMAKE_CXX_STANDARD 14)

# run python and read pytorch cmake path
execute_process(COMMAND python -c "import torch; print(torch.utils.cmake_prefix_path)" OUTPUT_VARIABLE TORCH_CMAKE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "TORCH_CMAKE_PATH: ${TORCH_CMAKE_PATH}")
set(CMAKE_PREFIX_PATH ${TORCH_CMAKE_PATH})
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

# find torch
find_package(Torch REQUIRED)

# find swig
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# find python
find_package(Python 3.6 REQUIRED)
# run python and read python include path
execute_process(COMMAND python -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())" OUTPUT_VARIABLE Python_INCLUDE_DIRS OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Python_INCLUDE_DIRS: ${Python_INCLUDE_DIRS}")
include_directories(${Python_INCLUDE_DIRS})

# if PYTHON_LIBRARIES is defined, use it
if(PYTHON_LIBRARIES)
    set(Python_LIBRARIES ${PYTHON_LIBRARIES})
endif()

# if Python_LIBRARIES is not found, warn user
if(NOT Python_LIBRARIES)
    message(WARNING "Python_LIBRARIES not found, please set it manually")
endif()

message(STATUS "Python_LIBRARIES: ${Python_LIBRARIES}")

# add sources
include_directories(./src)
aux_source_directory(./src SOURCES)

# swig
if(WRAP_LIB)
    set_property(SOURCE ./src/library.i PROPERTY CPLUSPLUS ON)
    swig_add_library(library TYPE SHARED LANGUAGE python SOURCES ./src/library.i ${SOURCES})
    swig_link_libraries(library ${Python_LIBRARIES} ${TORCH_LIBRARIES})
endif()

# unit test
if(UNIT_TEST)
    add_library(test_lib ${SOURCES})
    target_link_libraries(test_lib ${TORCH_LIBRARIES})

    add_executable(thread_pool_test ./test/thread_pool_test.cpp)
    target_link_libraries(thread_pool_test test_lib)

    add_executable(gomoku_test ./test/gomoku_test.cpp)
    target_link_libraries(gomoku_test test_lib)

    add_executable(libtorch_test ./test/libtorch_test.cpp)
    target_link_libraries(libtorch_test test_lib)

    add_executable(mcts_test ./test/mcts_test.cpp)
    target_link_libraries(mcts_test test_lib)
endif()
