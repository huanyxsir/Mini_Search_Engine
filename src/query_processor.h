#pragma once
#include "inverted_index.h"
#include <string>
#include <vector>

class QueryProcessor {
public:
    QueryProcessor(const InvertedIndex &idx);
    // simple single-term or multi-term AND query (terms separated by space)
    std::vector<docid_t> and_query(const std::string &query) const;
    std::vector<docid_t> or_query(const std::string &query) const;

private:
    const InvertedIndex &index_;
};
