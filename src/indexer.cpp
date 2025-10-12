#include "indexer.h"
#include <filesystem>
#include <fstream>
#include <iostream>

Indexer::Indexer(InvertedIndex &idx) : index_(idx) {}

void Indexer::index_directory(const std::string &dir_path) {
    namespace fs = std::filesystem;
    docid_t id = 1;
    for(auto &p : fs::directory_iterator(dir_path)) {
        if(!p.is_regular_file()) continue;
        std::ifstream ifs(p.path());
        if(!ifs) continue;
        std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        index_document(id, content);
        ++id;
    }
}

void Indexer::index_document(docid_t docid, const std::string &text) {
    auto tokens = tokenizer_.tokenize(text);
    for(const auto &t : tokens) index_.add_term(t, docid);
}
