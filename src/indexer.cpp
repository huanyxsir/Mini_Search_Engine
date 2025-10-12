#include "indexer.h"
#include <filesystem>
#include <fstream>
#include <iostream>

Indexer::Indexer(InvertedIndex &idx, Tokenizer &tokenizer) : index(idx), tokenizer(tokenizer) {}

void Indexer::index_directory(const std::string &dir_path, const std::string &stopword_path) {
    tokenizer.load_stopwords(stopword_path);

    namespace fs = std::filesystem;
    docid_t id = 0;
    for(auto p : fs::recursive_directory_iterator(dir_path)) {
        if(!p.is_regular_file()) continue;
        std::ifstream ifs(p.path());
        if(!ifs) continue;
        std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        index_document(id, content);
        fileName.push_back(p.path().string());
        ++ id;
    }

}

void Indexer::index_document(docid_t docid, const std::string &text) {
    auto tokens = tokenizer.tokenize(text);
    int tot = 0;
    // std::cerr << "start " << docid << "\n";
    for(auto t : tokens) ++ tot, index.add_term(t, docid, tot);
    // std::cerr << "finish " << docid << "\n";
}

std::string Indexer::get_filename(docid_t docid) {return fileName[docid];}