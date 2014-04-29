//
//  main.cpp
//  bst
//
//  Created by Haibo Wang on 10/1/14.
//  Copyright (c) 2014 Hello Technology Pte Ltd. All rights reserved.
//

#include<iostream>
#include "binary_search_tree.hpp"
using namespace std;


int main(int argc, char* argv[]) {
    
    sketchpad::binary_search_tree<int> bst;
    std::list<int> input{123,24,63,25,98,23,35,46,565};
    std::list<int> sorted = bst.sort(input);
    
    std::copy(sorted.begin(),sorted.end(), ostream_iterator<int>(cout, " "));
    cout<<endl;
    
    sketchpad::binary_search_tree<std::string> months;
    std::list<std::string> jantodec {"January", "Febuary", "March", "April", "May", "June", "July", "Augusut", "September", "October", "November","December"};
    std::list<std::string> alphabetial = months.sort(jantodec);
    std::copy(alphabetial.begin(), alphabetial.end(), ostream_iterator<std::string>(cout, " "));
    cout<<std::endl;
    
    
    
    return 0;
    
    
}