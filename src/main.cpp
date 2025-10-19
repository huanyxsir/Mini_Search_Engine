#include "inverted_index.h"
#include "index_generator.h"
#include "query_processor.h"
#include <iostream>
#include <sstream>

const std::string STOPWORD_PATH = "../src/stop_words_english.txt";
const std::string SAVE_INDEX_GENERATOR_PATH = "../save_data/index_generator.bin";
const std::string SAVE_INVERTED_INDEX_PATH = "../save_data/inverted_index.bin";
const std::string DATA_PATH = "../data";

void command(std::string q, InvertedIndex &idx, IndexGenerator &index_generator) {
    if(q == "/save") {
        idx.save(SAVE_INVERTED_INDEX_PATH);
        index_generator.save(SAVE_INDEX_GENERATOR_PATH);
    } else if(q == "/load") {
        idx.load(SAVE_INVERTED_INDEX_PATH);
        index_generator.load(SAVE_INDEX_GENERATOR_PATH);
    } else if(q == "/update") {
        index_generator.index_directory(DATA_PATH);
    } else {
        std::cerr << "Unknown command\n";
    }
}

int main(int argc, char** argv) {
    InvertedIndex idx;
    Tokenizer tokenizer;

    tokenizer.load_stopwords(STOPWORD_PATH);
    IndexGenerator index_generator(idx, tokenizer);

    std::cout << "Initializing inverted index ...\n";
    idx.load(SAVE_INVERTED_INDEX_PATH);
    std::cout << "Completed.\n";
    
    std::cout << "Initializing index_generator ...\n";
    index_generator.load(SAVE_INDEX_GENERATOR_PATH);
    std::cout << "Completed.\n";


    QueryProcessor qp(idx, tokenizer);

    std::string q;
    while(1) {
        std::cout << "Enter query (or input # to quit): ";
        if(!std::getline(std::cin, q)) break;
        if(q == "#") break;

        if(q[0] == '/') {
            command(q, idx, index_generator);
            continue;
        }

        std::vector<std::pair<docid_t, double>> res = qp.TF_IDF(q);
        std::cout << "The results are : \n";
        for (auto [docid, score] : res) std::cout << "PATH : " << index_generator.get_filename(docid) << "\n";
    }
    return 0;
}
