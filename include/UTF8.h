#ifndef _UTF8_H
#define _UTF8_H

#include "ByteBuffer.h"

#define MAX_CODE_POINT 0x10ffff

void utf8_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded);

void utf8_encoder (
  const char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded);

#endif
