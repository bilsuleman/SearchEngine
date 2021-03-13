// Project 2: Search Engine
// Bilal Suleman
// Written in C++

// Description:
// This project is intended to read data from a file and build a search index
// based off of that. This index is then used to allow the user to search for
// terms in the document and the program then outputs URL's containing pages
// containing those search terms.

// This file is the header file containing all the functions pertaining to the
// search engine. These functions are then called in main to actually execute
// the search and display results.


#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <cctype>
#include <string>
using namespace std;


// This is a helper function for cleanToken, it checks whether or not
// if a particular character is a alphabetic letter or not, true or false is
// returned.
bool checkLetter(string s) {
    // cout << "In checkSize" << endl;
    bool valid = false;
    for (string::iterator it = s.begin(); it != s.end(); it++) {
        if (isalpha(*it)) {     // iterator is used to access character
            // cout << "Letter is: " << *it << endl;
            valid = true;
            break;
        } else {
            // cout << "Output is: " << *it << endl;
            valid = false;
        }
    }
    return valid;
}

// This functions cleans a token for processing by the search engine.
// This is achieved by removing all punctuation from the token and rejecting
// token sthat containing no alphabetic characters or are just invalid.
// The cleaned token is returned from this function.
string cleanToken(string s) {
    // cout << "In clean" << endl;
    while ( ispunct(s.front()) || ispunct(s.back()) ) {
        if ( ispunct(s.front()) ) {
            // cout << endl << "Erasing " << *(s.begin()) << endl;
            s.erase( s.begin() );
        } 
        if ( ispunct(s.back()) ) {
            // cout << endl << "Erasing " << *(s.end() - 1) << endl;
            s.erase( s.end() - 1 );
        }
    }
    if ( !checkLetter(s) ) {
        return "";
    }
    
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    // cout << s << endl;
    return s;
}

// This function is used to extract tokens from a body of text, it processes
// an entire body of text and breaks it up into unique tokens as well as
// calling cleanToken and cleaning the tokens in the process. This is done
// by utilizing a set which does not allow for repeat tokens. The set is what
// is returned as well.
set<string> gatherTokens(string text) {
    set<string> tokens;
    stringstream body(text);
    string temp;
    while (getline(body, temp, ' ')) {
        temp = cleanToken(temp);
        if (temp.compare("") != 0) {
            tokens.insert(temp);
        }
    }
    return tokens;
}

// Helper function to reduce CC from buildIndex, just perfoms loop for insert
// operation into map.
void insertIndex(map<string, set<string>>& index, string& body, string& url) {
    set<string> keys = gatherTokens(body);
    for (string s : keys) {
        // cout << "Working on: " << s << endl;
        index[s].insert(url);
    }
}

// This function combines the functionality of gatherTokens and cleanToken
// with file processing to take in a file and build an index from that file
// by getting the uniqe tokens in that file and then adding those to a map.
// The end result is a map where each unique key, generated from the set
// returned from gatherTokens, is mapped to the set of url's that contain that
// term.
int buildIndex(string filename, map<string, set<string>>& index) {
    fstream infile(filename);
    if (!infile.is_open()) {
        cout << "Error opening file." << endl;
        return 0;
    }
    int processed = 0;
    string url, body;
    while (!infile.eof()) {
        getline(infile, url, '\n');
        getline(infile, body, '\n');
        if (!infile.fail()) {
            insertIndex(index, body, url);
            processed++;
        }
    }
    // cout << "Printing index: " << endl;
    // for(auto i : index) {
    //     cout << i.first << ": ";
    //     for(auto i : i.second) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    // }
    return processed;
}

// This helper function can be used to print the contents of a set to the
// screen.
void printSet(set<string> s) {
    for (auto &i : s) {
        cout << i << endl;
    }
}

