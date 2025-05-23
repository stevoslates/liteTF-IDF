#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include "ingest.h"
#include "utils.h"
#include "data.h"

std::unordered_map<int, std::string> docIdToFileName;
std::unordered_map<int, std::unordered_map<std::string, int>> wordFreqPerDoc;
std::unordered_map<std::string, int> docFreq;
std::unordered_map<int, int> docLengths;


int readFile(int docID, const std::string& filename) {
    std::ifstream myfile(filename);
    std::string myline;

    std::unordered_set<std::string> seenInThisDoc;

    int totalWords {0};
    if (myfile.is_open()) {
        while (std::getline(myfile, myline)) {
            std::vector<std::string> tokens = tokenize(myline); //tokenize each line
            totalWords += tokens.size();
            for (const auto& s : tokens) { //populate frequency maps
                wordFreqPerDoc[docID][s] += 1;
                if (!seenInThisDoc.contains(s)) {
                    docFreq[s] += 1;
                    seenInThisDoc.insert(s);
                }
            }
        }
    } else {
        std::cout << "Couldn't open file\n";
        return 1;
    }

    docIdToFileName[docID] = filename;
    docLengths[docID] = totalWords;
    return 0;
}

std::vector<std::string> tokenize(const std::string& curline) {
    std::vector<std::string> tokens;
    std::string word;
    for (char c : curline) {
        if (std::isalnum(c)) {
            word += std::tolower(c);
        } else if (!word.empty()) {
            tokens.push_back(word);
            word.clear();
        }
    }
    // Add the last word if the line ends with an alphanumeric character
    if (!word.empty()) {
        tokens.push_back(word);
    }
    
    return tokens;
}
