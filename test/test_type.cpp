#include "test.h"

int main() {
    /* test: is_terminal() */
    assert(!is_terminal(Pos::UNKNOWN));
    assert(!is_terminal(Pos::S));
    assert(!is_terminal(Pos::NP));
    assert(!is_terminal(Pos::VP));
    assert(!is_terminal(Pos::PP));
    assert(!is_terminal(Pos::SS));
    assert(is_terminal(Pos::NOUN));
    assert(is_terminal(Pos::DET));
    assert(is_terminal(Pos::VERB));
    assert(is_terminal(Pos::ADJ));
    assert(is_terminal(Pos::ADV));
    assert(is_terminal(Pos::PREP));

    /* test: is_nonterminal() */
    assert(!is_nonterminal(Pos::UNKNOWN));
    assert(is_nonterminal(Pos::S));
    assert(is_nonterminal(Pos::NP));
    assert(is_nonterminal(Pos::VP));
    assert(is_nonterminal(Pos::PP));
    assert(is_nonterminal(Pos::SS));
    assert(!is_nonterminal(Pos::NOUN));
    assert(!is_nonterminal(Pos::DET));
    assert(!is_nonterminal(Pos::VERB));
    assert(!is_nonterminal(Pos::ADJ));
    assert(!is_nonterminal(Pos::ADV));
    assert(!is_nonterminal(Pos::PREP));

    return 0;
}
