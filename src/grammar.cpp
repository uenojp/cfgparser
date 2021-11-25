#include "grammar.h"

bool Rule::operator<(const Rule& r) const {
    return X < r.X || (X == r.X && Y < r.Y) || (X == r.X && Y == r.Y && Z < r.Z);
}

std::ostream& operator<<(std::ostream& os, const Rule& rule) {
    os << "(" << rule.X << ", " << rule.Y << ", " << rule.Z << ")";
    return os;
}

Grammar::Grammar(const std::string& filename) : start(Pos::S), terminals(std::set<Pos>{Pos::NOUN, Pos::DET, Pos::VERB, Pos::ADJ, Pos::ADV, Pos::PREP}), nonterminals(std::set<Pos>{Pos::S, Pos::NP, Pos::VP, Pos::PP, Pos::SS}) {
    load(filename);
}

bool Grammar::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }

    /* ファイルから一行ずつ読み込み、遷移規則を文法として追加 */
    std::string line;
    while (std::getline(file, line)) {
        /* ファイルから読み込んだ
         *  <品詞> <品詞> <品詞>
         * もしくは
         *  <品詞> <品詞>
         * を空白でsplitする */
        const auto rule = split(line, ' ');
        const std::size_t len = rule.size();

        if (len == 2 || len == 3) {
            Pos X = to_pos(rule[0]);
            Pos Y = to_pos(rule[1]);
            Pos Z = Pos::UNKNOWN; /* 品詞が2列のみ(X -> Y)の場合ZはPos::UNKNOWN */
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

bool Grammar::has_rule(Pos X, Pos Y, Pos Z) const {
    return this->rules.contains(Rule{X, Y, Z});
}
