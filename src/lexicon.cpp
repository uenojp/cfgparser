#include "lexicon.h"

Lexicon::Lexicon(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "could not open " << filename << std::endl;
    }
    /* ファイルから一行ずつ読み込み、単語と品詞を単語辞書へ登録 */
    std::string line;
    while (std::getline(file, line)) {
        /* <WORD> <POS> を空白でsplitする */
        auto word_pos = split(line, ' ');
        if (word_pos.size() != 2)
            continue;
        Pos pos = to_pos(word_pos[1]);
        if (pos == Pos::UNKNOWN)
            continue;
        this->add(word_pos[0], pos);
    }
}

bool Lexicon::add(const std::string& word, const Pos& pos) {
    if (pos == Pos::UNKNOWN) {
        return false;
    }
    const auto [_, success] = this->lexicon.emplace(word, pos);
    return success;
}

Pos Lexicon::look_up(const std::string& word) const {
    const auto it = this->lexicon.find(word);
    /* 単語wordの品詞が単語辞書になければPos::UNKNOWNを返す */
    if (it == this->lexicon.end()) {
        return Pos::UNKNOWN;
    }
    return it->second;
}

auto Lexicon::begin() const -> decltype(this->lexicon.cbegin()) {
    return this->lexicon.cbegin();
}

auto Lexicon::end() const -> decltype(this->lexicon.cend()) {
    return this->lexicon.cend();
}
