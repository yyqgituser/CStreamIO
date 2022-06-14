#ifndef _ASCII_H
#define _ASCII_H

#include "ByteBuffer.h"

void ascii_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded);

void ascii_encoder (
  char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded);


#endif
