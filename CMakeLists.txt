cmake_minimum_required(VERSION 3.1)

project(CStreamIO)

enable_testing()

set(CMAKE_CXX_STANDARD 11)
include_directories(src)
include_directories(include)

add_library(cstreamio src/GBK.cpp src/gbk_coding_data.cpp src/UTF8.cpp)
add_subdirectory(unittests)

install(TARGETS cstreamio DESTINATION lib)
install(FILES include/ASCII.h include/BufferTooSmallException.h include/GBK.h include/MemoryInputStream.h include/TextReader.h include/TextWriter.h include/UTF8.h DESTINATION include) 
