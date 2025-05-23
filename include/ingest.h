#ifndef INGEST_H
#define INGEST_H

#include <string>
#include <vector>
#include <unordered_map>

// shared global data structures
extern std::unordered_map<int, std::string> docIdToFileName;
extern std::unordered_map<int, std::unordered_map<std::string, int>> wordFreqPerDoc;
extern std::unordered_map<std::string, int> docFreq;
extern std::unordered_map<int, int> docLengths;

// reads and tokenizes a file into the global structures
int readFile(int docID, const std::string& filename);

// tokenizes a single line into lowercase alphanumeric words
std::vector<std::string> tokenize(const std::string& curline);

#endif
