#include "inverted_index.h"
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>
#include "IObit.h"

void InvertedIndex::add_term(const std::string &term, docid_t docid, uint32_t tot) {
    // std::cerr << term << " " << docid << " " << tot << "\n";
    dict[term].add(docid, tot);
    if(docid >= docNum) docNum = docid + 1, sumTermFreq.resize(docNum);
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
    if(other_docNum > docNum) docNum = other_docNum, sumTermFreq.resize(docNum);
    for (docid_t docid = 0; docid < other_docNum; ++ docid) sumTermFreq[docid] += other_sumTermFreq[docid];

    // std::cerr << sumTermFreq.size() << "\n";
}

void InvertedIndex::save(const std::string &path) {
    std::ofstream ofs(path, std::ios::binary);
    if(!ofs) {
        std::cerr << "Cannot open file : " << path << "\n";
        return;
    }

    IOBit::write_byte_aligned(ofs, docNum);

    // std::cerr << sumTermFreq.size() << "\n";
    IOBit::write_byte_aligned(ofs, sumTermFreq.size());
    for(auto freq : sumTermFreq) IOBit::write_byte_aligned(ofs, freq);

    IOBit::write_byte_aligned(ofs, docFreq.size());
    for(auto &[term, freq] : docFreq) {
        IOBit::write_string(ofs, term);
        IOBit::write_byte_aligned(ofs, freq);
    }

    IOBit::write_byte_aligned(ofs, dict.size());
    for(auto [term, posting_list] : dict) {
        IOBit::write_string(ofs, term);
        posting_list.save(ofs);
    }

    std::cout << "Finish save inverted index to " << path << "\n";
}

void InvertedIndex::load(const std::string &path) {
    std::ifstream ifs(path, std::ios::binary);
    if(!ifs) {
        std::cerr << "Cannot open file : " << path << "\n";
        return;
    }

    dict.clear();
    docFreq.clear();
    sumTermFreq.clear();

    docNum = IOBit::read_byte_aligned(ifs);
    // std::cerr << docNum << "\n";
    
    sumTermFreq.resize(IOBit::read_byte_aligned(ifs));
    for(uint32_t i = 0; i < sumTermFreq.size(); ++ i) sumTermFreq[i] = IOBit::read_byte_aligned(ifs);
    
    uint32_t docFreq_size = IOBit::read_byte_aligned(ifs);
    
    for(uint32_t i = 0; i < docFreq_size; ++i) {
        std::string term = IOBit::read_string(ifs);
        docFreq[term] = IOBit::read_byte_aligned(ifs);
    }
    
    uint32_t dict_size = IOBit::read_byte_aligned(ifs);
    
    for(uint32_t i = 0; i < dict_size; ++i) {
        std::string term = IOBit::read_string(ifs);
        // std::cerr << term << "\n";
        PostingList posting_list;
        posting_list.load(ifs);
        dict[term] = posting_list;
    }
    
    std::cout << "Finish load inverted index from " << path << "\n";
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