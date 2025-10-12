#include "trie.h"

void Trie::add(const std::string &word, uint32_t val) {
    Node* now_node = root;
    for (auto c : word) {
        if(now_node->children[c] == NULL) now_node->children[c] = new Node;
        now_node = now_node->children[c];
    }
    now_node->val += val;
}

uint32_t Trie::query(const std::string &word) {
    Node* now_node = root;
    for (auto c : word) {
        if(now_node->children[c] == NULL) return 0;
        now_node = now_node->children[c];
    }
    return now_node->val;
}