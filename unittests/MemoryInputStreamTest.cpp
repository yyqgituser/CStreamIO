#include "gtest/gtest.h"
#include "MemoryInputStream.h"

#include <cstring>

TEST(MemoryInputStreamTest, MemoryInputStreamTestCase1) {
  const char *data = "a1234567890";
  MemoryInputStream in(data, std::strlen(data));
  char buf[20];

  int num = in.read(buf, 0, 20);
  EXPECT_EQ(num, std::strlen(data));
  for(int i = 0; i < std::strlen(data); i++) {
    EXPECT_EQ(buf[i], data[i]);
  }

  int b = in.read();
  EXPECT_EQ(b, -1);

  num = in.read(buf, 0, sizeof(buf));
  EXPECT_EQ(num, -1);
}
