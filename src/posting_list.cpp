#include "posting_list.h"
#include <algorithm>

void PostingList::add(docid_t doc) {
    if(data_.empty() || data_.back().first != doc) {
        data_.push_back({doc, 1});
    } else {
        data_.back().second += 1;
    }
}

const std::vector<std::pair<docid_t, uint32_t>>& PostingList::entries() const {
    return data_;
}

void PostingList::merge(const PostingList &other) {
    std::vector<std::pair<docid_t,uint32_t>> merged;
    merged.reserve(data_.size() + other.data_.size());
    auto i = data_.begin();
    auto j = other.data_.begin();
    while(i != data_.end() && j != other.data_.end()) {
        if(i->first < j->first) { merged.push_back(*i); ++i; }
        else if(i->first > j->first) { merged.push_back(*j); ++j; }
        else { merged.push_back({i->first, i->second + j->second}); ++i; ++j; }
    }
    while(i != data_.end()) { merged.push_back(*i); ++i; }
    while(j != other.data_.end()) { merged.push_back(*j); ++j; }
    data_.swap(merged);
}