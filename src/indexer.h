#pragma once
#include "inverted_index.h"
#include "tokenizer.h"
#include <string>

class Indexer {
public:
    Indexer(InvertedIndex &idx);
    // index a directory of text files (each file -> one docID). Simple implementation:
    void index_directory(const std::string &dir_path);
    // index a single text (doc) with given docID
    void index_document(docid_t docid, const std::string &text);

private:
    InvertedIndex &index_;
    Tokenizer tokenizer_;
};
