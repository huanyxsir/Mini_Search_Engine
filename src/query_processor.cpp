#include "query_processor.h"
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <vector>

QueryProcessor::QueryProcessor(InvertedIndex &index, Tokenizer &tokenizer) : index(index), tokenizer(tokenizer) {}

std::vector<std::pair<docid_t, double>> QueryProcessor::TF_IDF(const std::string &query, uint32_t tot) {
    std::vector<std::string> terms = tokenizer.tokenize(query);
    if (terms.empty()) return {};

    std::vector<std::pair<docid_t, double>> allDocs(index.get_docNum());

    for (int i = 0; i < allDocs.size(); ++ i) allDocs[i].first = i;

    for (auto term : terms) {
        // std::cerr << term << "\n";
        std::vector<double> tf = index.get_TF(term);
        double idf = index.get_IDF(term);
        for (int i = 0; i < allDocs.size(); ++ i) allDocs[i].second += tf[i] * idf;
    }

    std::sort(allDocs.begin(), allDocs.end(), [&](const std::pair<docid_t, double> val1, const std::pair<docid_t, double> val2) -> bool {return val1.second > val2.second;});
    allDocs.resize(tot);
    while(allDocs.size() && allDocs.back().second <= 1e-9) allDocs.pop_back();
    return allDocs;
}