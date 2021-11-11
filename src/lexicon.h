#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include "type.h"
#include "utils.h"

class Lexicon {
   private:
    std::unordered_map<std::string, Pos> lexicon;

   public:
    Lexicon(const std::string& filename);
    bool add(const std::string& word, const Pos& pos);
    Pos look_up(std::string word) const;

    auto begin() const -> decltype(lexicon.cbegin());
    auto end() const -> decltype(lexicon.cend());
};
