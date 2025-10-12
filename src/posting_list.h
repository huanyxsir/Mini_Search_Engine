#pragma once
#include <vector>
#include <cstdint>

using docid_t = uint32_t;

class PostingList {
    public:
        PostingList() = default;
        ~PostingList() = default;
        
        void add(docid_t doc, uint32_t pos);
        const std::vector<std::pair<docid_t, uint32_t>>& entries() const;
        void merge(const PostingList &other);

    private:
        std::vector<std::pair<docid_t, uint32_t>> data;
};