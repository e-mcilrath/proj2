// merge.cpp

#include "volsort.h"

#include <iostream>

// Prototypes

Node *msort(Node *head, bool numeric);
void  split(Node *head, Node *&left, Node *&right);
Node *merge(Node *left, Node *right, bool numeric);

// Implementations

// pick int or string
// 
void merge_sort(List &l, bool numeric) {
}

Node *msort(Node *head, bool numeric) {
    // set equal to new head, call split function
    // if first = nullptr or next == nullptr then return

    // recursion using split

    // slow ptr - 1 node at a time
    // fast ptr - 2 ptr at a time 
    // this is so slow ptr ends up at middle

    // RELINK POINTERS!!!!!
    // dummy head, dummy tail for merge sort
}

void split(Node *head, Node *&left, Node *&right) {
}

Node *merge(Node *left, Node *right, bool numeric) {
}

