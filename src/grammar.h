#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "type.h"
#include "utils.h"

/* 正規文法における X -> Y という遷移規則 */
class Rule {
   private:
   public:
    const Pos X;
    const Pos Y;
    const Pos Z;

    Rule(Pos x, Pos y, Pos z) : X(x), Y(y), Z(z) {}

    /* 順序を定義(std::setで扱うため) */
    bool operator<(const Rule& other) const;
};

/* Ruleの標準出力用に<<をオーバーロード */
std::ostream& operator<<(std::ostream& os, const Rule& other);

/* 文脈自由文法 */
class Grammar {
   private:
    /* 文法規則をファイルからロード */
    bool load(const std::string& filename);

   public:
    const Pos start;                  /*初期記号 */
    const std::set<Pos> terminals;    /* 終端記号 */
    const std::set<Pos> nonterminals; /*非終端記号 */
    std::set<Rule> rules;

    Grammar(const std::string& filename);

    /* X -> YZという文法規則の存在判定 */
    bool has_rule(Pos X, Pos Y, Pos Z) const;
};
