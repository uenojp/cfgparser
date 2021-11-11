#include "grammer.h"

using enum Pos;
Grammer::Grammer(const std::string& filename) : terminals(std::set<Pos>{NOUN, DET, VERB, ADJ, ADV, PREP}), nonterminals(std::set<Pos>{S, NP, VP, PP, SS}), start(S) {
    load_rules(filename);
}

bool Grammer::load_rules(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }

    std::fstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        auto rule = split(line, ' ');
        std::size_t len = rule.size();

        if (len == 2) { /* X -> a, a \in \Sigma */
            // TODO: 直接終端文字列を指す規則の処理
        } else if (len == 3) { /* X -> YZ */
            Pos X = to_pos(rule[0]);
            Pos Y = to_pos(rule[1]);
            Pos Z = to_pos(rule[2]);
            if (X == UNKNOWN || Y == UNKNOWN || Z == UNKNOWN) {
                std::cerr << "Error: invalid rule " << line << std::endl;
                return false;
            }
            this->rules.push_back(Rule{X, Y, Z});
        } else {
            std::cerr << "Error: invalid format " << line << std::endl;
        }
    }
}
