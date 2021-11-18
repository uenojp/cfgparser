#include "grammer.h"

Rule::Rule(Pos x, Pos y, Pos z) : X(x), Y(y), Z(z) {}

bool Rule::operator<(const Rule& r) const {
    return X < r.X || (X == r.X && Y < r.Y) || (X == r.X && Y == r.Y && Z < r.Z);
}

std::ostream& operator<<(std::ostream& os, const Rule& rule) {
    os << "(" << rule.X << ", " << rule.Y << ", " << rule.Z << ")";
    return os;
}

Grammer::Grammer(const std::string& filename) : terminals(std::set<Pos>{Pos::NOUN, Pos::DET, Pos::VERB, Pos::ADJ, Pos::ADV, Pos::PREP}), nonterminals(std::set<Pos>{Pos::S, Pos::NP, Pos::VP, Pos::PP, Pos::SS}), start(Pos::S) {
    load_rules(filename);
}

bool Grammer::load_rules(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        auto rule = split(line, ' ');
        std::size_t len = rule.size();

        /* X -> a, a \in \Sigma */
        /* X -> YZ */
        if (len == 2 || len == 3) {
            Pos X = to_pos(rule[0]);
            Pos Y = to_pos(rule[1]);
            Pos Z = Pos::UNKNOWN;
            if (len == 3) {
                Z = to_pos(rule[2]);
            }
            if (X == Pos::UNKNOWN || Y == Pos::UNKNOWN || (Z == Pos::UNKNOWN && len == 3)) {
                std::cerr << "Error: invalid rule " << line << std::endl;
                return false;
            }
            this->rules.emplace(Rule{X, Y, Z});
        } else {
            std::cerr << "Error: invalid format " << line << std::endl;
            return false;
        }
    }
    return true;
}

bool Grammer::has_rule(Pos X, Pos Y, Pos Z) const {
    return this->rules.contains(Rule{X, Y, Z});
}