// This function is used to actually conduct a search. This is achieved by
// using the index already built in other words the map, and then using the
// search keys to look through the map. The users search is broken up by term
// using a string stream and the result is searched for using the find function
// for C++ maps. The C++ set union, intersection, and difference functions are
// then applied depending on the parameters the user selected.
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    map<string, set<string>>::iterator it;
    stringstream s(sentence);
    string search;
    while (s >> search) {
        char option;
        option = search.front();
        search = cleanToken(search);
        it = index.find(search);
        if (it == index.end()) {
            if (option == '+') {
                result.clear();
            }
        } else {
            // cout << "Results for: " << search << endl <<
            // "With option: " << option << endl;
            // cout << it->first << " : ";
            // printSet(it->second);
            if (option == '+') {
                set<string> intersection;
                set_intersection(result.begin(), result.end(),
                    it->second.begin(), it->second.end(),
                    std::inserter(intersection, intersection.begin()));
                result = intersection;
            } else if (option == '-') {
                set<string> difference;
                set_difference(result.begin(), result.end(),
                    it->second.begin(), it->second.end(),
                    std::inserter(difference, difference.begin()));
                result = difference;
            } else {
                set<string> unionSet;
                set_union(result.begin(), result.end(),
                    it->second.begin(), it->second.end(),
                    std::inserter(unionSet, unionSet.begin()));
                result = unionSet;
            }
        }
    }
    return result;
}

// This is the search engine UI. It calls all helper functions to build the
// index and then outpus the results of the built index. It then enters a loop
// where it keeps asking the user for a query until the user terminates the
// program by hitting the enter key.
void searchEngine(string filename) {
    map<string, set<string>> searchIndex;
    cout << "Stand by while building index..." << endl;
    int pages = buildIndex(filename, searchIndex);
    if (pages == 0) {
        return;
    }
    cout << "Indexed " << pages << " pages containing "
         << searchIndex.size() << " unique terms" << endl;
    string query = "test";
    // cin.ignore();
    while (query.length() > 0) {
        set<string> searchResults;
        cout << endl;
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, query, '\n');
        if (query.length() == 0) {
            break;
        }
        // cout << "Query: " << query << endl;
        searchResults = findQueryMatches(searchIndex, query);
        cout << "Found " << searchResults.size()
             << " matching pages" << endl;
        printSet(searchResults);
    }
    cout << "Thank you for searching!" << endl;
}

// Creative component removing stop words
// The folowing functions remove stop words from the index.

// This function takes in a list of stop words from a file and loops through
// the index removing all occurences of these words. It then returns the
// index cleaned of all stop words. To use this function answer y in main when
// prompted to use the creative component. To demonstrate that it is actually
// working I have set it up to print out the stop words being removed,
// this 
void removeStop(map<string, set<string>> &index, char print) {
    fstream infile("stopWords.txt");
    set<string> stopWords;
    string word;
    while (!infile.eof()) {
        getline(infile, word, '\n');
        if (!infile.fail()) {
            stopWords.insert(word);
        }
    }
    map<string, set<string>>::iterator it;
    for (string i : stopWords) {
        it = index.find(i);
        if (it == index.end()) {
            continue;
        } else if (print == 'y') {
           cout << "Removing: " << i << endl;
           index.erase(it); 
        } else {
            index.erase(it);
        }
    }
}

// Second version of search engine UI that utilizes creative component.
void searchEngine2(string filename, char print) {
    map<string, set<string>> searchIndex;
    cout << "Stand by while building index..." << endl;
    int pages = buildIndex(filename, searchIndex);
    if (pages == 0) {
        return;
    }
    cout << "Indexed " << pages << " pages containing "
         << searchIndex.size() << " unique terms" << endl;
    cout << "Removing stop words from index..." << endl;
    removeStop(searchIndex, print);
    cout << "Now indexed " << pages << " pages containing "
         << searchIndex.size() << " unique terms" << endl;
    string query = "test";
    // cin.ignore();
    while (query.length() > 0) {
        set<string> searchResults;
        cout << endl;
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, query, '\n');
        if (query.length() == 0) {
            break;
        }
        // cout << "Query: " << query << endl;
        searchResults = findQueryMatches(searchIndex, query);
        cout << "Found " << searchResults.size()
             << " matching pages" << endl;
        printSet(searchResults);
    }
    cout << "Thank you for searching!" << endl;
}


