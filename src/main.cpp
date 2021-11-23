#include <iostream>
#include <string>

#include "cfgparser.h"

int main(int argc, char** argv) {
    /* コマンドライン引数をstd::vector<std::string>へ変換 */
    std::vector<std::string> args(argv, argv + argc);

    if (args.size() != 4) {
        std::cerr << "Usage: " << args[0] << " <grammar file> <lexicon file> <sentence>" << std::endl;
        return 1;
    }

    /* 文法規則と単語辞書のパスを取得 */
    std::string rule = args[1];
    std::string lexicon = args[2];
    std::string sentence = args[3];

    CkyParser parser(Config{
        grammar : rule,
        lexicon : lexicon,
    });

    // std::string sentence = "the child runs quickly to the large house";
    // std::string sentence = "I saw a girl with a telescope";
    std::string sentence = "";
    auto result = parser.parse(sentence);

    parser.show_table();

    if (result.ok) {
        std::cout << std::endl
                  << "\"" << sentence << "\" "
                  << "is the correct sentence." << std::endl;
    } else {
        std::cout << std::endl
                  << "\"" << sentence << "\" "
                  << "is NOT the correct sentence." << std::endl;
    }

    std::cout << std::endl << result.s_expression << std::endl;

    return 0;
}