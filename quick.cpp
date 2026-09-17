// quick.cpp
// Eric McIlrath, Abe Rashdan (emcilrat, arashdan)
// our own quick sort, done by relinking the nodes

#include "volsort.h"

// Prototypes

Node *qsort(Node *head, bool numeric);
void  partition(Node *head, Node *pivot, Node *&left, Node *&right, bool numeric);
Node *concatenate(Node *left, Node *right);

void quick_sort(List &l, bool numeric) {
    l.head = qsort(l.head, numeric); //just a shell
}

Node *qsort(Node *head, bool numeric) {

    if (head == nullptr || head->next == nullptr) { // check for 1 or 0 in list
        return head;
    }

    Node *pivot = head;          // pivot is head
    Node *rest = head->next;     // dont lose the rest of the list
    pivot->next = nullptr;       //detach head

    // initialize left and right
    Node *left = nullptr;
    Node *right = nullptr;

    partition(rest, pivot, left, right, numeric);

    //sort left and right
    left = qsort(left, numeric);
    right = qsort(right, numeric);

    // final product
    return concatenate(concatenate(left, pivot), right);
}

void partition(Node *head, Node *pivot, Node *&left, Node *&right, bool numeric) {

    // make sure they're empty
    left = nullptr;
    right = nullptr;

    // run through the list
    for (Node *current = head; current != nullptr; ) { // empty because using & and changing *->next

        Node *next = current->next; //save *->next
        if (numeric ? current->number < pivot->number : current->string < pivot->string) { // compare number or string
            current->next = left;   // add current node to left
            left = current;
        }
        else {
            current->next = right;  //add current node to right
            right = current;
        }
        current = next; // iterate
    }
}

Node *concatenate(Node *left, Node *right) {

    if (left == nullptr) {
        return right;
    }
    else {
        Node *temp = left;
        while (temp->next != nullptr) { // get to the end of left
            temp = temp->next;
        }
        temp->next = right; //concatenate
        return left;
    }
}
