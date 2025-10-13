#pragma once
#include <vector>
#include <cstdint>

using docid_t = uint32_t;

class PostingList {
    public:
        PostingList() = default;
        ~PostingList() = default;
        
        void add(docid_t doc, uint32_t pos);
        std::vector<std::pair<docid_t, uint32_t>>& entries();
        void merge(PostingList &other);

    private:
        std::vector<std::pair<docid_t, uint32_t>> data;
};