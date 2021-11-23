#include "cky_parser.h"

#include <iomanip>

Result CkyParser::parse(const std::string &sentence) {
    if (sentence.empty()) {
        return Result{ok : false, s_expression : ""};
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
                              return e.pos == this->grammar.start;
                          }) != entries.end();

    return Result{ok : is_grammatical, s_expression : derive_s_expression(words)};
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
        for (const auto rule : this->grammar.rules) {
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
            for (const auto &X : this->grammar.nonterminals) {
                if (this->grammar.has_rule(X, Y, Z)) {
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

/* modelの中の文字列tagを後ろからreplacementsの要素に徐々に置き換えていく
 * 例えば、
 *  model        : "{{tag}} foo {{tag}} {{tag}} bar {{tag}}"
 *  tag          : "{{tag}}"
 *  replacements : {"one", "two", "three"}
 * のとき
 *  result : "{{tag}} foo one two bar three"  */
std::string sequential_replace_from_tail(const std::string &model, const std::string &tag, const std::vector<std::string> &replacements) {
    std::string result = model;

    // replacementsの末尾から走査するためのイテレータ
    auto it = replacements.rbegin();

    std::size_t found = model.rfind(tag);
    if (found == std::string::npos) {
        return result;
    }

    while (found != std::string::npos) {
        result.replace(found, tag.size(), *it);
        it++;
        found = result.rfind(tag, found);
    }

    return result;
}

std::string CkyParser::derive_s_expression(const std::vector<std::string> &words) {
    // 上三角行列の最右、最上のセルを取得
    auto cell = this->table[0][words.size() - 1];
    // cellの中から、エントリにPos::Sがあるものを探す
    auto start_entry_it = std::find_if(cell.entries.begin(), cell.entries.end(), [&](Entry e) {
        return e.pos == this->grammar.start;
    });
    if (start_entry_it == cell.entries.end()) {
        return "";
    }

    std::string s_expression;
    derive_s_expression(*start_entry_it, s_expression);

    s_expression = "(S" + s_expression + ")";

    return sequential_replace_from_tail(s_expression, "{{word}}", words);
}

/*
 *             S
 *            / \
 *          NP  VP
 *         / \    \
 *        DT  N    V
 *       the child runs
 *
 * S(NP(DT(the)
 *      N(child))
 *   VP(V(runs)))
 */

void CkyParser::derive_s_expression(Entry &entry, std::string &s_expression) {
    if (entry.left.first == -1) {
        s_expression += " {{word}}";
        return;
    }

    Entry left;
    Entry right;
    try {
        left = this->table[entry.left.first][entry.left.second].entries.at(0);
        right = this->table[entry.right.first][entry.right.second].entries.at(0);

    } catch (const std::exception &e) { std::cerr << "unreachable" << e.what() << std::endl; }

    s_expression += "(";
    s_expression += to_string(left.pos);
    derive_s_expression(left, s_expression);
    s_expression += ")";

    s_expression += "(";
    s_expression += to_string(right.pos);
    derive_s_expression(right, s_expression);
    s_expression += ")";
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
