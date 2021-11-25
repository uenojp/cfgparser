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

/* EntryはCKY表の各セルに入る品詞と2つの導出元のポインタを持つ
 *
 * 例えば、あるCKY表のあるセルが
 *  [NP(1,1)(2,6) PP(1,2)(3,6)  ]
 * のとき、VERB(1,1)(2,6)やVP(1,2)(3,6)それぞれがエントリ */
class Entry {
   public:
    Pos pos;
    std::pair<int, int> left;
    std::pair<int, int> right;

    Entry(){};
    Entry(Pos pos, std::pair<int, int> left, std::pair<int, int> right) : pos(pos), left(left), right(right) {}
};

std::ostream& operator<<(std::ostream& os, const Entry& entry);

/* CellはCKY表の各セルで、複数のエントリを持つ
 *
 * 例えば、以下のように複数のエントリを持つ
 *  [NP(1,1)(2,6) PP(1,2)(3,6)  ] */
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
    std::vector<std::vector<Cell>> table; /* CKY表 */

    /* CKY表を初期化する */
    void init_table(const std::vector<std::string>& words);

    /* CKY表を埋める */
    void fill_table(std::size_t len);

    /* CKY表のセル(i, j)を埋める */
    void fill_cell(std::size_t i, std::size_t j);

    /* セル(i,k)とセル(k+1,j)を結合する */
    void combine_cells(std::size_t i, std::size_t k, std::size_t j);

    /* CKY表からS式を導く */
    std::string derive_s_expression(const std::vector<std::string>& words);
    /* DFS用 */
    void derive_s_expression(const Entry& entry, std::string& s_expression);

   public:
    Grammar grammar; /* 文法 */
    Lexicon lexicon; /* 単語辞書 */

    CkyParser(const Config& config) : grammar(Grammar(config.grammar)), lexicon(Lexicon(config.lexicon)) {}

    /* 英文sentenceをパースし、正文の判定とS式の結果を返す */
    Result parse(const std::string& sentence);

    /* CKY表を出力する */
    void show_table();

    /* CKY表をエントリのポインタ含めて出力する */
    void show_table_with_detail();
};
