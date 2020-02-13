#include "../include/hparser.h"
#include "../third_party/catch.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>

int main() {
  std::ifstream in("/home/pn/html/container/examples/1.html", std::ios::binary);
  assert(in.good() == true);
  std::string content;
  while(true) {
    char tmp;
    in.read(&tmp, sizeof(tmp));
    if(in.eof() == true) {
      break;
    }
    content.push_back(tmp);
  }
  in.close();
  hparser h(content);
  auto result = h.find_attr("href");
  for(auto it = result.begin(); it != result.end(); ++it) {
    std::cout << (*it)->tag() << " : " << (*it)->operator[]("href") << std::endl;
  }
  return 0;
}
