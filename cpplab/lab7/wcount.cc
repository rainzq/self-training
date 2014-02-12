#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <set>
#include <utility>


// So we don't have to type "std::" everywhere...
using namespace std;

void initSkipList(set<string>& skipList);
void initSkipList(set<string>& skipList, char *filename);
string processWord(string word);
void processText(const set<string> &skipList, map<string, int>& wordCounts);
void outputWordsByCount(map<string, int>& wordCounts);


int main(int argc, char **argv)
{
    set<string> skipList;
    map<string, int> wordCounts;
    if (argc == 2) {
        initSkipList(skipList, argv[1]);
    } else {
        initSkipList(skipList);
    }

    // Process the text on console-input, using the skip-list.
    processText(skipList, wordCounts);

    // Finally, output the word-list and the associated counts.
    outputWordsByCount(wordCounts);
}

/*
 * This function initializes the skip-list of words.
 *
 * skipList = the set of words to skip
 */
void initSkipList(set<string>& skipList)
{
    // Use a pre-specified skip-list.

    const char *swords[] = {
        "a", "all", "am", "an", "and", "are", "as", "at",
        "be", "been", "but", "by",
        "did", "do",
        "for", "from",
        "had", "has", "have", "he", "her", "hers", "him", "his",
        "i", "if", "in", "into", "is", "it", "its",
        "me", "my",
        "not",
        "of", "on", "or",
        "so",
        "that", "the", "their", "them", "they", "this", "to",
        "up", "us",
        "was", "we", "what", "who", "why", "will", "with",
        "you", "your",
        0
    };

    for (int i = 0; swords[i] != 0; i++)
        skipList.insert(move(string(swords[i])));
}

/*
 * This function initializes the skip-list of words.
 *
 * skipList = the set of words to skip which read from filename
 */
void initSkipList(set<string>& skipList, char *filename)
{
    // skipwords read from filename
    ifstream file(filename);
    string word;
    while (file >> word) {
        skipList.insert(move(word));
    }
}


/*
 * This helper-function converts a word to all lower-case, and then removes
 * any leading and/or trailing punctuation.
 *
 * Parameters:
 *   word    The word to process.  It is passed by-value so that it can be
 *           manipulated within the function without affecting the caller.
 *
 * Return value:
 *   The word after all leading and trailing punctuation have been removed.
 *   Of course, if the word is entirely punctuation (e.g. "--") then the result
 *   may be an empty string object (containing "").
 */
string processWord(string word)
{
    string result;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    // find the first non-punctuation character
    auto begin_it = find_if_not(word.begin(), word.end(), ::ispunct);
    if (begin_it == word.end()) {  // all are punctuations
        return "";
    }
    // find the first tail punctuation
    auto end_it = find_if(begin_it, word.end(), ::ispunct);
    return result.assign(begin_it, end_it);
}


void processText(const set<string> &skipList, map<string, int>& wordCounts)
{
    int total = 0;
    int skipped = 0;
    string word, word_fined;
    while (cin >> word) {
        word_fined = processWord(word);
        if (!word_fined.empty()) {
            ++total;
            if (skipList.find(word_fined) == skipList.end()) {
                ++wordCounts[word_fined];
            } else {
                ++skipped;
            }
        }
    }
    cout << "Total words in document: " << total << endl;
    cout << "Unique words: " << wordCounts.size() << endl;
    cout << "Skipped " << skipped << " words." << endl;
}


/*
 * This helper-function outputs the generated word-list in descending order
 * of count.  The function uses an STL associative container to sort the words
 * by how many times they appear.  Because multiple words can have the same
 * counts, a multimap is used.
 */
void outputWordsByCount(map<string, int>& wordCounts)
{
    multimap<int, string, greater<int> > sortByCount;

    for (auto wIter = wordCounts.begin(); wIter != wordCounts.end(); wIter++)
        sortByCount.insert(pair<int, string> (wIter->second, wIter->first));

    for (auto cIter = sortByCount.begin(); cIter != sortByCount.end(); cIter++)
        cout << cIter->second << '\t' << cIter->first << endl;
}

