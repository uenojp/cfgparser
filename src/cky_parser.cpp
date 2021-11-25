#include "cky_parser.h"

/* 英文sentenceをCKYアルゴリズムで解析し、解析結果をS式として返す
 * ref. https://courses.engr.illinois.edu/cs447/fa2018/Slides/Lecture09.pdf */
Result CkyParser::parse(const std::string &sentence) {
    if (sentence.empty()) {
        return Result{ok : false, s_expression : ""};
    }

    const auto words = split(sentence, ' ');
    const std::size_t len = words.size();

    init_table(words);
    fill_table(len);

    const auto &entries = this->table[0][len - 1].entries;
    // 最右上にPos::Sがあれば正文、そうでなければ非文
    const bool is_grammatical = std::find_if(entries.begin(), entries.end(), [&](Entry e) {
                                    return e.pos == this->grammar.start;
                                }) != entries.end();
    // S式を導出する
    const std::string s_expression = is_grammatical ? derive_s_expression(words) : "";

    return Result{ok : is_grammatical, s_expression : s_expression};
}

/* CKY表を初期化する
 * 1. 対角線に各単語の品詞をエントリとしてをセルに追加する
 * 例えば、文が"the child runs"でthe DET, child NOUN, runs VERBが辞書にあるとすると、
 *  the   child runs
 *  [DET ][    ][    ]
 *  [    ][NOUN][    ]
 *  [    ][    ][VERB]
 * のようにセルに追加する(ポインタは(-1,-1)とする)
 *
 * 2. 仮に規則に狭義の品詞(NOUN,VERBなど)から品詞句直ちに導ける規則があるなら、その品詞句もセルに追加する
 * 例えば、VP -> VERBという規則があるとすると、VERBからVPは直ちに導出できるため、
 *  the   child runs
 *  [DET ][    ][       ]
 *  [    ][NOUN][       ]
 *  [    ][    ][VERB VP]
 * とする */
void CkyParser::init_table(const std::vector<std::string> &words) {
    const std::size_t len = words.size();

    /* 文によってCKY表のサイズは異なるため、(文の単語数)x(文の単語数)にリサイズ */
    this->table.resize(len);
    for (auto &row : this->table) {
        row.clear();
        row.resize(len);
    }

    /* 対角線に単語の品詞を追加する */
    for (std::size_t i = 0; const auto &word : words) {
        Entry entry(this->lexicon.look_up(word), std::make_pair(-1, -1), std::make_pair(-1, -1));
        this->table[i][i].entries.push_back(entry);
        i++;
    }

    /* 対角線に狭義の品詞から品詞句直ちに導ける規則があるなら、その品詞句もセルに追加する */
    for (std::size_t i = 0; i < len; i++) {
        // TODO: entriesのエラーチェック
        const Pos pos = this->table[i][i].entries[0].pos;
        for (const auto rule : this->grammar.rules) {
            /* X -> YでYが狭義の品詞(NOUN,VERBなど)のとき、Xを追加
             * 例えば、VP -> VERBのとき、VERBのセルにVPを追加 */
            if (is_nonterminal(rule.X) && is_terminal(rule.Y) && rule.Z == Pos::UNKNOWN) {
                if (rule.Y == pos) {
                    Entry entry(rule.X, std::make_pair(-1, -1), std::make_pair(-1, -1));
                    this->table[i][i].entries.push_back(entry);
                }
            }
        }
    }
}

/* CKY表を以下の順で走査しセルを埋めていく
 * [1  ][6  ][10 ][13 ][15 ]
 * [   ][2  ][7  ][11 ][14 ]
 * [   ][   ][3  ][8  ][12 ]
 * [   ][   ][   ][4  ][9  ]
 * [   ][   ][   ][   ][5  ] */
void CkyParser::fill_table(std::size_t len) {
    for (std::size_t span = 1; span < len; span++) {
        for (std::size_t i = 0; i < len - span; i++) {
            fill_cell(i, i + span);
        }
    }
}

/* セル(i,j)において、以下のように左のセルと下のセルを組み合わせる
 * 同じマークのセルを組み合わせる
 *                         j
 *   [   ][   ][   ][   ][   ]
 * i [   ][ ! ][ $ ][ # ][   ]
 *   [   ][   ][   ][   ][ ! ]
 *   [   ][   ][   ][   ][ $ ]
 *   [   ][   ][   ][   ][ # ] */
void CkyParser::fill_cell(std::size_t i, std::size_t j) {
    for (std::size_t k = i; k < j; k++) {
        combine_cells(i, k, j);
    }
}

