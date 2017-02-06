#include "gtest/gtest.h"
#include "ASCII.h"

TEST(ASCII_test, ascii_coder_testcase) {
  ByteBuffer bb(100);
  char32_t src_buf[] = {'a', 'b', 'c', '\0'};
  char32_t dst_buf[4];
  unsigned int src_n;
  unsigned int dst_n;
  
  bb.setLimit(bb.getCapacity());
  ascii_encoder(src_buf, 0, 3, &bb, &src_n);
  bb.setLimit(bb.getPosition());
  bb.setPosition(0);
  ascii_decoder(&bb, dst_buf, 0, 3, &dst_n);
  EXPECT_EQ(src_n, dst_n);
  for(int i = 0; i < 3; i++) {
    EXPECT_EQ(src_buf[i], dst_buf[i]);
  }
}
