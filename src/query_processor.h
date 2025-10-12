#pragma once
#include <string>
#include <vector>
#include "inverted_index.h"
#include "tokenizer.h"

class QueryProcessor {
    public:
        QueryProcessor(InvertedIndex &idx, Tokenizer &tokenizer);
        ~QueryProcessor() = default;
        std::vector<std::pair<docid_t, double>> TF_IDF(const std::string &query, uint32_t tot = 10);

    private:
        InvertedIndex &index;
        Tokenizer &tokenizer;
};
