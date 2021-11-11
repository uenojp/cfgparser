#include "lexicon.h"

Lexicon::Lexicon(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "could not open " << filename << '\n';
    }
    std::string line;
    while (std::getline(file, line)) {
        auto word_pos = split(line, ' ');
        if (word_pos.size() != 2)
            continue;
        this->add(word_pos[0], to_pos(word_pos[1]));
    }
}

bool Lexicon::add(const std::string& word, const Pos& pos) {
    if (pos == Pos::UNKNOWN) {
        return false;
    }
    const auto [_, success] = this->lexicon.emplace(word, pos);
    return success;
}

Pos Lexicon::look_up(const std::string word) const {
    const auto it = this->lexicon.find(word);
    if (it == this->lexicon.end()) {
        return Pos::UNKNOWN;
    }
    return it->second;
}

// 読み取り専用のiteratorを返す
auto Lexicon::begin() const -> decltype(this->lexicon.cbegin()) {
    return this->lexicon.cbegin();
}

auto Lexicon::end() const -> decltype(this->lexicon.cend()) {
    return this->lexicon.cend();
}
