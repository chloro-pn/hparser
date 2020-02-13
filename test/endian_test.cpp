#include "../third_party/catch.hpp"
#include "../include/endian.h"
#include <iostream>

TEST_CASE("endian test") {
  int i = 1;
  if(*((char*)&i) ==  0) {
    REQUIRE(local_endian().get() == endian::big_endian);
  }
  else {
    REQUIRE(local_endian().get() == endian::little_endian);
  }
}

