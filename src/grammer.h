#pragma once

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "type.h"
#include "utils.h"

struct Rule {
    Pos X;
    Pos Y;
    Pos Z;
};

class Grammer {
   private:
    const std::set<Pos> terminals;
    const std::set<Pos> nonterminals;
    const Pos start;
    std::vector<Rule> rules;

    bool load_rules(const std::string& filename);

   public:
    Grammer(const std::string& filename);
};
