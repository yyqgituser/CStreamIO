#include "TextWriter.h"

void TextWriter::write(const char32_t* char_buf, unsigned int offset, unsigned int count) {
  unsigned int nencoded;
  if (bb.hasFreeSpace()) {
    encoder(char_buf, offset, count, &bb, &nencoded);
    offset += nencoded;
    count -= nencoded;
  }
  while(count > 0) {
    out->write(bb.buffer + bb.begin, bb.remaining());
    bb.clear();
    encoder(char_buf, offset, count, &bb, &nencoded);
    offset += nencoded;
    count -= nencoded;
  }
}

void TextWriter::flush() {
  if(bb.hasRemaining()) {
    out->write(bb.buffer + bb.begin, bb.remaining());
    out->flush();
    bb.clear();
  }
}