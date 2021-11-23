#include "cky_parser.h"

#include <iomanip>

bool CkyParser::parse(const std::string &sentence) {
    if (sentence.empty()) {
        return false;
    }

    const auto words = split(sentence, ' ');
    std::size_t len = words.size();

    this->table.resize(len);
    for (auto &row : this->table) {
        row.clear();
        row.resize(len);
    }

    init_table(words);
    fill_table(len);

    const auto &entries = this->table[0][len - 1].entries;
    // 最右上に`Pos::S`があれば正文、そうでなければ非文
    bool is_grammatical = std::find_if(entries.begin(), entries.end(), [&](Entry e) {
                              return e.pos == this->grammer.start;
                          }) != entries.end();

    return is_grammatical;
}

void CkyParser::init_table(const std::vector<std::string> &words) {
    std::size_t i;

    i = 0;
    for (const auto &word : words) {
        Entry entry(this->lexicon.look_up(word), std::make_pair(-1, -1), std::make_pair(-1, -1));
        this->table[i][i].entries.push_back(entry);
        i++;
    }

    for (i = 0; i < words.size(); i++) {
        // TODO: entriesのエラーチェック
        Pos pos = this->table[i][i].entries[0].pos;
        for (const auto rule : this->grammer.rules) {
            // X -> YでYが狭義の品詞(NOUNとかVERBとか)のとき、Xを追加
            // 例えば、VP -> VERBのとき、VERBのセルにVPを追加
            if (is_nonterminal(rule.X) && is_terminal(rule.Y) && rule.Z == Pos::UNKNOWN) {
                if (rule.Y == pos) {
                    Entry entry(rule.X, std::make_pair(-1, -1), std::make_pair(-1, -1));
                    this->table[i][i].entries.push_back(entry);
                }
            }
        }
    }
}

void CkyParser::fill_table(std::size_t len) {
    for (std::size_t span = 1; span < len; span++) {
        for (std::size_t i = 0; i < len - span; i++) {
            fill_cell(i, i + span);
        }
    }
}

void CkyParser::fill_cell(int i, int j) {
    for (int k = i; k < j; k++) {
        combine_cells(i, k, j);
    }
}

void CkyParser::combine_cells(int i, int k, int j) {
    for (const auto &[Y, _l, _r] : this->table[i][k].entries) {
        for (const auto &[Z, _l, _r] : this->table[k + 1][j].entries) {
            for (const auto &X : this->grammer.nonterminals) {
                if (this->grammer.has_rule(X, Y, Z)) {
                    Entry entry(X, std::make_pair(i, k), std::make_pair(k + 1, j));
                    this->table[i][j].entries.push_back(entry);
                }
            }
        }
    }
}

void CkyParser::show_table() {
    for (const auto &row : this->table) {
        for (const auto &cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
}

std::ostream &operator<<(std::ostream &os, const Cell &cell) {
    int max_width = 38;
    os << std::left << "[";
    bool first = true;
    std::string entries;
    for (const auto &entry : cell.entries) {
        entries += (first ? "" : " ") + to_string(entry.pos);
        entries += "(" + std::to_string(entry.left.first) + "," + std::to_string(entry.left.second) + ")";
        entries += "(" + std::to_string(entry.right.first) + "," + std::to_string(entry.right.second) + ")";
        first = false;
    }
    os << entries;
    if (int(entries.size()) <= max_width) {
        os << std::right << std::setw(max_width - entries.size());
    }
    os << "]";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Entry &entry) {
    std::string left = "(" + std::to_string(entry.left.first) + "," + std::to_string(entry.left.second) + ")";
    std::string right = "(" + std::to_string(entry.right.first) + "," + std::to_string(entry.right.second) + ")";
    os << "(" << to_string(entry.pos) << "," << left << right << ")";
    return os;
}
