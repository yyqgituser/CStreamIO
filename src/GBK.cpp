#include "GBK.h"
#include "surrogate.h"

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

static inline unsigned int decodeSingle(int b) {
  if (b >= 0) {
      return (unsigned int)b;
  }
  return DECODER_REPLACE_CHAR;
}

static inline unsigned int decodeDouble(int byte1, int byte2) {
  if (((byte1 < 0) || (byte1 > GBK_DECODER_INDEX1_LEN))
      || ((byte2 < BYTE2_START) || (byte2 > BYTE2_END)))
      return DECODER_REPLACE_CHAR;

  int n = (GBK_DECODER_INDEX1[byte1] & 0xf) * (BYTE2_END - BYTE2_START + 1) + (byte2 - BYTE2_START);
  return (unsigned int)GBK_DECODER_INDEX2[GBK_DECODER_INDEX1[byte1] >> 4][n];
}

void gbk_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded ) {    

  char *sa = src->getBuffer();
  unsigned int sp = src->getPosition();
  unsigned int sl = src->getLimit();
  unsigned int dp = offset;
  unsigned  int dl = offset + count;

  while (sp < sl && dp < dl) {
    int b1, b2;

    b1 = sa[sp];
    int inputSize = 1;

    unsigned int c = decodeSingle(b1);
    if(c == DECODER_REPLACE_CHAR) {
      b1 &= 0xff;
      if (sl - sp < 2) {
        break;
      }
      b2 = sa[sp + 1] & 0xff;
      c = decodeDouble(b1, b2);
      inputSize = 2;
    }

    dest_buf[dp++] = c;
    sp += inputSize;
  }

  src->setPosition(sp);
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
  char* da = dest->getBuffer();
  unsigned int dp = dest->getPosition();
  unsigned int dl = dest->getLimit();

  while (sp < sl) {
    unsigned int c = src_buf[sp];

    int b = encodeSingle(c);
    if (b != -1) { // Single Byte
      if(dl - dp < 1) {
        goto return_exit;
      }
      da[dp++] = (char)b;
      sp++;
      continue;
    }

    int ncode  = encodeDouble(c);
    if (ncode != 0 && c != 0x0 ) {
      if (dl - dp < 2) {
        goto return_exit;
      }
      da[dp++] = (char) ((ncode & 0xff00) >> 8);
      da[dp++] = (char) (ncode & 0xff);
      sp++;
      continue;
    }
    if (dl - dp < 1) {
     goto return_exit;
    }
    da[dp++] = ENCODER_REPLACE_BYTE;
    sp++;
  }

return_exit:
  dest->setPosition(dp);
  *nencoded = (sp - offset);  
}
