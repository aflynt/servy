#ifndef _UTIL
#define _UTIL

#include <iostream>
#include <sstream>
#include <vector>
#include "machine.h"

using namespace std;

vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline(ss,item, delim)) {
      result.push_back(item);
    }
    return result;
}

// split "thor:60" with delim : into str and int
//vector<machine> get_machines( vector<string>& vs) {
vector<machine> get_machines( const string& mstring) {
    
    // first split on ','
    auto vs = split(mstring, ','); // now have split machines vector of strings

    vector<machine> result;

    // split each string into str,int pairs
    for (auto& s : vs){
      for (char& ch : s) //replace each punct char with space
        switch(ch) {
          case ':':
            ch = ' ';
        }

      stringstream ss (s); // make istream ss reading from string s
      string name;
      int numcores;
      ss >> name;
      ss >> numcores;
      result.push_back(machine{name,numcores});
    }
    return result;
}

#endif
