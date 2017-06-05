#ifndef _GBK_H
#define _GBK_H

#include "ByteBuffer.h"

// GBK decoder, map GBK code to UCS4

void gbk_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded );

// BGK encoder
void gbk_encoder (
  const char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded);

#endif

