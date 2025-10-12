#include "inverted_index.h"
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>

void InvertedIndex::add_term(const std::string &term, docid_t doc, uint32_t pos) {
    // std::cerr << term << " " << doc << " " << pos << "\n";
    dict[term].add(doc, pos);

    if(doc >= termFreq.size()) termFreq.resize(doc + 1), sumTermFreq.resize(doc + 1);

    // std::cerr << "finish resize\n";
    if(!termFreq[doc].query(term)) docFreq.add(term, 1);
    // std::cerr << "finish docFreq\n";
    termFreq[doc].add(term, 1);
    ++ sumTermFreq[doc];
}

void InvertedIndex::save_to_file(const std::string &path) const {
    /* */
}

void InvertedIndex::load_from_file(const std::string &path) {
    /* */
}

std::vector<double> InvertedIndex::get_TF(const std::string &term) {
    std::vector<double> tfList;
    for (int i = 0; i < termFreq.size(); ++ i) tfList.push_back(1.0 * termFreq[i].query(term) / sumTermFreq[i]);
    return tfList;
}

double InvertedIndex::get_IDF(const std::string &term) {
    return log(1.0 * termFreq.size() / (docFreq.query(term) + 1));
}

uint32_t InvertedIndex::get_docnum() {return termFreq.size();}