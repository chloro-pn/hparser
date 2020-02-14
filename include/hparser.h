#pragma once
#include <string>
#include <vector>
#include <stack>
#include <memory>
#include <functional>
#include <exception>
#include <algorithm>

class parser_error : public std::exception {
private:
  std::string what_;
public:
  explicit parser_error(std::string content):what_(content) {

  }

  const char* what() const noexcept override {
    return what_.c_str();
  }
};

class hparser {
private:
  using string_type = std::string;
  using char_type = typename string_type::value_type;

  string_type raw_;
  string_type global_notes_;

  class html_element {
  private:
    friend class hparser;
    struct kv {
      string_type key_;
      string_type value_;
    };
    string_type tag_;
    std::vector<kv> attrs_;
    string_type content_;
    std::vector<std::shared_ptr<html_element>> childs_;
    std::weak_ptr<html_element> parent_;

  public:
    using kv_type = kv;

    html_element() = default;

    string_type tag() const {
      return tag_;
    }

    string_type content() const {
      return content_;
    }

    size_t attrs_size() const {
      return attrs_.size();
    }

    size_t childs_size() const {
      return childs_.size();
    }

    kv_type get_attr(size_t index) const {
      return attrs_.at(index);
    }

    std::shared_ptr<html_element> get_child(size_t index) const {
      return childs_.at(index);
    }

    std::vector<kv_type> get_all_attrs() const {
      return attrs_;
    }

    std::vector<std::shared_ptr<html_element>> get_all_childs() const {
      return childs_;
    }

    string_type operator[](string_type str) const {
      auto it = std::find_if(attrs_.begin(), attrs_.end(), [&](const kv& each)->bool {
                               return each.key_ == str;
                             });
      if(it != attrs_.end()) {
        return it->value_;
      }
      return "";
    }

    bool root() const {
      if(parent_.lock()) {
        return false;
      }
      return true;
    }

    std::shared_ptr<html_element> parent() const {
      return parent_.lock();
    }
  };

  std::shared_ptr<html_element> root_;

  void end_label_handle(std::stack<std::shared_ptr<html_element>>& s, size_t& begin);

  void begin_label_handle(std::stack<std::shared_ptr<html_element>>& s, size_t& begin);

  void note_label_handle(std::stack<std::shared_ptr<html_element>>& s, size_t& begin);

  void parse();

  std::vector<std::shared_ptr<html_element>> hiera_trave_find(std::function<bool(std::shared_ptr<html_element>)> check_func) const;

public:
  using element_type = html_element;

  string_type global_notes() const {
    return global_notes_;
  }

  explicit hparser(std::string str);

  std::shared_ptr<html_element> get_root() const {
    return root_;
  }

  std::vector<std::shared_ptr<html_element>> find_tag(std::string str) const;

  std::vector<std::shared_ptr<html_element>> find_attr(std::string str) const;

  std::vector<std::shared_ptr<html_element>> find_content(std::string str) const;

  std::vector<std::shared_ptr<html_element>> find(std::function<bool(std::shared_ptr<html_element> each)> func) const;
};
