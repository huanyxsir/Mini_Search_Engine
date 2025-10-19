#pragma once
#include <string>
#include <vector>
#include "inverted_index.h"
#include "tokenizer.h"

class QueryProcessor {
    public:
        QueryProcessor(InvertedIndex &idx, Tokenizer &tokenizer);
        ~QueryProcessor() = default;
        std::vector<std::pair<docid_t, double>> TF_IDF(const std::string &query, uint32_t tot = 10, double k1 = 1.2, double b = 0.75);

    private:
        InvertedIndex &index;
        Tokenizer &tokenizer;
};
