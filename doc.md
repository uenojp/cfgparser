## class GkyParser
- Memberes
  - table: `std::vector<std::vector<<Cell>>`
  - grammer: `Grammer`
  - lexcon: `Lexcon`
- Methods
  - `bool parse(const std::string&)`
  - `void show_s_expression()`

## class Grammer
- Member
  - terminals: `std::set<Pos>`
  - nonterminals: `std::set<Pos>`
  - start: `Pos`
  - rules: `std::vector<Rule>`

## class Cell
- Member
  - entries: `std::vector<Entry>`

## class Entry
- pos: `Pos`
- left: `std::pair<int, int>`
- right: `std::pair<int, int>`

## struct Rule
- X: `Pos`
- Y: `Pos`
- Z: `Pos`

## enum Pos
- Enumralter-list
  - UNKNOWN
  - S
  - NP
  - VP
  - PP
  - SS
  - NOUN
  - DET
  - VERB
  - ADJ
  - ADV
  - PREP

## class Lexicon
- `std::unordered_map<std::string, Pos>`
