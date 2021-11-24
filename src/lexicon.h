#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include "type.h"
#include "utils.h"

class Lexicon {
   private:
    /* 単語辞書 */
    std::unordered_map<std::string, Pos> lexicon;

    /* 文法規則をファイルからロード */
    bool load(const std::string& filename);

   public:
    Lexicon(const std::string& filename);

    /* 単語wordの品詞posを辞書に登録する */
    bool add(const std::string& word, const Pos& pos);

    /* 単語wordの品詞を返す */
    Pos look_up(const std::string& word) const;

    /* 読み取り専用のiteratorを返す */
    auto begin() const -> decltype(lexicon.cbegin());
    auto end() const -> decltype(lexicon.cend());
};
