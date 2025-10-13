#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "posting_list.h"

class InvertedIndex {
    public:
        InvertedIndex() = default;
        ~InvertedIndex() = default;
        
        void add_term(const std::string &term, docid_t docid, uint32_t tot);

        void save_to_file(const std::string &path) const;
        void load_from_file(const std::string &path);

        void merge(InvertedIndex &other);
        std::vector<double> get_TF(const std::string &trem);
        double get_IDF(const std::string &trem);

        std::unordered_map<std::string, PostingList> &get_dict();
        std::unordered_map<std::string, uint32_t> &get_docFreq();
        std::vector<uint32_t> &get_sumTermFreq();
        docid_t get_docNum();

    private:
        std::unordered_map<std::string, PostingList> dict;
        std::unordered_map<std::string, uint32_t> docFreq;
        std::vector<uint32_t> sumTermFreq;
        docid_t docNum = 0;
};