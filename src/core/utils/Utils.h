#pragma once

#include <string>
#include <algorithm>
#include <cctype>

namespace Utils {

    // 去除字符串首尾空格
    std::string trim(const std::string& str);

    // 将字符串转换为小写
    std::string toLower(const std::string& str);

    // 将字符串转换为大写
    std::string toUpper(const std::string& str);

    // 判断字符串是否以指定字符串开头
    bool startsWith(const std::string& str, const std::string& prefix);

    // 判断字符串是否以指定字符串结尾
    bool endsWith(const std::string& str, const std::string& suffix);

} // namespace Utils