#include "query_processor.h"
#include <sstream>
#include <algorithm>
#include <unordered_map>

QueryProcessor::QueryProcessor(const InvertedIndex &idx) : index_(idx) {}

static std::vector<docid_t> postings_to_docids(const PostingList* p) {
    std::vector<docid_t> out;
    if(!p) return out;
    for(auto &pr : p->entries()) out.push_back(pr.first);
    return out;
}

std::vector<docid_t> QueryProcessor::and_query(const std::string &query) const {
    std::istringstream iss(query);
    std::string term;
    std::vector<std::vector<docid_t>> lists;
    while(iss >> term) {
        const PostingList* pl = index_.get_postings(term);
        lists.push_back(postings_to_docids(pl));
    }
    if(lists.empty()) return {};
    // intersect lists (assume each list sorted by docid)
    std::sort(lists.begin(), lists.end(), [](auto &a, auto &b){ return a.size() < b.size(); });
    std::vector<docid_t> res = lists.front();
    for(size_t i=1;i<lists.size();++i) {
        std::vector<docid_t> tmp;
        std::set_intersection(res.begin(), res.end(), lists[i].begin(), lists[i].end(), std::back_inserter(tmp));
        res.swap(tmp);
        if(res.empty()) break;
    }
    return res;
}

std::vector<docid_t> QueryProcessor::or_query(const std::string &query) const {
    std::istringstream iss(query);
    std::string term;
    std::unordered_map<docid_t,int> freq;
    while(iss >> term) {
        const PostingList* pl = index_.get_postings(term);
        if(!pl) continue;
        for(auto &p: pl->entries()) freq[p.first] += p.second; // accumulate freq as score
    }
    std::vector<std::pair<docid_t,int>> items(freq.begin(), freq.end());
    std::sort(items.begin(), items.end(), [](auto &a, auto &b){ return a.second > b.second; });
    std::vector<docid_t> out;
    for(auto &kv: items) out.push_back(kv.first);
    return out;
}
