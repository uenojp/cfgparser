#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "type.h"
#include "utils.h"

class Rule {
   private:
   public:
    const Pos X;
    const Pos Y;
    const Pos Z;

    Rule(Pos x, Pos y, Pos z);
    bool operator<(const Rule& other) const;
};

std::ostream& operator<<(std::ostream& os, const Rule& other);

class Grammer {
   private:
    bool load_rules(const std::string& filename);

   public:
    const std::set<Pos> terminals;
    const std::set<Pos> nonterminals;
    const Pos start;
    std::set<Rule> rules;

    Grammer(const std::string& filename);
};
