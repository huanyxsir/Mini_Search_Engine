#pragma once
#include <vector>
#include <cstdint>
#include <utility>

using docid_t = uint32_t;

class PostingList {
public:
    void add(docid_t doc);
    const std::vector<std::pair<docid_t, uint32_t>>& entries() const;
    // merge other posting list into this (assuming sorted unique docIDs)
    void merge(const PostingList &other);

private:
    // pair<docID, frequency>
    std::vector<std::pair<docid_t, uint32_t>> data_;
};