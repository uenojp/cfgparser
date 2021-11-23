#include "test.h"

int main() {
    Lexicon d("./asset/lexicon1");

    assert(d.look_up("the") == Pos::DET);
    assert(d.look_up("child") == Pos::NOUN);
    assert(d.look_up("runs") == Pos::VERB);
    assert(d.look_up("quickly") == Pos::ADV);
    assert(d.look_up("to") == Pos::PREP);
    assert(d.look_up("large") == Pos::ADJ);
    assert(d.look_up("house") == Pos::NOUN);

    assert(d.look_up("") == Pos::UNKNOWN);
    assert(d.look_up("foo") == Pos::UNKNOWN);

    return 0;
}
