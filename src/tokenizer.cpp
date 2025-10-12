#include "tokenizer.h"
#include "porter2_stemmer.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cctype>


Tokenizer::Tokenizer() {
    /* */
}

std::string Tokenizer::trim(const std::string &w) const {
    return Porter2Stemmer::trim(w);
}

std::string Tokenizer::stem(const std::string &w) const {
    return Porter2Stemmer::stem(w);
}

std::vector<std::string> Tokenizer::tokenize(const std::string &text) const {
    std::vector<std::string> out;
    std::istringstream iss(text);
    std::string token;
    while(iss >> token) {
        std::string n = trim(token);
        if(n.empty()) continue;
        n = stem(n);
        if(stopwords_.count(n)) continue;
        out.push_back(n);
    }
    return out;
}

void Tokenizer::load_stopwords(const std::string &path) {
    std::ifstream ifs(path);
    if(!ifs) return;
    stopwords_.clear();
    std::string w;
    while(ifs >> w) stopwords_.insert(w);
}