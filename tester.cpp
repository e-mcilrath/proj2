// tester.cpp
// Eric McIlrath, Abe Rashdan (emcilrat, arashdan)
// scratch program we used to poke at the sorts without going through main

#include "volsort.h"

#include <iostream>
#include <vector>

// just including the .cpp files so we dont have to touch the Makefile
#include "list.cpp"
#include "stl.cpp"
#include "qsort.cpp"
#include "merge.cpp"

using namespace std;

int main(){

    List l;
    vector<string> v1;

    v1.push_back("234");
    v1.push_back("443");
    v1.push_back("41");
    v1.push_back("5");
    v1.push_back("6");
    v1.push_back("3");
    v1.push_back("111");
    v1.push_back("123");

    for(size_t i = 0; i < v1.size(); i++){
        l.push_front(v1[i]);
    }

    merge_sort(l, true);

    //just prints everything out
    for (Node *curr = l.head; curr != nullptr; curr = curr->next){
        cout << curr->string << ' ';
    }

    cout << endl;
}
