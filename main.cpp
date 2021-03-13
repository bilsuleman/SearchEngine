// Project 2: Search Engine
// Bilal Suleman
// Written in C++

// Description:
// This project is intended to read data from a file and build a search index
// based off of that. This index is then used to allow the user to search for
// terms in the document and the program then outputs URL's containing pages
// containing those search terms.

// This is just the entry point for the program containing main which launches
// the search engine UI. It also contains functions intended to test functions
// of the search engine individually.

#include <iostream>
#include "search.h"
using namespace std;

// These are some tester functions to test my functions against sample input.
void testClean();
void testGetTokens();
void testBuildIndex(map<string, set<string>> &);
void testFindQueryMatches(map<string, set<string>> &);

int main() {
    
    // Use this function to call and test the functions you write inside of
    // search.h.  If you do not call the functions here, nothing will run.
    
    // testClean();
    // testGetTokens();
    // map<string, set<string>> index;
    
    // testBuildIndex(index);
    // testFindQueryMatches(index);
    string file;
    char option;
    char print;
    cout << "Enter file to use for searching: ";
    cin >> file;
    cout << "Use creative component? y/n: ";
    cin >> option;
    cin.ignore();
    if (option == 'y') {
        cout << "Print removed stop words? y/n: ";
        cin >> print;
        cin.ignore();
        searchEngine2(file, print);
    } else {
        searchEngine(file);
    }
    return 0;
}

void testClean() {
    string test = "start";
    while(test.compare("q") != 0) {
        cout << "Enter a test string: ";
        cin >> test;
        cout << "Output: " << cleanToken(test) << endl;
    }
}

void testGetTokens() {
    string text;
    cout << "Enter input to tokenize: ";
    getline(cin, text, '\n');
    set<string> test = gatherTokens(text);
    cout << "Size: " << test.size() << endl;
    for (string s : test) {
        cout << s << endl;
    }
}

void testBuildIndex(map<string, set<string>> &searchIndex) {
    string file;
    cout << "Enter name of file to process: ";
    cin >> file;
    int num = buildIndex(file, searchIndex);
    cout << "Number of webpages processed: " << num << endl;
    cout << "Number of items indexed: " << searchIndex.size() << endl;
}

void testFindQueryMatches(map<string, set<string>> &searchIndex) {
    string search;
    set<string> results;
    cout << "Enter a search: ";
    cin.ignore();
    getline(cin, search, '\n');
    results = findQueryMatches(searchIndex, search);
    cout << "All search results: " << endl;
    for (auto &i : results) {
        cout << i << endl;
    }
    cout << endl;
}

