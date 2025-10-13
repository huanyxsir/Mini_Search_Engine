#pragma once
#include <vector>
#include <cstdint>
#include <sstream>

using docid_t = uint32_t;

class PostingList {
    public:
        PostingList() = default;
        ~PostingList() = default;
        
        void add(docid_t docid, uint32_t tot);
        std::vector<std::pair<docid_t, uint32_t>>& get_data();
        void merge(PostingList &other);
        void save(std::ostream &os) const;
        void load(std::istream &is);

    private:
        std::vector<std::pair<docid_t, uint32_t>> data;
        std::vector<uint32_t> compute_gaps() const;
};