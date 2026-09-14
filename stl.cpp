// stl.cpp

#include "volsort.h"

#include <algorithm>
#include <iostream>
#include <vector>

//compare number (1)
//compare string (0)

void stl_sort(List &l, bool numeric) {
    std::vector<Node*> nodes;

    for (Node* current = l.head; current != nullptr; current = current->next) {
        nodes.push_back(current);
    }


    std::sort(nodes.begin(), nodes.end(), [numeric](Node* a, Node* b) {
        return a->number < b->number; // TODO int or string based on 'numeric'
    }); 

    nodes[nodes.size() - 1]->next = nullptr;
    l.head = nodes.front();

    for (int i = 0; i < nodes.size() - 1; i++) {
        nodes[i]->next = nodes[i + 1];
    }

}
