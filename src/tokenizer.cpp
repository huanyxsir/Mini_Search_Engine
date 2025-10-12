#include "tokenizer.h"
#include "porter2_stemmer.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <iostream>

std::string Tokenizer::trim(const std::string &w) const {
    return Porter2Stemmer::trim(w);
}

std::string Tokenizer::stem(const std::string &w) const {
    return Porter2Stemmer::stem(w);
}

std::vector<std::string> Tokenizer::tokenize(const std::string &text) {
    std::vector<std::string> out;
    std::istringstream iss(text);
    std::string token;
    while(iss >> token) {
        std::string _token = stem(trim(token));
        if(_token.empty()) continue;
        if(stopwords.query(_token)) continue;
        out.push_back(_token);
    }
    return out;
}

void Tokenizer::load_stopwords(const std::string &path) {
    std::ifstream ifs(path);
    if(!ifs) return;
    std::string w;
    while(ifs >> w) stopwords.add(stem(trim(w)), 1);
}