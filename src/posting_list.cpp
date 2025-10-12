#include "posting_list.h"
#include <algorithm>

void PostingList::add(docid_t doc, uint32_t pos) {
    data.push_back({doc, pos});
}

const std::vector<std::pair<docid_t, uint32_t>>& PostingList::entries() const {
    return data;
}

void PostingList::merge(const PostingList &other) {
    /* */
}