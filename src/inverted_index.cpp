#include "inverted_index.h"
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>

void InvertedIndex::add_term(const std::string &term, docid_t docid, uint32_t tot) {
    // std::cerr << term << " " << docid << " " << tot << "\n";
    dict[term].add(docid, tot);
    if(docid >= docNum) docNum = docid + 1, sumTermFreq.reserve(docNum);
    ++ docFreq[term];
    sumTermFreq[docid] += tot;
}

void InvertedIndex::merge(InvertedIndex &other) {
    std::unordered_map<std::string, PostingList> &other_dict = other.get_dict();
    std::unordered_map<std::string, uint32_t> &other_docFreq = other.get_docFreq();
    std::vector<uint32_t> &other_sumTermFreq = other.get_sumTermFreq();
    docid_t other_docNum = other.get_docNum();

    // std::cerr << "start merge\n";
    for (auto [key, val] : other_dict) dict[key].merge(val);
    // std::cerr << "finish dict merge\n";
    for (auto [key, val] : other_docFreq) docFreq[key] += val;
    if(other_docNum > docNum) docNum = other_docNum, sumTermFreq.reserve(docNum);
    for (docid_t docid = 0; docid < other_docNum; ++ docid) sumTermFreq[docid] += other_sumTermFreq[docid];

    // std::cerr << docNum << "\n";
}

void InvertedIndex::save_to_file(const std::string &path) const {
    /* */
}

void InvertedIndex::load_from_file(const std::string &path) {
    /* */
}

std::vector<double> InvertedIndex::get_TF(const std::string &term) {
    std::vector<std::pair<docid_t, uint32_t>> &postinglist = dict[term].get_data();
    std::vector<double> res(docNum);

    for (auto [docid, tf] : postinglist) res[docid] = 1.0 * tf / sumTermFreq[docid];

    return res;
}

double InvertedIndex::get_IDF(const std::string &term) { return log(1.0 * docNum / (docFreq[term] + 1));}

docid_t InvertedIndex::get_docNum() {return docNum;}

std::unordered_map<std::string, PostingList> &InvertedIndex::get_dict() {return dict;}

std::unordered_map<std::string, uint32_t> &InvertedIndex::get_docFreq() {return docFreq;}

std::vector<uint32_t> &InvertedIndex::get_sumTermFreq() {return sumTermFreq;}