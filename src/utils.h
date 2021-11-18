#pragma once

#include <set>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, char delimiter);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& v);

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U> p);