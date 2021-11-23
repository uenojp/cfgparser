#pragma once

#include <set>
#include <sstream>
#include <string>
#include <vector>

/* 文字列をdelemiter区切りで分割し、std::vectorで返す */
std::vector<std::string> split(const std::string& str, char delimiter);

/* std::vectorの標準出力用に<<をオーバーロード */
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

/* std::setの標準出力用に<<をオーバーロード */
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& v);

/* std::pairの標準出力用に<<をオーバーロード */
template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p);
