// list.cpp
// Eric McIlrath, Abe Rashdan (emcilrat, arashdan)
// the List constructor, destructor, and push_front

#include "volsort.h"

#include <string>

List::List() {
    size = 0;
    head = nullptr;
}

List::~List() {
    while(head != nullptr) {  //delete everything
        Node *temp = head;
        head = head -> next;
        delete temp;
    }
}

void List::push_front(const std::string &s) {
    Node *temp = new Node;
    temp -> string = s;

    try {
        temp -> number = std::stoi(s);  //stoi throws if its not a number
    } catch (...) {
        temp -> number = 0;             //just use 0 then
    }

    temp -> next = head;  //point at the old front
    head = temp;

    size++;
}
