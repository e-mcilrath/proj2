#include "volsort.h"
using namespace std; 
#include <iostream>

List::List() {
    size = 0;
    head = nullptr;
}

List::~List() {
    while(head != nullptr) {
        Node *temp = head;
        head = head -> next;
        delete temp;
    }
}

void List::push_front(const std::string &s) {
    Node *temp = new Node;
    temp -> string = s;
    temp -> number = std::stoi(s);

    temp -> next = head;
    head = temp;

    cout << head -> number << endl; 

    size++;
}
