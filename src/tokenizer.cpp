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
        if(stopwords.find(_token) != stopwords.end()) continue;
        out.push_back(_token);
    }
    return out;
}

void Tokenizer::load_stopwords(const std::string &path) {
    std::ifstream isf(path);
    if(!isf) return;
    // std::cerr << "load\n";
    std::string w;
    while(isf >> w) stopwords.insert(stem(trim(w)));
}