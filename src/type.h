#pragma once

#include <iostream>

enum class Pos {
    UNKNOWN,
    /* 品詞句 */
    S,
    NP,
    VP,
    PP,
    SS, /* 中間表現 */
    /* 品詞 */
    NOUN,
    DET,
    VERB,
    ADJ,
    ADV,
    PREP,
};

/* 品詞を表す文字列を返す */
std::string to_string(const Pos& pos);

/* 品詞を表す文字列から品詞を返す */
Pos to_pos(const std::string& str);

/* Posの標準出力用に<<をオーバーロード */
std::ostream& operator<<(std::ostream& os, const Pos& pos);

/* 終端記号の判定 */
bool is_terminal(const Pos& pos);

/* 非終端記号の判定 */
bool is_nonterminal(const Pos& pos);
