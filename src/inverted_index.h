#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "posting_list.h"
#include "trie.h"

class InvertedIndex {
    public:
        InvertedIndex() = default;
        ~InvertedIndex() = default;
        
        void add_term(const std::string &term, docid_t doc, uint32_t pos);
        void save_to_file(const std::string &path) const;
        void load_from_file(const std::string &path);

        std::vector<double> get_TF(const std::string &trem);
        double get_IDF(const std::string &trem);
        uint32_t get_docnum();

    private:
        std::unordered_map<std::string, PostingList> dict;
        Trie docFreq;
        std::vector<Trie> termFreq;
        std::vector<uint32_t> sumTermFreq;
};