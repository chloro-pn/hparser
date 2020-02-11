#pragma once
#include <string>
#include <vector>
#include <regex>
#include <stack>
#include <iostream>
#include <cassert>

class hparser {
private:
  int state_;
  std::string raw_;

  struct html_element {
    std::string label_;
    std::vector<std::pair<std::string, std::string>> attrs_;
    std::string content_;
    std::vector<html_element> childs_;
  };

  html_element root_;

public:
  explicit hparser(std::string str):raw_(str) {

  }

  void parse() {
    std::stack<html_element> s;
    std::vector<html_element> current_elements_;
    int begin = 0;
    state_ = 0;

    while(begin < raw_.size()) {

      if(raw_[begin] == '<') {
        int j = begin;
        if(raw_[begin + 1] == '/') {
          //结束标签
          while(raw_[j] != '>') {
            ++j;
          }
          std::string end_label = raw_.substr(begin + 2, j - begin - 2);
          auto this_e = s.top();
          s.pop();
          if(state_ == 1) {
            for(auto it = current_elements_.begin(); it!=current_elements_.end(); ++it) {
              this_e.childs_.push_back(*it);
            }
            current_elements_.clear();
            current_elements_.push_back(this_e);
          }
          else {
            current_elements_.push_back(this_e);
          }
          state_ = 1;
        }
        else {
          //开始标签
          while(true) {
            if(raw_[j] == '>' || raw_[j] == ' ') {
              break;
            }
            ++j;
          }
          html_element tmp;
          tmp.label_ = raw_.substr(begin + 1, j - begin - 1);
          if(j == ' ') {
            //处理属性
          }
          s.push(tmp);
          state_ = 0;
        }
        begin = j;
      }
      else {
        ++begin;
      }
    }
    root_ = s.top();
  }
};
