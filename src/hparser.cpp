#include "../include/hparser.h"
#include <string>
#include <vector>
#include <regex>
#include <stack>
#include <memory>
#include <functional>
#include <queue>
#include <exception>

void hparser::end_label_handle(std::stack<std::shared_ptr<html_element>>& s, size_t& begin) {
  size_t j = begin;
  while(raw_[j] != '>') {
    ++j;
  }
  string_type end_label = raw_.substr(begin + 2, j - begin - 2);
  if(s.empty() == true) {
    throw parser_error("unknow error!");
  }
  auto this_e = s.top();
  s.pop();
  if(end_label != this_e->tag_) {
    throw parser_error(std::string("tag ") + end_label + " match error!");
  }
  if(s.empty() == true) {
    root_ = this_e;
  }
  else {
    this_e->parent_ = s.top();
    s.top()->childs_.push_back(this_e);
  }
  begin = j + 1;
}

void hparser::begin_label_handle(std::stack<std::shared_ptr<html_element>>& s, size_t& begin) {
    int j = begin;
    while(true) {
      if(raw_[j] == '>' || raw_[j] == ' ') {
        break;
      }
      ++j;
    }
    bool inner_close = false;
    std::shared_ptr<html_element> tmp = std::make_shared<html_element>();
    tmp->tag_ = raw_.substr(begin + 1, j - begin - 1);
    if(raw_[j] == ' ') {
      //处理属性
      int k = j;
      while(raw_[k] != '>') {
        ++k;
      }
      ++k;
      std::regex pattern(" +([^ ]+)=(\".+?\")");
      auto b = raw_.cbegin();
      b += j;
      auto e = raw_.cbegin();
      e += k;
      std::smatch result;
      while(std::regex_search(b, e, result, pattern)) {
        b = result[0].second;
        html_element::kv kv;
        kv.key_ = result[1];
        kv.value_ = result[2];
        tmp->attrs_.push_back(kv);
      }
      if(std::regex_search(b, e, result, std::regex(" */>"))) {
        inner_close = true;
      }
      j = k;
    }
    else {
      ++j;
    }
    if(inner_close == false) {
      s.push(tmp);
    }
    else {
      if(s.empty() == true) {
        root_ = tmp;
      }
      else {
        s.top()->childs_.push_back(tmp);
      }
    }
    begin = j;
  }

void hparser::note_label_handle(std::stack<std::shared_ptr<html_element>>& s, size_t& begin) {
  int j = begin;
  while(raw_[j] != '>') {
    ++j;
  }
  ++j;
  string_type notes = raw_.substr(begin, j - begin);
  if(s.empty() == false) {
    s.top()->content_ += notes;
  }
  else {
    global_notes_ += notes;
  }
  begin = j;
}

void hparser::parse() {
  std::stack<std::shared_ptr<html_element>> s;
  size_t begin = 0;

  while(begin < raw_.size()) {
    if(raw_[begin] == '<') {
      if(raw_[begin + 1] == '/') {
        end_label_handle(s, begin);
      }
      else if(raw_[begin + 1] == '!') {
        note_label_handle(s, begin);
      }
      else {
        begin_label_handle(s, begin);
      }
    }
    else {
      if(s.empty() == false) {
        s.top()->content_.push_back(raw_[begin]);
      }
      else {
        global_notes_.push_back(raw_[begin]);
      }
      ++begin;
    }
  }
  if(s.empty() == false || root_ == nullptr) {
    throw parser_error("unknow error!");
  }
}

std::vector<std::shared_ptr<hparser::html_element>> hparser::hiera_trave_find(std::function<bool(std::shared_ptr<html_element>)> check_func) const {
  std::queue<std::shared_ptr<html_element>> que;
  std::vector<std::shared_ptr<html_element>> result;
  que.push(root_);
  while(que.empty() == false) {
    auto this_element = que.front();
    que.pop();
    if(check_func(this_element) == true) {
      result.push_back(this_element);
    }
    for(auto it = this_element->childs_.begin(); it != this_element->childs_.end(); ++it) {
      que.push(*it);
    }
  }
  return result;
}

hparser::hparser(std::string str):raw_(str) {
  parse();
}

std::vector<std::shared_ptr<hparser::html_element>> hparser::find_tag(std::string str) const {
  std::regex pattern(str);
  std::smatch result;
  std::vector<std::shared_ptr<html_element>> re;
  return hiera_trave_find([&](std::shared_ptr<html_element> each)->bool {
    if(std::regex_match(each->tag_, result, pattern) == true) {
      return true;
    }
    return false;
  });
}

std::vector<std::shared_ptr<hparser::html_element>> hparser::find_attr(std::string str) const {
  std::regex pattern(str);
  std::smatch result;
  std::vector<std::shared_ptr<html_element>> re;
  return hiera_trave_find([&](std::shared_ptr<html_element> each)->bool {
    for(auto it = each->attrs_.begin(); it != each->attrs_.end(); ++it) {
      if(std::regex_match(it->key_, result, pattern) == true) {
        return true;
      }
    }
    return false;
  });
}

std::vector<std::shared_ptr<hparser::html_element>> hparser::find_content(std::string str) const {
  std::regex pattern(str);
  std::smatch result;
  std::vector<std::shared_ptr<html_element>> re;
  return hiera_trave_find([&](std::shared_ptr<html_element> each)->bool {
    if(std::regex_search(each->content_, result, pattern) == true) {
      return true;
    }
    return false;
  });
}

std::vector<std::shared_ptr<hparser::html_element>> hparser::find(std::function<bool(std::shared_ptr<html_element> each)> func) const {
  return hiera_trave_find(func);
}
