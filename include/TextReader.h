#ifndef _TEXT_READER_H
#define _TEXT_READER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include "ByteBuffer.h"

using std::istream;

typedef void (*charset_decoder) (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded
  );

class TextReader {
private:
  const static int DEFAULT_BUFFER_SIZE = 1024;

  // byte order mark
  const static int UTF8_BOM = 0xEFBBBF;

  istream *in;

  charset_decoder decoder;

  ByteBuffer bb;

public:
  TextReader(istream *inputStream, charset_decoder decoder): TextReader(inputStream, decoder, DEFAULT_BUFFER_SIZE, true) {
  }

  TextReader(istream *inputStream, charset_decoder decoder, int bufferSize): TextReader(inputStream, decoder, bufferSize, true) {
  }

  TextReader(istream *inputStream, charset_decoder decoder, bool skipUtf8Bom) :
              TextReader(inputStream, decoder, DEFAULT_BUFFER_SIZE, skipUtf8Bom) {
  }

  TextReader(istream *inputStream, charset_decoder decoder, int bufferSize, bool skipUtf8Bom);

  ~TextReader() {
  }

private:
  inline int doRead();

public:
  int read(char32_t* dest_buf, unsigned int offset, unsigned int count);
};

#endif
