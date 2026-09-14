// stl.cpp

#include "volsort.h"

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
// C++ Style comparison function
bool node_number_compare(const Node *a, const Node *b){ //returns true if the first nodes number is less else returns false
    if (a-> number < b-> number) return true; 
    else return false;

};

bool node_string_compare(const Node *a, const Node *b){ //returns true if the first nodes string is less else returns false

    if (a-> string < b-> string) return true; 
    else return false;

}

void stl_sort(List &l, bool numeric) {
    std::vector<Node*> nodes;

    for (Node* current = l.head; current != nullptr; current = current->next) { // iterate through the list and push it back into a vector
        nodes.push_back(current);
    }


    if (numeric == true) sort(nodes.begin(), nodes.end(), node_string_compare);   
    else sort(nodes.begin(), nodes.end(), node_number_compare);

    nodes[nodes.size() - 1]->next = nullptr;
    l.head = nodes.front();

    for (int i = 0; i < nodes.size() - 1; i++) {
        nodes[i]->next = nodes[i + 1];
    }

}
