// stl.cpp
// Eric McIlrath, Abe Rashdan (emcilrat, arashdan)
// sorts the list with std::sort

#include "volsort.h"

#include <algorithm>
#include <vector>

// C++ Style comparison function
bool node_number_compare(const Node *a, const Node *b){ //returns true if the first nodes number is less else returns false
    return a-> number < b-> number;
}

bool node_string_compare(const Node *a, const Node *b){ //returns true if the first nodes string is less else returns false
    return a-> string < b-> string;
}

void stl_sort(List &l, bool numeric) {
    std::vector<Node*> nodes;
    nodes.reserve(l.size);  //we know the size already

    for (Node* current = l.head; current != nullptr; current = current->next) { // iterate through the list and push it back into a vector
        nodes.push_back(current);
    }

    if(nodes.empty()) return;

    std::sort(nodes.begin(), nodes.end(), numeric ? node_number_compare : node_string_compare);

    for (size_t i = 0; i < nodes.size() - 1; i++) { // loops through entire vector and assigns appropriate next node values
        nodes[i]->next = nodes[i + 1];
    }

    nodes[nodes.size() - 1]->next = nullptr; //assign the last node's next to be null
    l.head = nodes[0]; // we go to the front of the vector and assign it as head
}
