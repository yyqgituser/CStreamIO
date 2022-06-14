#include "gtest/gtest.h"
#include "UTF8.h"

TEST(UTF8_test, utf8_encoding_testcase) {
  ByteBuffer bb(100);
  char32_t src_buf[1];
  char32_t dst_buf[10];
  unsigned int src_n;
  unsigned int dst_n;
  //
  for (int i = 0; i <= MAX_CODE_POINT; i++) {
    bb.clear();
    src_buf[0] = i;
    src_n = 0;
    utf8_encoder(src_buf, 0, 1, &bb, &src_n);
    EXPECT_EQ(1, src_n);
    dst_n = 0;
    utf8_decoder(&bb, dst_buf, 0, 10, &dst_n);
    EXPECT_EQ(1, dst_n);
    EXPECT_EQ(src_buf[0], dst_buf[0]);    
  }
}
