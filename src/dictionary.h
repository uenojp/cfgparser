#pragma once

#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "type.h"
#include "utils.h"

class Dictionary {
   private:
    std::unordered_map<std::string, Pos> dictionary;

   public:
    Dictionary(const std::string& path);
    bool add(const std::string word, const Pos pos);
    Pos look_up(const std::string word) const;

    auto begin() const -> decltype(dictionary.cbegin());
    auto end() const -> decltype(dictionary.cend());
};
