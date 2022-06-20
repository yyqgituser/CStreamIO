#include "TextWriter.h"
#include "TextReader.h"

#include "gtest/gtest.h"
#include <fstream>

TEST(TextWriterTest, TextWriterTestCaseUTF8) {
  std::ofstream ofs("TextWriterTestCaseUTF8.txt", std::ios::out|std::ios::binary|std::ios::trunc);
  TextWriter writer(&ofs, "UTF-8", 10);

  char32_t src_buf[] = U"\U0001F607士大夫士大夫精神放松的防卫任务\U0001F607士大夫士大夫精神放松的防卫任务\U0001F607士大夫士大夫精神放松的防卫任务";
  unsigned int src_n = sizeof(src_buf) / sizeof(src_buf[0]) - 1;
  writer.write(src_buf, 0, src_n);

  writer.flush();

  char32_t dst_buf[200];
  std::ifstream ifs("TextWriterTestCaseUTF8.txt", std::ios::in|std::ios::binary);
  TextReader reader(&ifs, "UTF-8", 10); 

  unsigned int dst_n = reader.read(dst_buf, 0, 200);

  EXPECT_EQ(src_n, dst_n);
  for(unsigned int i = 0; i < src_n; i++) {
    EXPECT_EQ(src_buf[i], dst_buf[i]);
  }
}

TEST(TextWriterTest, TextWriterTestCaseGBK) {
  std::ofstream ofs("TextWriterTestCaseGBK.txt", std::ios::out|std::ios::binary|std::ios::trunc);
  TextWriter writer(&ofs, "GBK", 10);

  char32_t src_buf[] = U"士大夫士大夫精神放松的防卫任务 士大夫士大夫精神放松的防卫任务 士大夫士大夫精神放松的防卫任务";
  unsigned int src_n = sizeof(src_buf) / sizeof(src_buf[0]) - 1;
  writer.write(src_buf, 0, src_n);

  writer.flush();

  char32_t dst_buf[200];
  std::ifstream ifs("TextWriterTestCaseGBK.txt", std::ios::in|std::ios::binary);
  TextReader reader(&ifs, "GBK", 10);

  unsigned int dst_n = reader.read(dst_buf, 0, 200);

  EXPECT_EQ(src_n, dst_n);
  for(unsigned int i = 0; i < src_n; i++) {
    EXPECT_EQ(src_buf[i], dst_buf[i]);
  }
}


