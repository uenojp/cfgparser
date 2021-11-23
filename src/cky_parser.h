#pragma once

#include <iomanip>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "grammar.h"
#include "lexicon.h"
#include "type.h"
#include "utils.h"

class Entry {
   public:
    Pos pos;
    std::pair<int, int> left;
    std::pair<int, int> right;

    Entry(){};
    Entry(Pos pos, std::pair<int, int> left, std::pair<int, int> right) : pos(pos), left(left), right(right) {}
};

std::ostream& operator<<(std::ostream& os, const Entry& entry);

class Cell {
   public:
    std::vector<Entry> entries;

    Cell(){};
    Cell(std::vector<Entry> entries) : entries(entries) {}
};

std::ostream& operator<<(std::ostream& os, const Cell& cell);

/* パーザ用の設定データ(文法規則と単語辞書のパス) */
struct Config {
    std::string grammar;
    std::string lexicon;
};

/* パース結果 */
struct Result {
    bool ok;                  /* 正文の判定 */
    std::string s_expression; /* S式 */
};

class CkyParser {
   private:
    std::vector<std::vector<Cell>> table;

    void init_table(const std::vector<std::string>& words);
    void fill_table(std::size_t len);
    void fill_cell(int i, int j);
    void combine_cells(int i, int j, int k);

    std::string derive_s_expression(const std::vector<std::string>& words);
    void derive_s_expression(Entry& entry, std::string& s_expression);

   public:
    Grammar grammar;
    Lexicon lexicon;

    CkyParser(const Config& config) : grammar(Grammar(config.grammar)), lexicon(Lexicon(config.lexicon)) {}
    Result parse(const std::string& sentence);
    void show_table();
};
