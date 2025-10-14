#include "index_generator.h"
#include "IObit.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

IndexGenerator::IndexGenerator(InvertedIndex &idx, Tokenizer &tokenizer) : index(idx), tokenizer(tokenizer) {}

void IndexGenerator::index_directory(const std::string &dir_path) {
    namespace fs = std::filesystem;
    for(auto p : fs::recursive_directory_iterator(dir_path)) {
        if(!p.is_regular_file()) continue;
        std::ifstream ifs(p.path());
        if(!ifs) continue;
        auto it = std::find(fileName.begin(), fileName.end(), p.path().string());
        if (it != fileName.end()) continue;
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

void IndexGenerator::save(const std::string &path) {
    std::ofstream ofs(path, std::ios::binary);
    if(!ofs) {
        std::cerr << "Cannot open file : " << path << "\n";
        return;
    }

    IOBit::write_byte_aligned(ofs, docNum);
    for (auto file : fileName) IOBit::write_string(ofs, file);

    std::cout << "Finish save filename to " << path << "\n";
}

void IndexGenerator::load(const std::string &path) {
    std::ifstream ifs(path, std::ios::binary);
    if(!ifs) {
        std::cerr << "Cannot open file : " << path << "\n";
        return;
    }

    docNum = IOBit::read_byte_aligned(ifs);
    fileName.resize(docNum);
    for (uint32_t i = 0; i < docNum; ++ i) fileName[i] = IOBit::read_string(ifs);

    std::cout << "Finish load filename from " << path << "\n";
}