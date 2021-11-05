#pragma once

#include <iostream>

enum class Pos {
    UNKNOWN,
    /* 品詞句 */
    S,
    NP,
    VP,
    PP,
    SS,  // 中間表現
    /* 品詞 */
    NOUN,
    DET,
    VERB,
    ADJ,
    ADV,
    PREP,
};

std::string to_string(const Pos& pos);

Pos to_pos(const std::string& str);

std::ostream& operator<<(std::ostream& os, const Pos& pos);

/* 終端記号の判定 */
bool is_terminal(const Pos& pos);

/* 非終端記号の判定 */
bool is_nonterminal(const Pos& pos);
