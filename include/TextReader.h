#ifndef _TEXT_READER_H
#define _TEXT_READER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include "Charsets.h"

using std::istream;

class TextReader {
private:
  const static int DEFAULT_BUFFER_SIZE = 1024;

  istream *in;

  charset_decoder decoder;

  ByteBuffer bb;

public:
  TextReader(istream *inputStream, string charsetName): TextReader(inputStream, charsetName, DEFAULT_BUFFER_SIZE) {

  }
  
  TextReader(istream *inputStream, string charsetName, int bufferSize);

  ~TextReader() {
  }

private:
  inline int doRead();

public:
  int read(char32_t* dest_buf, unsigned int offset, unsigned int count);
};

#endif
