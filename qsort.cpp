// qsort.cpp
// Eric McIlrath, Abe Rashdan (emcilrat, arashdan)
// sorts the list with C qsort

#include "volsort.h"

#include <cstdlib>
#include <vector>

int qComparisonInt(const void * a, const void * b){ //we declare the function with void pointers
    const Node* nodeA = *(const Node**)a; // we are telling the program to treat the varables as nodes
    const Node* nodeB = *(const Node**)b;

    if(nodeA-> number < nodeB -> number) return -1; //sorting a and b
    else if( nodeA-> number > nodeB -> number ) return 1;
    return 0;
}

int qComparisonString(const void* a, const void * b){
    const Node* nodeA = *(const Node**)a; // same deal but on the string instead
    const Node* nodeB = *(const Node**)b;

    if(nodeA-> string < nodeB -> string) return -1; //sorting a and b
    else if( nodeA-> string > nodeB -> string ) return 1;
    return 0;
}

void qsort_sort(List &l, bool numeric) {
    std::vector<Node*> nodes;
    nodes.reserve(l.size);  //we know the size already

    for (Node* current = l.head; current != nullptr; current = current->next) { // iterate through the list and push it back into a vector
        nodes.push_back(current);
    }

    if(nodes.empty()) return;

    // raw array, how many, and how big one of them is
    qsort(nodes.data(), nodes.size(), sizeof(Node*), numeric ? qComparisonInt : qComparisonString);

    for (size_t i = 0; i < nodes.size() - 1; i++) { // loops through entire vector and assigns appropriate next node values
        nodes[i]->next = nodes[i + 1];
    }

    nodes[nodes.size() - 1]->next = nullptr; //assign the last node's next to be null
    l.head = nodes[0]; // we go to the front of the vector and assign it as head
}
