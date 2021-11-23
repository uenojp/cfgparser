#include <iostream>
#include <string>

#include "cfgparser.h"

int main() {
    CkyParser parser(Config{
        dict : "./asset/dict2",
        rule : "./asset/rule2",
    });

    parser.parse("I saw a girl with a telescope");
    parser.show_table();

    return 0;
}