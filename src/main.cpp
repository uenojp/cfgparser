#include <iostream>
#include <string>

#include "cfgparser.h"

int main() {
    CkyParser p(Config{
        dict : "./asset/dict1",
        rule : "./asset/rule1",
    });

    return 0;
}