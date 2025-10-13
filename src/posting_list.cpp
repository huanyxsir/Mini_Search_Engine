#include "posting_list.h"
#include "IObit.h"
#include <algorithm>
#include <iostream>
#include <sstream>

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

std::vector<uint32_t> PostingList::compute_gaps() const {
    if (data.empty()) return {};
    
    std::vector<uint32_t> gaps;
    docid_t pre_docid = 0;
    
    auto sorted_data = data;
    std::sort(sorted_data.begin(), sorted_data.end());
    
    for (auto [docid, df] : sorted_data) {
        gaps.push_back(docid - pre_docid);
        pre_docid = docid;
    }
    
    return gaps;
}

void PostingList::save(std::ostream &os) const {
    if (data.empty()) {
        IOBit::write_byte_aligned(os, 0);
        return;
    }
    
    auto sorted_data = data;
    std::sort(sorted_data.begin(), sorted_data.end());
    
    IOBit::write_byte_aligned(os, sorted_data.size());
    
    std::vector<uint32_t> gaps = compute_gaps();
    
    IOBit::write_byte_aligned(os, sorted_data[0].first);
    IOBit::write_byte_aligned(os, sorted_data[0].second);
    
    for (size_t i = 1; i < sorted_data.size(); ++i) {
        IOBit::write_byte_aligned(os, gaps[i]);
        IOBit::write_byte_aligned(os, sorted_data[i].second);
    }
}

void PostingList::load(std::istream &is) {
    data.clear();
    
    uint32_t list_size = IOBit::read_byte_aligned(is);
    if (list_size == 0) return;
    
    docid_t first_docid = IOBit::read_byte_aligned(is);
    uint32_t first_tf = IOBit::read_byte_aligned(is);
    data.push_back({first_docid, first_tf});
    
    docid_t now_docid = first_docid;
    
    for (uint32_t i = 1; i < list_size; ++ i) {
        uint32_t gap = IOBit::read_byte_aligned(is);
        uint32_t tf = IOBit::read_byte_aligned(is);
        
        now_docid += gap;

        // std::cerr << now_docid << " " << tf << "\n";
        data.push_back({now_docid, tf});
    }
}