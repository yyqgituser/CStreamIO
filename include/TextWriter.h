#ifndef _TEXT_WRITER_H
#define _TEXT_WRITER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "Charsets.h"

using std::ostream;

class TextWriter {
private:
  const static int DEFAULT_BUFFER_SIZE = 4 * 1024;

  ostream *out;

  ByteBuffer bb;

  charset_encoder encoder;

public:
  TextWriter(ostream *outputStream, string charsetName): TextWriter(outputStream, charsetName, DEFAULT_BUFFER_SIZE) {
  }

  TextWriter(ostream *outputStream, string charsetName, int bufferSize);

  ~TextWriter() {
    flush();
  }

public:  
  void write(const char32_t* char_buf, unsigned int offset, unsigned int count);

  void flush();
};

#endif
