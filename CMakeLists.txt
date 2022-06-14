cmake_minimum_required(VERSION 3.1)

project(CStreamIO)

enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(src)
include_directories(include)

add_library(cstreamio src/GBK.cpp src/gbk_coding_data.cpp src/UTF8.cpp src/ASCII.cpp src/TextReader.cpp src/TextWriter.cpp)
add_subdirectory(unittests)

install(TARGETS cstreamio DESTINATION lib)
install(FILES include/ByteBuffer.h include/ASCII.h include/GBK.h include/MemoryInputStream.h include/TextReader.h include/TextWriter.h include/UTF8.h DESTINATION include) 
