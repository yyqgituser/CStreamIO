#include "GBK.h"
#include <stdexcept>

#define DECODER_REPLACE_CHAR 0xFFFD

#define BYTE2_START 0x40

#define BYTE2_END 0xFE

extern short GBK_DECODER_INDEX1[];

extern unsigned short *GBK_DECODER_INDEX2[];

extern int GBK_DECODER_INDEX1_LEN;

#define ENCODER_REPLACE_BYTE ('?')

extern short GBK_ENCODER_INDEX1[];

extern unsigned short *GBK_ENCODER_INDEX2[];

extern int GBK_ENCODER_INDEX1_LEN;

static inline int decodeSingle(int b) {
  if (b >= 0) {
      return (unsigned int)b;
  }
  return -1;
}

static inline unsigned int decodeDouble(int byte1, int byte2) {
  if (((byte1 < 0) || (byte1 > GBK_DECODER_INDEX1_LEN))
      || ((byte2 < BYTE2_START) || (byte2 > BYTE2_END)))
      throw std::runtime_error("GBK code decode error: " + std::to_string(byte1) + ", " + std::to_string(byte2));

  int n = (GBK_DECODER_INDEX1[byte1] & 0xf) * (BYTE2_END - BYTE2_START + 1) + (byte2 - BYTE2_START);
  return (unsigned int)GBK_DECODER_INDEX2[GBK_DECODER_INDEX1[byte1] >> 4][n];
}

void gbk_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded ) {    

  char *src_buf = src->buffer;
  unsigned int sp = src->begin;
  unsigned int sl = src->end;
  unsigned int dp = offset;
  unsigned  int dl = offset + count;

  while (sp < sl && dp < dl) {
    int b1, b2;

    b1 = src_buf[sp];
    int inputSize = 1;

    int c = decodeSingle(b1);
    if(c == -1) {
      b1 &= 0xff;
      if (sl - sp < 2) {
        break;
      }
      b2 = src_buf[sp + 1] & 0xff;
      c = decodeDouble(b1, b2);
      inputSize = 2;
    }

    dest_buf[dp++] = c;
    sp += inputSize;
  }

  src->begin = sp;
  *ndecoded = (dp - offset);  
}

static inline int encodeSingle(unsigned int ch) {
  if (ch < 0x80)
      return (int)ch;
  else
      return -1;
}

static inline int encodeDouble(unsigned int ch) {
  int offset = GBK_ENCODER_INDEX1[((ch & 0xff00) >> 8 )] << 8;
  return GBK_ENCODER_INDEX2[offset >> 12][(offset & 0xfff) + (ch & 0xff)];
}

void gbk_encoder (
  const char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded) {

  unsigned int sp = offset;
  unsigned int sl = offset + count;
  char* dest_buf = dest->buffer;
  unsigned int dp = dest->end;
  unsigned int dl = dest->capacity;

  while (sp < sl) {
    char32_t c = src_buf[sp];

    int b = encodeSingle(c);
    if (b != -1) { // Single Byte
      if(dl - dp < 1) {
        break;
      }
      dest_buf[dp++] = (char)b;
      sp++;
      continue;
    }

    int ncode  = encodeDouble(c);
    if (ncode != 0 && c != 0x0 ) {
      if (dl - dp < 2) {
        break;
      }
      dest_buf[dp++] = (char) ((ncode & 0xff00) >> 8);
      dest_buf[dp++] = (char) (ncode & 0xff);
      sp++;
      continue;
    } else {
      throw std::runtime_error("GBK code encode error: " + std::to_string(c));
    }
  }
  dest->end = dp;
  *nencoded = (sp - offset);  
}
