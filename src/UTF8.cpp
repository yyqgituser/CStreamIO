#include "UTF8.h"

#include <stdexcept>

/*
 * # Bits   Bit pattern
 * 1    7   0xxxxxxx
 * 2   11   110xxxxx 10xxxxxx
 * 3   16   1110xxxx 10xxxxxx 10xxxxxx
 * 4   21   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 5   26   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 6   31   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 */


#define isContinuation(b) (((b) & 0xc0) == 0x80)

void utf8_decoder (
    ByteBuffer *src,
    char32_t *dest_buf,
    unsigned int offset,
    unsigned int count,
    unsigned int *ndecoded) {
  char *src_buf = src->buffer;
  unsigned int sb = src->begin;
  unsigned int se = src->end;
  unsigned int db = offset;
  unsigned int de = offset + count;

  while (sb < se && db < de) {
    int b1 = src_buf[sb] & 0xff;
    int b2, b3, b4, b5, b6;
    if (b1 <= 0x7f) {
      dest_buf[db++] = (char32_t)(b1 & 0x7f);
      sb++;
    } else if (b1 <= 0xdf) {
      if (se - sb < 2) {
        break;
      }
      b2 = src_buf[sb + 1];
      if (!isContinuation(b2)) {
        goto fail_exit;
      }
      dest_buf[db++] = (char32_t)(((b1 & 0x1f) << 6) | (b2 & 0x3f));
      sb += 2;
    } else if (b1 <= 0xef) {
      if (se - sb < 3) {
        break;
      }
      b2 = src_buf[sb + 1];
      b3 = src_buf[sb + 2];
      if ((!isContinuation(b2)) || (!isContinuation(b3))) {
        goto fail_exit;
      }
      dest_buf[db++] = (char32_t)(((b1 & 0x0f) << 12) | ((b2 & 0x3f) << 6) | (b3 & 0x3f));
      sb += 3;
    } else if (b1 <= 0xf7) {
      if (se - sb < 4) {
        break;
      }
      b2 = src_buf[sb + 1];
      b3 = src_buf[sb + 2];
      b4 = src_buf[sb + 3];
      if ((!isContinuation(b2)) || (!isContinuation(b3)) || (!isContinuation(b4))) {
        goto fail_exit;
      }
      char32_t cp = (char32_t)(((b1 & 0x07) << 18) | ((b2 & 0x3f) << 12) | ((b3 & 0x3f) << 6) | (b4 & 0x3f));
      if (cp > MAX_CODE_POINT) {
        goto fail_exit;
      }
      dest_buf[db++] = cp;
      sb += 4;
    } else if (b1 <= 0xfb) {
      if (se - sb < 5) {
        break;
      }
      b2 = src_buf[sb + 1];
      b3 = src_buf[sb + 2];
      b4 = src_buf[sb + 3];
      b5 = src_buf[sb + 4];
      if ((!isContinuation(b2)) || (!isContinuation(b3)) || (!isContinuation(b4)) || (!isContinuation(b5))) {
        goto fail_exit;
      }
      char32_t cp = (char32_t)(((b1 & 0x03) << 24) | ((b2 & 0x3f) << 18) | ((b3 & 0x3f) << 12) | ((b4 & 0x3f) << 6) | (b5 & 0x3f));
      if (cp > MAX_CODE_POINT) {
        goto fail_exit;
      }
      dest_buf[db++] = cp;
      sb += 5;
    } else if (b1 <= 0xfd) {
      if (se - sb < 6) {
        break;
      }
      b2 = src_buf[sb + 1];
      b3 = src_buf[sb + 2];
      b4 = src_buf[sb + 3];
      b5 = src_buf[sb + 4];
      b6 = src_buf[sb + 5];
      if ((!isContinuation(b2)) || (!isContinuation(b3)) || (!isContinuation(b4)) || (!isContinuation(b5)) || (!isContinuation(b6))) {
        goto fail_exit;
      }
      char32_t cp = (char32_t)(((b1 & 0x01) << 30) | ((b2 & 0x3f) << 24) | ((b3 & 0x3f) << 18) | ((b4 & 0x3f) << 12) | ((b5 & 0x3f) << 6) | (b6 & 0x3f));;
      if (cp > MAX_CODE_POINT) {
        goto fail_exit;
      }
      dest_buf[db++] = cp;
      sb += 6;
    } else {
      goto fail_exit;
    }
  }

  src->begin = sb;
  *ndecoded = db - offset;
  return;
fail_exit:
  throw std::runtime_error("UTF-8 encoding error");
}

void utf8_encoder (
  const char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded) {

  unsigned int sb = offset;
  unsigned int se = offset + count;
  char* da = dest->buffer;
  unsigned int db = dest->end;
  unsigned int de = dest->capacity;

  while (sb < se) {
    char32_t uc = src_buf[sb];
    if (uc <= 0x7f) {
      if (db >= de) {
        break;
      }
      da[db++] = (char)uc;
      sb++;
    } else if (uc < 0x7ff) { // 2 bytes, 11 bits
      if (de - db < 2) {
        break;
      }
      da[db++] = (char)(0xc0 | ((uc >> 6)));
      da[db++] = (char)(0x80 | (uc & 0x3f));
      sb++;
    } else if (uc <= 0xffff) { // 3 bytes, 16 bits      
      if (de - db < 3) {
        break;
      }
      da[db++] = (char)(0xe0 | ((uc >> 12)));
      da[db++] = (char)(0x80 | ((uc >> 06) & 0x3f));
      da[db++] = (char)(0x80 | (uc & 0x3f));
      sb++;
      continue;
    } else if (uc <= MAX_CODE_POINT) {
      if (de - db < 4) {
        break;
      }
      da[db++] = (char)(0xf0 | ((uc >> 18)));
      da[db++] = (char)(0x80 | ((uc >> 12) & 0x3f));
      da[db++] = (char)(0x80 | ((uc >> 06) & 0x3f));
      da[db++] = (char)(0x80 | (uc & 0x3f));
      sb++; 
    } else {
      throw std::runtime_error("UTF-8 encoding error");
    }
  }

  dest->end = db;
  *nencoded = sb - offset;
}
