# hparser
hparser is a light-weight, simple and fast xhtml parser library for c++11 with DOM-like interface, which supports utf-8 encoding(without BOM).

hparser 仅提供访问接口，不提供修改接口。

hparser 可基于正则表达式进行访问和查询。由于std::regex目前对unicode的支持有限，因此如果你使用ascii码以外的正则匹配可能无法得到正确答案。对此hparser提供
utf8_to_utf32接口(std::string -> std::u32string)和utf32_to_utf8(std::u32string -> std::string)接口。如果有支持u32string(即char32_t存储类型)
的正则表达式库，可以结合hparser.find接口以及utf8_to_utf32接口执行正则匹配。

# test
基于Catch2进行单元测试。

# build
hparser 使用cmake工具进行构建
```
mkdir build && cd build
cmake ..
make
```
在(project_dir)/build/lib 中会生成静态库libhparser.a。在(project_dir)/build/bin中会生成可执行程序 hparser_test 和 examples。分别为单元测试
程序和example程序。examples中通过解析文件(project_dir)/examples/1.html，输出所有拥有属性"href"的元素。输出格式为tag : "url" \n。
hparser解析的属性值均没有去除"号。

# 文档
预计分为三部分:hparser查询接口介绍，hparser::element_type类介绍和utf8_to_utf32/utf32_to_utf8接口介绍。

