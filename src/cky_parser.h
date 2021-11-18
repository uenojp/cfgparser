#pragma once

#include <iomanip>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "grammer.h"
#include "lexicon.h"
#include "type.h"
#include "utils.h"

class Entry {
   public:
    Pos pos;
    std::pair<int, int> backpointers;
    Entry(Pos pos, std::pair<int, int> backpointers) : pos(pos), backpointers(backpointers) {}
};

std::ostream& operator<<(std::ostream& os, const Entry& entry);

class Cell {
   public:
    std::vector<Entry> entries;
    Cell(){};
    Cell(std::vector<Entry> entries) : entries(entries){};
};

std::ostream& operator<<(std::ostream& os, const Cell& cell);

struct Config {
    std::string dict;
    std::string rule;
};

class CkyParser {
   private:
    std::vector<std::vector<Cell>> table;

    void init_table(const std::vector<std::string>& words);
    void fill_table(std::size_t);
    void fill_cell(int i, int j);
    void combine_cells(int i, int j, int k);

   public:
    Grammer grammer;
    Lexicon lexicon;

    CkyParser(const Config& config) : grammer(Grammer(config.rule)), lexicon(Lexicon(config.dict)) {}
    bool parse(const std::string& sentence);
    void show_table();
};