/* CKY表のセル(i,k)の品詞Yとセル(k+1,j)の品詞Yから導出できる規則X(X -> YZ)が存在すれば、
 * セル(i,j)にXを追加する
 * 例えば、NP -> ADJ NOUNという規則が存在するときセル(i,j)にNPを追加する
 *         k             j
 *  i ...[ADJ ][   ]...[NP  ]
 *                      ...
 * k+1                 [NOUN]
 *                      ... */
void CkyParser::combine_cells(std::size_t i, std::size_t k, std::size_t j) {
    for (const auto &[Y, _l, _r] : this->table[i][k].entries) {
        for (const auto &[Z, _l, _r] : this->table[k + 1][j].entries) {
            for (const auto &X : this->grammar.nonterminals) {
                if (this->grammar.has_rule(X, Y, Z)) {
                    /* CKY表を埋めた後、右上からたどってS式を導けるよう、セルのインデックスを持たせる */
                    Entry entry(X, std::make_pair(i, k), std::make_pair(k + 1, j));
                    this->table[i][j].entries.push_back(entry);
                }
            }
        }
    }
}

/* CKY表を表示(品詞のみ) */
void CkyParser::show_table() {
    const int cell_width = 8; /* セルの表示幅 */
    for (const auto &row : this->table) {
        for (const auto &cell : row) {
            bool first = true;
            std::string entries;

            std::cout << std::left << "[";
            for (const auto &entry : cell.entries) {
                entries += (first ? "" : " ") + to_string(entry.pos);
                first = false;
            }
            std::cout << entries;
            /* 表示幅に満たない場合、空白で埋める */
            if (int(entries.size()) <= cell_width) {
                std::cout << std::right << std::setw(cell_width - entries.size());
            }
            std::cout << "]";
        }
        std::cout << std::endl;
    }
}

/* CKY表を表示(品詞とポインタ) */
void CkyParser::show_table_with_detail() {
    for (const auto &row : this->table) {
        for (const auto &cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
}

/* modelの中の文字列tagを後ろからreplacementsの要素に徐々に置き換える
 * S式の導出で使用する
 * 例えば、
 *  model        : "{{tag}} foo {{tag}} {{tag}} bar {{tag}}"
 *  tag          : "{{tag}}"
 *  replacements : {"one", "two", "three"}
 * のとき
 *  result : "{{tag}} foo one two bar three"  */
std::string sequential_replace_from_tail(const std::string &model, const std::string &tag, const std::vector<std::string> &replacements) {
    std::string result = model;

    auto it = replacements.rbegin(); /* replacementsの末尾から走査するためのイテレータ */

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
    /* 上三角行列の右上のセルを取得 */
    const auto cell = this->table[0][words.size() - 1];
    /* cellの中から、Pos::Sを持つエントリを探す */
    const auto start_entry_it = std::find_if(cell.entries.begin(), cell.entries.end(), [&](Entry e) {
        return e.pos == this->grammar.start;
    });
    /* 右上のセルにPos:Sがなければ、正文ではないので空の文字列を返す */
    if (start_entry_it == cell.entries.end()) {
        return "";
    }

    std::string s_expression;
    /* Pos::SからDFS */
    derive_s_expression(*start_entry_it, s_expression);

    s_expression = "(S" + s_expression + ")";

    /* {{word}}を与えられた文字に変換して返す */
    return sequential_replace_from_tail(s_expression, "{{word}}", words);
}

/* 与えられたエントリからDFSをしてS式を導出する
 * 例えば、
 *             S
 *            / \
 *          NP  VP
 *         / \    \
 *        DT  N    V
 *       the child runs
 * のとき、
 * S(NP(DT(the)
 *      N(child))
 *   VP(V(runs)))
 * となる(ここでは見やすいよう改行を入れている)
 */
void CkyParser::derive_s_expression(const Entry &entry, std::string &s_expression) {
    if (entry.left.first == -1) {
        s_expression += " {{word}}";
        return;
    }

    // TODO: 境界の処理
    const Entry left = this->table[entry.left.first][entry.left.second].entries[0];
    const Entry right = this->table[entry.right.first][entry.right.second].entries[0];

    /* 左の部分木を走査 */
    s_expression += "(";
    s_expression += to_string(left.pos);
    derive_s_expression(left, s_expression);
    s_expression += ")";

    /* 右の部分木を走査 */
    s_expression += "(";
    s_expression += to_string(right.pos);
    derive_s_expression(right, s_expression);
    s_expression += ")";
}

std::ostream &operator<<(std::ostream &os, const Cell &cell) {
    /* Cellを[NOUN(2,3) VP(2,3)]のフォーマットで表示 */
    const int cell_width = 38; /* セルの表示幅 */
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
    /* 表示幅に満たない場合、空白で埋める */
    if (int(entries.size()) <= cell_width) {
        os << std::right << std::setw(cell_width - entries.size());
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
