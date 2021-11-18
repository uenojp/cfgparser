#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "grammer.h"
#include "lexicon.h"
#include "type.h"

class Entry {
   public:
    Pos pos;
    std::pair<uint32_t, uint32_t> backpointers;
};

class Cell {
   public:
    std::set<Entry> entries;
};

struct Config {
    std::string dict;
    std::string rule;
};

class CkyParser {
   private:
    std::vector<std::vector<std::set<Cell>>> table;

   public:
    Grammer grammer;
    Lexicon lexicon;

    CkyParser(const Config &confg);
    bool parse(const std::string &sentence);
};
