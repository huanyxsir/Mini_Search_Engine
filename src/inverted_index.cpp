#include "inverted_index.h"
#include <sstream>
#include <fstream>
#include <iostream>

void InvertedIndex::add_term(const std::string &term, docid_t doc) {
    dict_[term].add(doc);
}

const PostingList* InvertedIndex::get_postings(const std::string &term) const {
    auto it = dict_.find(term);
    if(it == dict_.end()) return nullptr;
    return &it->second;
}

PostingList* InvertedIndex::get_postings_mut(const std::string &term) {
    return &dict_[term];
}

void InvertedIndex::save_to_file(const std::string &path) const {
    std::ofstream ofs(path);
    if(!ofs) { std::cerr << "Failed to open " << path << "\n"; return; }
    for(const auto &kv : dict_) {
        ofs << kv.first;
        for(auto &p : kv.second.entries()) {
            ofs << " " << p.first << ":" << p.second;
        }
        ofs << "\n";
    }
}

void InvertedIndex::load_from_file(const std::string &path) {
    // minimal loader; left as exercise to robustly parse
    dict_.clear();
    std::ifstream ifs(path);
    std::string line;
    while(std::getline(ifs, line)) {
        if(line.empty()) continue;
        std::istringstream iss(line);
        std::string term;
        iss >> term;
        std::string entry;
        while(iss >> entry) {
            auto colon = entry.find(':');
            if(colon == std::string::npos) continue;
            docid_t id = static_cast<docid_t>(std::stoul(entry.substr(0, colon)));
            uint32_t freq = static_cast<uint32_t>(std::stoul(entry.substr(colon+1)));
            for(uint32_t k=0;k<freq;k++) dict_[term].add(id); // rebuild by repeated adds
        }
    }
}
