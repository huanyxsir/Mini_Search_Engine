#include "index_generator.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

IndexGenerator::IndexGenerator(InvertedIndex &idx, Tokenizer &tokenizer) : index(idx), tokenizer(tokenizer) {}

void IndexGenerator::index_directory(const std::string &dir_path, const std::string &stopword_path) {
    tokenizer.load_stopwords(stopword_path);

    namespace fs = std::filesystem;
    for(auto p : fs::recursive_directory_iterator(dir_path)) {
        if(!p.is_regular_file()) continue;
        std::ifstream ifs(p.path());
        if(!ifs) continue;
        std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        // std::cerr << docNum << "\n";
        index_document(docNum, content);
        fileName.push_back(p.path().string());
        ++ docNum;
    }

}

void IndexGenerator::index_document(docid_t docid, const std::string &text) {
    InvertedIndex new_index;
    auto tokens = tokenizer.tokenize(text);
    
    std::unordered_map<std::string, uint32_t> tot;
    for(auto t : tokens) ++ tot[t];
    // std::cerr << "finish tokens\n";
    for (auto [key, val] : tot) new_index.add_term(key, docid, val);
    // std::cerr << "finish new_index\n";
    index.merge(new_index);
}

std::string IndexGenerator::get_filename(docid_t docid) {return fileName[docid];}