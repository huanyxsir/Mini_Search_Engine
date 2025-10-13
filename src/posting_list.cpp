#include "posting_list.h"
#include <algorithm>

void PostingList::add(docid_t doc, uint32_t pos) {
    data.push_back({doc, pos});
}

std::vector<std::pair<docid_t, uint32_t>>& PostingList::entries() {
    return data;
}

void PostingList::merge(PostingList &other) {
    std::vector<std::pair<docid_t, uint32_t>> &other_data = other.entries(), res;
    int i = 0, j = 0;
    while(i < data.size() || j < other_data.size()) {
        if(j == data.size() || (i < data.size() && data[i] < other_data[j])) res.push_back(data[i ++]);
        else res.push_back(data[j ++]);
    }
    std::swap(data, res);
}