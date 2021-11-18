#include "cky_parser.h"

#include <string>

CkyParser::CkyParser(const Config &config) : grammer(Grammer(config.dict)), lexicon(Lexicon(config.rule)) {}
