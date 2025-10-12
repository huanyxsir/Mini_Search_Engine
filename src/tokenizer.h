#pragma once
#include <string>
#include <vector>
#include <unordered_set>

class Tokenizer {
    public:
        Tokenizer();
        std::vector<std::string> tokenize(const std::string &text) const;
        void load_stopwords(const std::string &path);

    private:
        std::unordered_set<std::string> stopwords_;
        std::string trim(const std::string &w) const;
        std::string stem(const std::string &w) const;
};