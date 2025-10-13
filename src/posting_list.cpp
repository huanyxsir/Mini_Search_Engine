#include "posting_list.h"
#include <algorithm>
#include <iostream>

void PostingList::add(docid_t docid, uint32_t tot) { data.push_back({docid, tot}); }

std::vector<std::pair<docid_t, uint32_t>>& PostingList::get_data() { return data; }

void PostingList::merge(PostingList &other) {
    std::vector<std::pair<docid_t, uint32_t>> &other_data = other.get_data(), res;
    int i = 0, j = 0;
    while(i < data.size() || j < other_data.size()) {
        // std::cerr << i << " " << j << " " << data.size() << " " << other_data.size() << "\n";
        if(i < data.size() && j < other_data.size() && data[i].first == other_data[j].first) res.push_back({data[i].first, data[i].second + other_data[j].second}), ++ i, ++ j;
        else if(j == other_data.size() || (i < data.size() && data[i] < other_data[j])) res.push_back(data[i ++]);
        else res.push_back(other_data[j ++]);
    }
    // std::cerr << data.size() << " " << other_data.size() << " " << res.size() << " " << "\n";
    std::swap(data, res);
}