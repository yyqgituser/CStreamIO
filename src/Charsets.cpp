#include "Charsets.h"
#include "UTF8.h"
#include "GBK.h"

map<string, Charset> KnownCharsets::charsetTable;


KnownCharsets::Init::Init() {
    KnownCharsets::charsetTable.insert(std::pair<string, Charset>("UTF-8", Charset("UTF-8", utf8_decoder, utf8_encoder, true, (char32_t)0xEFBBBF)));
    KnownCharsets::charsetTable.insert(std::pair<string, Charset>("GBK", Charset("GBK", gbk_decoder, gbk_encoder)));
}

KnownCharsets::Init KnownCharsets::initializer;