#include "inverted_index.h"
#include "indexer.h"
#include "query_processor.h"
#include <iostream>

const std::string STOPWORD_PATH = "stop_wotds_english.txt";

int main(int argc, char** argv) {
    std::string dir = argv[1];
    InvertedIndex idx;
    Tokenizer tokenizer;
    Indexer indexer(idx, tokenizer);

    std::cout << "Indexing files in: " << dir << " ...\n";

    indexer.index_directory(dir, STOPWORD_PATH);

    std::cout << "Indexing completed.\n";

    QueryProcessor qp(idx, tokenizer);

    std::string q;
    while(1) {
        std::cout << "Enter query (or input # to quit): ";
        if(!std::getline(std::cin, q)) break;
        if(q == "#") break;
        std::vector<std::pair<docid_t, double>> res = qp.TF_IDF(q);
        std::cout << "The results are : \n";
        for (auto [docid, val] : res) std::cout << indexer.get_filename(docid) << "\n";
    }
    return 0;
}
