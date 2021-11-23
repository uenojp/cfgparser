#!/bin/bash

# makeに失敗したら終了する
make || exit 1

./cfgparser ./asset/grammar1 ./asset/lexicon1 "the child runs quickly to the large house"

echo "------"

./cfgparser ./asset/grammar2 ./asset/lexicon2 "I saw a girl with a telescope"

