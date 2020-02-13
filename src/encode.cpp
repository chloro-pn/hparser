#include "../include/encode.h"

char32_t endian_cast(char32_t c, endian en) {
  if(local_endian().get() == en) {
    return c;
  }
  union {
    char32_t a;
    unsigned char b[4];
  } tmp;

  tmp.a = c;
  unsigned char t = tmp.b[0];
  tmp.b[0] = tmp.b[3];
  tmp.b[3] = t;

  t = tmp.b[1];
  tmp.b[1] = tmp.b[2];
  tmp.b[2] = t;
  return tmp.a;
}

size_t utf8_to_utf32(const std::string& src, std::u32string& dst, endian en) {
  size_t index = 0;
  while(index < src.size()) {
    char32_t tmp = 0;
    unsigned char c = src[index];
    int last = 0;
    if(c < 0x80) {
      tmp = c;
      dst.push_back(endian_cast(tmp, en));
      ++index;
      continue;
    }
    else if(c < 0xC0 || c > 0xF7) {
      return index; //invalid.
    }
    else if((c & 0xE0) == 0xC0) {
      tmp = c & 0x1F;
      last = 1;
    }
    else if((c & 0xF0) == 0xE0) {
      tmp = c & 0x0F;
      last = 2;
    }
    else if((c & 0xF8) == 0xF0) {
      tmp = c & 0x07;
      last = 3;
    }
    else {
      return index; //invalid;
    }

    for(int i = 1; i <= last; ++i) {
      c = src[index + i];
      tmp = (tmp << 6) | (c & 0x3f);
    }
    dst.push_back(endian_cast(tmp, en));
    index = index + last + 1;
  }
  return index;
}

size_t utf32_to_utf8(const std::u32string& str, std::string& dst, endian en) {
  size_t index = 0;
  while(index < str.size()) {
    char32_t c = endian_cast(str[index], en);
    if(c >= 0x1FFFFFU) {
      return index; // invalid
    }
    else if(c < 0x80U) {
      dst.push_back(char(c));
    }
    else if(c < 0x800U) {
      dst.push_back(char((c >> 6) | 0xC0U));
      dst.push_back(char((c & 0x3FU) | 0x80U));
    }
    else if(c < 0x10000U) {
      dst.push_back(char((c >> 12)) | 0xE0U);
      dst.push_back(char(((c >> 6) & 0x3FU) | 0x80U));
      dst.push_back(char((c & 0x3FU) | 0x80U));
    }
    else {
      dst.push_back(char((c >> 18)) | 0xF0U);
      dst.push_back(char(((c >> 12) & 0x3FU) | 0x80U));
      dst.push_back(char(((c >> 6) & 0x3FU) | 0x80U));
      dst.push_back(char((c & 0x3FU) | 0x80U));
    }
    ++index;
    continue;
  }
  return index;
}
