#ifndef _CHARSETS_H
#define _CHARSETS_H

#include <map>

#include "ByteBuffer.h"

using namespace std;

typedef void (*charset_decoder) (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded
  );

typedef void (*charset_encoder) (
  const char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded);

class Charset {
  public:
    string name;

    charset_decoder decoder;

    charset_encoder encoder;

    bool hasBom;

    char32_t bom;

    Charset(string name, charset_decoder decoder, charset_encoder encoder) : Charset(name, decoder, encoder, false, (char32_t)0) {
    }

    Charset(string name, charset_decoder decoder, charset_encoder encoder, bool hasBom, char32_t bom) : 
              name(name), decoder(decoder), encoder(encoder), hasBom(hasBom), bom(bom) {
    }

    ~Charset() {
    }
};

class KnownCharsets {
  public:
    static map<string, Charset> charsetTable;

    static class Init {
      public:
        Init();
    } initializer;
};

#endif
