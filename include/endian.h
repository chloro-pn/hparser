#pragma once

enum class endian {big_endian, little_endian};

class local_endian {
  union inner {
    short a;
    char b;
  };
  inner endian_;

public:
  local_endian() {
    endian_.a = 1;
  }

  endian get() const {
    return endian_.b == 0x01 ? endian::little_endian : endian::big_endian;
  }
};
