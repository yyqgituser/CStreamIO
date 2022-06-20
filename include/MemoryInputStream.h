#ifndef _MEMORY_INPUT_STREAM_H
#define _MEMORY_INPUT_STREAM_H

#include <cstring>
#include <stdexcept>

class MemoryInputStream {
private:
  // buffer
  const char* data;

  // end of data
  unsigned int limit;

  // current position
  unsigned int position;

public:
  MemoryInputStream(const char *mem, unsigned int size) :
    data(mem), limit(size), position(0) {
  }

  ~MemoryInputStream() {
  }

  // reads one byte, return -1 if there is no data for the end of stream.
  int read() {
    return position < limit ? (data[position++] & 0xff) : -1;
  }

  int read(char* buffer, unsigned int offset, unsigned int count) {
    if(position == limit) {
      return -1;
    }

    unsigned int remaining = limit - position;
    unsigned int cnt = count > remaining ? remaining : count;
    std::memcpy(buffer + offset, data + position, cnt);
    position += cnt;
    return cnt;
  }

  int readInt() {
    int ch1 = read();
    int ch2 = read();
    int ch3 = read();
    int ch4 = read();
    if ((ch1 | ch2 | ch3 | ch4) < 0)
      throw std::runtime_error("invalid data(unexpected eof)");
    return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
  }

  void readBlock(char *buffer, int offset, int length) {
    int n = 0;
    while (n < length) {
      int count = read(buffer, offset + n, length - n);
      if (count < 0) {
          throw std::runtime_error("invalid data(unexpected eof)");
      }
      n += count;
    }
  }

  long long readLong() {
  	char buf[8];
    readBlock(buf, 0, 8);
    return (((long long)buf[0] << 56) |
           ((long long)(buf[1] & 255) << 48) |
           ((long long)(buf[2] & 255) << 40) |
           ((long long)(buf[3] & 255) << 32) |
           ((long long)(buf[4] & 255) << 24) |
           ((buf[5] & 255) << 16) |
           ((buf[6] & 255) << 8) |
           ((buf[7] & 255) << 0));
  }

  int readByte() {
    int ch = read();
    if (ch < 0)
      throw std::runtime_error("invalid data(unexpected eof)");
    return ch;
  }

  int readVarint();

};

#endif
