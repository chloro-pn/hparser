#pragma once
#include <string>
#include "endian.h"

char32_t endian_cast(char32_t c, endian en);

size_t utf8_to_utf32(const std::string& src, std::u32string& dst, endian en = local_endian().get());

size_t utf32_to_utf8(const std::u32string& str, std::string& dst, endian en = local_endian().get());
