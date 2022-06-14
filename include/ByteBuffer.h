#ifndef _BYTE_BUFFER_H
#define _BYTE_BUFFER_H

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

class ByteBuffer {
public:
  // beginning of data(inclusive)
  unsigned int begin;

  // end of data(exclusive)
  unsigned int end;

  // buffer size
  unsigned int capacity;

  // buffer
  char* buffer;

public:
  ByteBuffer(unsigned int capacity) :
    begin(0), end(0), capacity(capacity) {    
    buffer = new char[capacity];
  }

  ~ByteBuffer() {
    if(buffer != (char *)0) {
      delete[] buffer;
    }
  }

  inline char get() {
    if (begin > end) {
      throw std::runtime_error("index out of bound: " + std::to_string(begin));
    }
    return buffer[begin++];
  }

  inline void skip(unsigned int length) {
    if (begin + length > end) {
        throw std::runtime_error("index out of bound: " + std::to_string(end));
    }
    begin += length;
  }
  
  inline void append(unsigned int length) {
	  if (end + length > capacity) {
		  throw std::runtime_error("index out of bound: " + std::to_string(capacity));
	  }
	  end += length;
  }
  
  inline bool hasRemaining() {
    return begin < end;
  }
  
  inline bool hasFreeSpace() {
    return end < capacity;
  }
  
  inline unsigned int remaining() {
    return end - begin;
  }

  inline unsigned int freeSpace() {
    return capacity - end;
  }

  inline void compact() {
    unsigned int i, j;
    if (begin == 0) {
      return;
    }
    for(i = begin, j = 0; i < end; i++) {
      buffer[j++] = buffer[i];
    }
    begin = 0; end = j;
  }

  inline void clear() {
    begin = end = 0;
  }

};


#endif
