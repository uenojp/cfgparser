#pragma once

#include <iostream>

enum class Entry {
    UNKNOWN,
    S,
    NP,
    VP,
    NOUN,
    DET,
    VERB,
    ADJ,
    ADV,
    PREP,
};

std::ostream &operator<<(std::ostream &os, Entry e);
