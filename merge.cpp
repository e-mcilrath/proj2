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
    Node *head, *tail;

    
    
    if(numeric){ //if comparing numbers

        if(left-> number <= right-> number){ 

         head = left; // first we initialize the head of the list
        left = left-> next; 
        } 
        else {
            head = right; 
            right = right-> next; 
        }
        
        tail = head; // start from the beginning of the list this is our dummy tail
        while(left!= nullptr && right != nullptr){

            if(left-> number <= right-> number ) { 
                tail->next = left; //push left to the list
                tail = left; // move tail to point to that number
                left = left-> next; //move to the next number in the left list

            }
            else { // repeat for right
                tail-> next = right; 
                tail = right; 
                right = right -> next; 
                
                
            }

            


        }
        // these two check if there are any remaining numbers in either side
        if(left != nullptr) tail-> next = left;
        else if( right != nullptr) tail-> next = right;
        



    }

    else { // do the same thing for merging strings
        if(left-> string <= right-> string){ 

         head = left; // first we initialize the head of the list
        left = left-> next; 
        } 
        else {
            head = right; 
            right = right-> next; 
        }
        
        tail = head; // start from the beginning of the list this is our dummy tail
        while(left!= nullptr && right != nullptr){

            if(left-> string <= right-> string ) { 
                tail->next = left; //push left to the list
                tail = left; // move tail to point to that number
                left = left-> next; //move to the next number in the left list

            }
            else { // repeat for right
                tail-> next = right; 
                tail = right; 
                right = right -> next; 
                
                
            }

            


        }
        // these two check if there are any remaining numbers in either side
        if(left != nullptr) tail-> next = left;
        else if( right != nullptr) tail-> next = right;

    }




   


}

