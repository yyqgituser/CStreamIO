#include "TextWriter.h"

TextWriter::TextWriter(ostream *outputStream, string charsetName, int bufferSize) :
    out(outputStream), bb(bufferSize), encoder(0) {
  std::map<string, Charset>::iterator itr = KnownCharsets::charsetTable.find(charsetName);
  if (itr == KnownCharsets::charsetTable.end()) {
    throw std::runtime_error("unkown charset: " + charsetName);
  }
  if (bufferSize < 8) {
    throw std::runtime_error("buffer size too small: " + std::to_string(bufferSize));
  }
  encoder = itr->second.encoder;
}

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