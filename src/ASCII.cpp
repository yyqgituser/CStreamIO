#include "ASCII.h"


void ascii_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded) {
  char *src_buf = src->buffer;
  int sb = src->begin;
  int se = src->end;
	int db = offset;
	int de = offset + count;

  while((sb < se) && (db < de)) {
    dest_buf[db++] = (char32_t)(src_buf[sb++] & 0xFF);
  }
  src->begin = sb;
  *ndecoded = db - offset;  
}

void ascii_encoder (
  char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded) {

  unsigned int sb = offset;
  unsigned int se = offset + count;
  char* dest_buf = dest->buffer;
  int db = dest->end;
  int de = dest->capacity;

  while((sb < se) && db < de) {
    dest_buf[db++] = (char)src_buf[sb++];
  }
  
  dest->end = db;
  *nencoded = (sb - offset); 
}
