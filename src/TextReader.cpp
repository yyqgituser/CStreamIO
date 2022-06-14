#include "TextReader.h"

using namespace std;

TextReader::TextReader(istream *inputStream, charset_decoder decoder, int bufferSize, bool skipUtf8Bom) :
  in(inputStream), decoder(decoder), bb(bufferSize) {
  if (bufferSize < 8) {
    throw std::runtime_error("buffer size too small: " + std::to_string(bufferSize));
  }
  inputStream->exceptions(std::ios::badbit);
  if (skipUtf8Bom) {
    int nread = doRead();
    if (nread < 3) {
      char *buf = bb.buffer;
      int c = ((buf[0] & 0xff << 16) | (buf[1] & 0xff << 8) | (buf[2] & 0xff));
      if (c == UTF8_BOM) {
        bb.skip(3);
      }
    }
  }
}

int TextReader::doRead() {
  in->read(bb.buffer + bb.end, bb.freeSpace());
  int nread = in->gcount();
  if (nread > 0) {
    bb.append(nread);
    return nread;
  } else {
      return -1;
  }
}

int TextReader::read(char32_t* dest_buf, unsigned int offset, unsigned int count) {
  unsigned int ndecoded, total_ndecoded = 0;
  if(bb.hasRemaining()) {
    decoder(&bb, dest_buf, offset, count, &ndecoded);
    offset += ndecoded;
    count -= ndecoded;
    total_ndecoded += ndecoded;
  }
  while(count != 0) {
    bb.compact();
    int nread = doRead();
    if(nread == -1) {
      if(bb.hasRemaining()) {
        throw std::runtime_error("encoding error");
      }
      if (total_ndecoded == 0) {
        return -1;
      }
      return total_ndecoded;
    }
    decoder(&bb, dest_buf, offset, count, &ndecoded);
    offset += ndecoded;
    count -= ndecoded;
    total_ndecoded += ndecoded;
  }
  return total_ndecoded;
}

