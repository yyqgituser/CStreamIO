#include "TextWriter.h"
#include "TextReader.h"
#include "UTF8.h"
#include "GBK.h"

#include "gtest/gtest.h"
#include <fstream>

TEST(TextWriterTest, TextWriterTestCaseUTF8) {
  std::ofstream ofs("TextWriterTestCaseUTF8.txt", std::ios::out|std::ios::binary|std::ios::trunc);
  TextWriter writer(&ofs, utf8_encoder);

  char32_t src_buf[] = U"美英法对利比亚发动空袭 卡扎菲欲报复";
  unsigned int src_n = 10;
  writer.write(src_buf, 0, src_n);

  writer.flush();
  ofs.close();

  char32_t dst_buf[20];
  std::ifstream ifs("TextWriterTestCaseUTF8.txt", std::ios::in|std::ios::binary);
  TextReader reader(&ifs, utf8_decoder);

  int skip = reader.skipUTF8BOM();
  EXPECT_EQ(skip, 0);

  unsigned int dst_n = reader.read(dst_buf, 0, 20);
  ifs.close();

  EXPECT_EQ(src_n, dst_n);
  for(int i = 0; i < src_n; i++) {
    EXPECT_EQ(src_buf[i], dst_buf[i]);
  }
}

TEST(TextWriterTest, TextWriterTestCaseGBK) {
  std::ofstream ofs("TextWriterTestCaseGBK.txt", std::ios::out|std::ios::binary|std::ios::trunc);
  TextWriter writer(&ofs, gbk_encoder);

  char32_t src_buf[] = U"美英法对利比亚发动空袭 卡扎菲欲报复";
  unsigned int src_n = 10;
  writer.write(src_buf, 0, src_n);

  writer.flush();
  ofs.close();

  char32_t dst_buf[20];
  std::ifstream ifs("TextWriterTestCaseGBK.txt", std::ios::in|std::ios::binary);
  TextReader reader(&ifs, gbk_decoder);

  unsigned int dst_n = reader.read(dst_buf, 0, 20);
  ifs.close();

  EXPECT_EQ(src_n, dst_n);
  for(int i = 0; i < src_n; i++) {
    EXPECT_EQ(src_buf[i], dst_buf[i]);
  }
}


