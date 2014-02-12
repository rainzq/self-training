#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;

bool process_text();
string process_word(string word);

int main(int argc, char **argv)
{
    process_text();
}

// process input words, process them and count the processed word
bool process_text()
{
    int total = 0;
    string word, word_fined;
    map<string, int> wcount;
    multimap<int, string, greater<int> > sortByCount;
    while (cin >> word) {
        word_fined = process_word(word);
        if (!word_fined.empty()) {
            ++total;
            ++wcount[word_fined];
        }
    }
    cout << "Total words in document: " << total << endl;
    cout << "Unique words: " << wcount.size() << endl;
    for (auto it = wcount.begin(); it != wcount.end(); ++it) {
        sortByCount.insert(pair<int, string> (it->second, it->first));
    }
    for (auto it = sortByCount.begin(); it != sortByCount.end(); ++it) {
        cout << it->second << ' ' << it->first << endl;
    }
    return true;
}

// convert word to lowercase
// remove leading and/or tail punctions
string process_word(string word)
{
    string result;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    // find the first non-punction character
    auto begin_it = find_if_not(word.begin(), word.end(), ::ispunct);
    if (begin_it == word.end()) {  // all punctions
        return "";
    }
    // find the first tail punction
    auto end_it = find_if(begin_it, word.end(), ::ispunct);
    return result.assign(begin_it, end_it);
}
