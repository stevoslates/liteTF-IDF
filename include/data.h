#ifndef DATA_H
#define DATA_H

#include <string>
#include <unordered_map>

extern std::unordered_map<int, std::string> docIdToFileName;
extern std::unordered_map<int, std::unordered_map<std::string, int>> wordFreqPerDoc;
extern std::unordered_map<std::string, int> docFreq;
extern std::unordered_map<int, int> docLengths;

#endif
