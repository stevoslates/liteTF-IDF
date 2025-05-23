#ifndef TFIDF_H
#define TFIDF_H

#include <vector>
#include <string>
#include <unordered_map>
#include <utility>

std::vector<std::string> getOrderedQueryTokens(const std::vector<std::string>& q_tokens);

std::vector<double> getQueryVec(const std::vector<std::string>& orderedTokens,
                                const std::unordered_map<std::string, int>& queryFreq,
                                int totalQueryTerms,
                                int totalDocs);

std::unordered_map<int, std::vector<double>> getDocVecs(const std::vector<std::string>& orderedTokens,
                                                        int totalDocs);

double calcCosine(const std::vector<double>& queryVec, const std::vector<double>& docVec);

std::vector<std::pair<int, double>> getScores(const std::vector<std::string>& query_tokens);

#endif
