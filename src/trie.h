#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

class Trie {
    public:
        Trie() = default;
        ~Trie();
        
        void add(const std::string &word, uint32_t val);
        uint32_t query(const std::string &word);

    private:
        struct Node {
            uint64_t val = 0;
            Node* children[128];
            Node() { for (int i = 0; i < 128; ++ i) children[i] = NULL; }
        };

        Node* root = new Node;
        void free_dfs(Node* node);
};