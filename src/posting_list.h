#pragma once
#include <vector>
#include <cstdint>

using docid_t = uint32_t;

class PostingList {
    public:
        PostingList() = default;
        ~PostingList() = default;
        
        void add(docid_t docid, uint32_t tot);
        std::vector<std::pair<docid_t, uint32_t>>& get_data();
        void merge(PostingList &other);

    private:
        std::vector<std::pair<docid_t, uint32_t>> data;
};