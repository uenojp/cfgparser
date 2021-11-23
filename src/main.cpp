#include <iostream>
#include <string>

#include "cfgparser.h"

int main() {
    CkyParser parser(Config{
        dict : "./asset/dict2",
        rule : "./asset/rule2",
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