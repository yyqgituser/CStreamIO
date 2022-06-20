#include "MemoryInputStream.h"

int MemoryInputStream::readVarint() {
  const static unsigned UNMASK_2_0 = 0x001fc07f;
  const static unsigned UNMASK_4_2_0 = 0xf01fc07f;
  int a, b;
  int v;

  a = readByte();
  if ( (a & 0x80) == 0) {
    return a;
  }

  b = readByte();
  if ( (b & 0x80) == 0) {
    a &= UNMASK_2_0;
    a <<= 7;
    v = a | b;
    return v;
  }

  a <<= 14;
  a |= readByte();
  if ( (a & 0x80) == 0) {
    a &= UNMASK_4_2_0;
    b &= UNMASK_2_0;
    b <<= 7;
    v = a | b;
    return v;
  }

  a &= UNMASK_4_2_0;
  b <<= 14;
  b |= readByte();
  if ( (b & 0x80) == 0) {
    b &= UNMASK_4_2_0;
    a <<= 7;
    v = a | b;
    return v;
  }

  b &= UNMASK_4_2_0;
  v = (a << 11) | (b << 4) | (readByte() & 0xf);
  return v;
}