#pragma once
#include "inverted_index.h"
#include "tokenizer.h"
#include <string>
#include <vector>

class IndexGenerator {
    public:
        IndexGenerator(InvertedIndex &idx, Tokenizer &tokenizer);
        ~IndexGenerator() = default;
        
        void index_directory(const std::string &dir_path);
        void index_document(docid_t docid, const std::string &text);
        std::string get_filename(docid_t docid);
        
        void save(const std::string &path);
        void load(const std::string &path);

    private:
        InvertedIndex &index;
        Tokenizer &tokenizer;
        std::vector<std::string> fileName;
        docid_t docNum = 0;
};
