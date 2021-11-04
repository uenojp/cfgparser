#include "type.h"

#include <iostream>

std::ostream &operator<<(std::ostream &os, Entry e) {
    switch (e) {
        case Entry::UNKNOWN: os << "UNKNOWN"; break;
        case Entry::S: os << "S"; break;
        case Entry::NP: os << "NP"; break;
        case Entry::VP: os << "VP"; break;
        case Entry::NOUN: os << "NOUN"; break;
        case Entry::DET: os << "DET"; break;
        case Entry::VERB: os << "VERB"; break;
        case Entry::ADJ: os << "ADJ"; break;
        case Entry::ADV: os << "ADV"; break;
        case Entry::PREP: os << "PREP"; break;
    }
    return os;
}