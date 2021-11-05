#include "type.h"

#include <iostream>

std::string to_string(const Pos& pos) {
    switch (pos) {
        case Pos::S:    return "S";
        case Pos::NP:   return "NP";
        case Pos::VP:   return "VP";
        case Pos::PP:   return "PP";
        case Pos::SS:   return "SS";
        case Pos::NOUN: return "NOUN";
        case Pos::DET:  return "DET";
        case Pos::VERB: return "VERB";
        case Pos::ADJ:  return "ADJ";
        case Pos::ADV:  return "ADV";
        case Pos::PREP: return "PREP";
        default: return "UNKNOWN";
    }
}

Pos to_pos(const std::string& pos) {
    if (pos == "S")    return Pos::S;
    if (pos == "NP")   return Pos::NP;
    if (pos == "VP")   return Pos::VP;
    if (pos == "PP")   return Pos::PP;
    if (pos == "SS")   return Pos::SS;
    if (pos == "NOUN") return Pos::NOUN;
    if (pos == "DET")  return Pos::DET;
    if (pos == "VERB") return Pos::VERB;
    if (pos == "ADJ")  return Pos::ADJ;
    if (pos == "ADV")  return Pos::ADV;
    if (pos == "PREP") return Pos::PREP;
    return Pos::UNKNOWN;
}

bool is_terminal(const Pos& pos) {
    return pos == Pos::NOUN || pos == Pos::DET || pos == Pos::VERB || pos == Pos::ADJ || pos == Pos::ADV || pos == Pos::PREP;
}

bool is_nonterminal(const Pos& pos) {
    return pos == Pos::S || pos == Pos::NP || pos == Pos::VP || pos == Pos::PP || pos == Pos::SS;
}

std::ostream& operator<<(std::ostream& os, const Pos& pos) {
    os << to_string(pos);
    return os;
}
