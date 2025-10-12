#include "inverted_index.h"
#include "indexer.h"
#include "query_processor.h"
#include <iostream>

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cout << "Usage: mini_search_engine <path_to_text_files_dir>\n";
        return 1;
    }
    std::string dir = argv[1];
    InvertedIndex idx;
    Indexer indexer(idx);
    std::cout << "Indexing files in: " << dir << " ...\n";
    indexer.index_directory(dir);
    std::cout << "Indexing completed.\n";

    QueryProcessor qp(idx);
    std::string q;
    while(true) {
        std::cout << "Enter query (or quit): ";
        if(!std::getline(std::cin, q)) break;
        if(q == "quit" || q == "exit") break;
        // simple: treat as AND query if contains "AND", "OR" -> OR query; else AND
        if(q.find(" OR ") != std::string::npos) {
            auto res = qp.or_query(q);
            std::cout << "OR result: ";
            for(auto d: res) std::cout << d << " ";
            std::cout << "\n";
        } else {
            auto res = qp.and_query(q);
            std::cout << "AND result: ";
            for(auto d: res) std::cout << d << " ";
            std::cout << "\n";
        }
    }
    return 0;
}
