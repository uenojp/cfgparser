#include "test.h"

int main() {
    Grammar g("./asset/grammar1");

    assert(g.rules.contains(Rule{Pos::S, Pos::NP, Pos::VP}));
    assert(g.rules.contains(Rule{Pos::NP, Pos::DET, Pos::NOUN}));
    assert(g.rules.contains(Rule{Pos::NP, Pos::DET, Pos::NP}));
    assert(g.rules.contains(Rule{Pos::NP, Pos::PREP, Pos::NP}));
    assert(g.rules.contains(Rule{Pos::NP, Pos::ADJ, Pos::NOUN}));
    assert(g.rules.contains(Rule{Pos::VP, Pos::VERB, Pos::NP}));
    assert(g.rules.contains(Rule{Pos::VP, Pos::VP, Pos::NP}));
    assert(g.rules.contains(Rule{Pos::VP, Pos::VERB, Pos::ADV}));
    assert(g.rules.contains(Rule{Pos::VP, Pos::VERB, Pos::UNKNOWN}));

    assert(!g.rules.contains(Rule{Pos::NP, Pos::NP, Pos::NP}));
    assert(!g.rules.contains(Rule{Pos::UNKNOWN, Pos::UNKNOWN, Pos::UNKNOWN}));

    return 0;
}
