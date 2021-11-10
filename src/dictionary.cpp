#include "dictionary.h"

Dictionary::Dictionary(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "could not open " << path << '\n';
    }
    std::string line;
    while (std::getline(file, line)) {
        auto word_pos = split(line, ' ');
        if (word_pos.size() != 2)
            continue;
        this->add(word_pos[0], to_pos(word_pos[1]));
    }
}

bool Dictionary::add(const std::string word, const Pos pos) {
    if (pos == Pos::UNKNOWN) {
        return false;
    }
    const auto [_, success] = this->dictionary.emplace(word, pos);
    return success;
}

Pos Dictionary::look_up(const std::string word) const {
    const auto it = this->dictionary.find(word);
    if (it == this->dictionary.end()) {
        return Pos::UNKNOWN;
    }
    return it->second;
}

// 読み取り専用のiteratorを返す
auto Dictionary::begin() const -> decltype(this->dictionary.cbegin()) {
    return this->dictionary.cbegin();
}

auto Dictionary::end() const -> decltype(this->dictionary.cend()) {
    return this->dictionary.cend();
}
