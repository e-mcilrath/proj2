// merge.cpp

#include "volsort.h"

#include <iostream>

// Prototypes

Node *msort(Node *head, bool numeric);
void  split(Node *head, Node *&left, Node *&right);
Node *merge(Node *left, Node *right, bool numeric);

// Implementations

void merge_sort(List &l, bool numeric) { // shell
    l.head = msort(l.head, numeric);
}

Node *msort(Node *head, bool numeric) {

    if (head == nullptr || head->next == nullptr) { // return case
        return head;
    }

    Node *left;
    Node *right;

    split(head, left, right);    // 1. split
    left = msort(left, numeric);        // 2. sort left
    right = msort(right, numeric);       // 3. sort right
    return merge(left, right, numeric); // 4. merge back into list

    // RELINK POINTERS!!!!!
    // dummy head, dummy tail for merge sort
}

void split(Node *head, Node *&left, Node *&right) {

    if (head == nullptr || head->next ==nullptr) {
        left = head;
        right = nullptr;
        return;
    }

    Node *slow = head;
    Node *fast = head->next; // so that slow->next is the middle

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    left = head;
    right = slow->next;

    slow->next = nullptr;
}


Node *merge(Node *left, Node *right, bool numeric) {
    Node head;
    Node *tail = &head;
    
    while (left != nullptr && right != nullptr) {
            if (numeric ? left->number <= right->number : left->string <= right->string) {
                tail->next = left;
                left = left->next;
            } else {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }

        tail->next = (left != nullptr) ? left : right;

    return head.next;
}

