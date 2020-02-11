#include "../third_party/catch.hpp"
#include <regex>
#include <iostream>
#include <fstream>
#include "../include/hparser.h"

TEST_CASE("regex test","[regex]") {
  std::string content("<html><p1>hello world</p1></html>");
  hparser h(content);

  h.parse();
}


