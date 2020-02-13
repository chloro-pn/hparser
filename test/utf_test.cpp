#include "../third_party/catch.hpp"
#include <iostream>
#include "../include/encode.h"

TEST_CASE("utf8_to_utf32 test") {
  std::string src(u8"中国");
  std::u32string result;
  size_t index = 0;
  index = utf8_to_utf32(src, result);
  REQUIRE(index == src.size());

  char32_t r = result[0];
  REQUIRE((uint32_t)r == 0x00004E2DU);
  r = result[1];
  REQUIRE((uint32_t)r == 0x000056FDU);
  if(local_endian().get() == endian::little_endian) {
    result.clear();
    index = 0;
    utf8_to_utf32(u8"中", result, endian::big_endian);
    r = result[0];
    REQUIRE((uint32_t)r == 0x2D4E0000);
  }
}

TEST_CASE("utf32_to_utf8 test") {
  std::u32string src(U"中");
  std::string result;
  size_t index = utf32_to_utf8(src, result);
  REQUIRE(index == src.size());
  REQUIRE(result.size() == 3);
  char r = result[0];
  REQUIRE(r == u8"中"[0]);
  r = result[1];
  REQUIRE(r == u8"中"[1]);
  r = result[2];
  REQUIRE(r == u8"中"[2]);
}
