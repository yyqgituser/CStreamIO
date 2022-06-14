#ifndef _TEXT_WRITER_H
#define _TEXT_WRITER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ByteBuffer.h"

using std::ostream;

typedef void (*charset_encoder) (
  const char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded);

class TextWriter {
private:
  const static int DEFAULT_BUFFER_SIZE = 4 * 1024;

  ostream *out;

  ByteBuffer bb;

  charset_encoder encoder;

public:
  TextWriter(ostream *outputStream, charset_encoder encoder): TextWriter(outputStream, encoder, DEFAULT_BUFFER_SIZE) {
  }

  TextWriter(ostream *outputStream, charset_encoder encoder, int bufferSize) :
    out(outputStream), bb(bufferSize), encoder(encoder) {
    if (bufferSize < 8) {
      throw std::runtime_error("buffer size too small: " + std::to_string(bufferSize));
    }
  }

  ~TextWriter() {
    flush();
  }

public:  
  void write(const char32_t* char_buf, unsigned int offset, unsigned int count);

  void flush();
};

#endif
