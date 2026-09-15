#include <vector> 
#include "volsort.h"
#include "stl.cpp"
#include "list.cpp"
#include <iostream> 
#include <iomanip> 
#include "qsort.cpp"

using namespace std; 

int main(){

 
List *l; 
l = new List; 
vector<string> v1;


v1.push_back("234"); 
v1.push_back("443");
v1.push_back("41"); 
v1.push_back("5");
v1.push_back("6"); 
v1.push_back("3");
v1.push_back("111"); 
v1.push_back("123");




for(int i=0; i< v1.size(); i++){ 
    
    l->push_front(v1[i]);

}



    qsort_sort(*l,true);

    //just prints everything out
    while (l->head->next != nullptr){ 
    
    cout << l->head->string << ' '; 
    l->head = l->head-> next; 
    }

    cout << endl; 


}