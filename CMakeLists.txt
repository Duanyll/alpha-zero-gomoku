cmake_minimum_required(VERSION 3.14.0)
project(libzerogomoku)

# option
option(WRAP_LIB "wrap library" ON)
option(UNIT_TEST "unit test" OFF)

set(CMAKE_CXX_STANDARD 14)

if (NOT TORCH_CMAKE_PATH)
    # run python and read pytorch cmake path
    execute_process(COMMAND python -c "import torch; print(torch.utils.cmake_prefix_path)" OUTPUT_VARIABLE TORCH_CMAKE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
    # cache TORCH_CMAKE_PATH
    set(TORCH_CMAKE_PATH ${TORCH_CMAKE_PATH} CACHE PATH "TORCH_CMAKE_PATH")
endif()
message(STATUS "TORCH_CMAKE_PATH: ${TORCH_CMAKE_PATH}")
set(CMAKE_PREFIX_PATH ${TORCH_CMAKE_PATH})
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
# suppress warning C4624 when using msvc
if(MSVC)
    add_compile_options(/wd4624)
endif()

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
    set_property(SOURCE ./src/libzerogomoku.i PROPERTY CPLUSPLUS ON)
    swig_add_library(libzerogomoku TYPE SHARED LANGUAGE python SOURCES ./src/libzerogomoku.i ${SOURCES})
    swig_link_libraries(libzerogomoku ${Python_LIBRARIES} ${TORCH_LIBRARIES})

    # The following code block is suggested to be used on Windows.
    # According to https://github.com/pytorch/pytorch/issues/25457,
    # the DLLs need to be copied to avoid memory errors.
    # if (WIN32)
    #     file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    #     add_custom_command(TARGET _libzerogomoku
    #                        POST_BUILD
    #                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #                        ${TORCH_DLLS}
    #                        $<TARGET_FILE_DIR:_libzerogomoku>)
    # endif (WIN32)
endif()

# unit test
if(UNIT_TEST)
    add_library(test_lib ${SOURCES})
    target_link_libraries(test_lib ${TORCH_LIBRARIES})

    add_executable(thread_pool_test ./test/thread_pool_test.cpp)
    target_link_libraries(thread_pool_test test_lib)

    add_executable(gomoku_test ./test/gomoku_test.cpp)
    target_link_libraries(gomoku_test test_lib)

    add_executable(libtorch_test ./test/neural_network_test.cpp)
    target_link_libraries(libtorch_test test_lib)

    add_executable(mcts_test ./test/mcts_test.cpp)
    target_link_libraries(mcts_test test_lib)
endif()
