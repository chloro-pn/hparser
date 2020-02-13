#include "../third_party/catch.hpp"
#include "../include/hparser.h"

TEST_CASE("single tag test", "[hparser]") {
  hparser parser("<p1>hello world</p1>");
  auto result = parser.find_tag(u8"p1");
  REQUIRE(result.size() == 1);
  auto it = result.begin();
  REQUIRE((*it)->tag() == "p1");
  REQUIRE((*it)->content() == "hello world");
  REQUIRE((*it)->get_all_attrs().empty());
}

TEST_CASE("multi tags test", "[hparser]") {
  hparser parser(u8"<html><head></head></html>");
  auto result = parser.find_tag(u8"html");
  REQUIRE(result.size() == 1);
  REQUIRE(result[0]->get_all_childs().size() == 1);
  REQUIRE(result[0]->get_child(0)->tag() == "head");

  result = parser.find_tag("html|head");
  REQUIRE(result.size() == 2);
  REQUIRE(result[0]->tag() == "html");
  REQUIRE(result[1]->tag() == "head");
  REQUIRE(result[1]->root() == false);
  auto p = result[1]->parent();
  REQUIRE(p == result[0]);
}

TEST_CASE("parent test", "[hparser]") {
  hparser parser("<html><h1>hello world</h1><div><p1>...</p1></div></html>");
  auto root = parser.get_root();
  REQUIRE(root->get_child(0)->parent() == root);
  auto child = root->get_child(1);
  REQUIRE(child->tag() == "div");
  REQUIRE(child->get_child(0)->content() == "...");
  REQUIRE(child->get_child(0)->parent() == child);
}

TEST_CASE("single attr test", "[hparser]") {
  hparser parser("<html lang=\"zh-cn\"><head></head></html>");
  auto result = parser.find_attr("lang");
  REQUIRE(result.size() == 1);
  REQUIRE(result[0]->operator[]("lang") == "\"zh-cn\"");
}

TEST_CASE("multi attrs test", "[hparser]") {
  hparser parser("<body>"
                 "<meta name=\"robots\" content=\"all\"></meta>"
                 "<meta name=\"bob\"></meta>"
                 "</body>");
  auto result = parser.find_attr(u8"name|content");
  REQUIRE(result.size() == 2);
  REQUIRE(result[0]->operator[]("name") == "\"robots\"");
  REQUIRE(result[0]->operator[]("content") == "\"all\"");
  REQUIRE(result[1]->operator[]("name") == "\"bob\"");
}

TEST_CASE("exception test", "[hparser]") {
  try {
    hparser parser("<body>"
                   "<meta name=\"robots\" content=\"all\"></meta>"
                   "<meta name=\"bob\">"
                   "</body>"
                   );
  }catch(const parser_error& e) {
    REQUIRE(std::string(e.what()) == "tag body match error!");
  }
}

TEST_CASE("content find test", "[hparser]") {
  hparser parser("<html><h1>hello world</h1><div><p1>...</p1></div></html>");

  SECTION("find") {
    auto result = parser.find_content("hello|hi");
    REQUIRE(result.size() == 1);
    REQUIRE(result[0]->tag() == "h1");
    REQUIRE(result[0]->content() == "hello world");
  }

  SECTION("not find") {
    auto result = parser.find_content("error");
    REQUIRE(result.empty() == true);
  }
}

TEST_CASE("find test","[hparser]") {
  hparser parser("<html>"
                   "<h1>hello world</h1>"
                   "<p1>pangnan</p1>"
                   "<div>"
                     "<p1>...</p1>"
                   "</div>"
                 "</html>");
  auto result = parser.find([](std::shared_ptr<hparser::element_type> each)->bool {
    if(each->root() == false && each->parent()->tag() == "div" && each->tag() == "p1") {
      return true;
    }
    return false;
  });
  REQUIRE(result.size() == 1);
  REQUIRE(result[0]->content() == "...");
}
