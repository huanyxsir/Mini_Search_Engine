#pragma once
#include <string>
#include <unordered_map>
#include "posting_list.h"

class InvertedIndex {
public:
    InvertedIndex() = default;
    void add_term(const std::string &term, docid_t doc);
    const PostingList* get_postings(const std::string &term) const;
    // for simplicity, expose mutable reference for saving/loading or merging
    PostingList* get_postings_mut(const std::string &term);

    // simple text save/load (can be expanded to binary/compressed)
    void save_to_file(const std::string &path) const;
    void load_from_file(const std::string &path);

private:
    std::unordered_map<std::string, PostingList> dict_;
};