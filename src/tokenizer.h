#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "trie.h"

class Tokenizer {
    public:
        Tokenizer() = default;
        ~Tokenizer() = default;
        
        std::vector<std::string> tokenize(const std::string &text);
        void load_stopwords(const std::string &path);

    private:
        Trie stopwords;
        std::string trim(const std::string &w) const;
        std::string stem(const std::string &w) const;
};