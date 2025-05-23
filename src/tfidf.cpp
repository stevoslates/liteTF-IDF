#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include "data.h"
#include "utils.h"

std::vector<std::string> getOrderedQueryTokens(const std::vector<std::string>& q_tokens) {
    std::unordered_set<std::string> seen;
    std::vector<std::string> ordered;

    for (const auto& token : q_tokens) {
        if (!seen.contains(token)) {
            ordered.push_back(token);
            seen.insert(token);
        }
    }
    return ordered;
}

std::vector<double> getQueryVec(const std::vector<std::string>& orderedTokens, const std::unordered_map<std::string, int>& queryFreq, int totalQueryTerms, int totalDocs) {
    std::vector<double> vec;
    for (const auto& token : orderedTokens) {
        int tfCount = queryFreq.at(token);
        double tf = (tfCount > 0) ? (1.0 + std::log(tfCount)) : 0.0;

        int df = docFreq.contains(token) ? docFreq[token] : 0;
        double idf = std::log(1.0 + static_cast<double>(totalDocs) / (1.0 + df));

        vec.push_back(tf * idf);
    }
    return vec;
}


std::unordered_map<int, std::vector<double>> getDocVecs(const std::vector<std::string>& orderedTokens,int totalDocs) {
    std::unordered_map<int, std::vector<double>> docVectors;

    for (const auto& [docId, _] : docIdToFileName) {
        const auto& wordFreq = wordFreqPerDoc[docId];
        //int totalTerms = docLengths[docId];

        std::vector<double> vec(orderedTokens.size(), 0.0); // pre-fill with 0s, fixed size, avoid push_back() overhead as we know size.

        for (size_t i = 0; i < orderedTokens.size(); ++i) {
            const std::string& token = orderedTokens[i];

            if (!wordFreq.contains(token)) {
                continue; // skip unless token exists in this doc
            }

            int tfCount = wordFreq.at(token);
            // log-scaled TF
            double tf = (tfCount > 0) ? (1.0 + std::log(tfCount)) : 0.0;

            // smoothed IDF
            int df = docFreq.contains(token) ? docFreq[token] : 0;
            double idf = std::log(1.0 + static_cast<double>(totalDocs) / (1.0 + df));
            vec[i] = tf * idf;
        }

        docVectors[docId] = std::move(vec);
    }

    return docVectors;
}

//can improve - if we know the places that the queryVec is non_zero we only need to check those indicies
double calcCosine(const std::vector<double>& queryVec, const std::vector<double>& docVec) {
    double dot = 0.0;
    double normQuery = 0.0;
    double normDoc = 0.0;

    for (size_t i = 0; i < queryVec.size(); ++i) {
        dot += queryVec[i] * docVec[i];
        normQuery += queryVec[i] * queryVec[i];
        normDoc += docVec[i] * docVec[i];
    }

    if (normQuery == 0.0 || normDoc == 0.0) {
        return 0.0;  // avoid division by zero
    }

    return dot / (std::sqrt(normQuery) * std::sqrt(normDoc));
}

std::vector<std::pair<int, double>> getScores(const std::vector<std::string>& query_tokens) {
    //order query tokens for easy cosine calculation
    std::vector<std::string> orderedQueryTokens = getOrderedQueryTokens(query_tokens);

    std::unordered_map<std::string, int> queryFreq;
    for (const auto& token : query_tokens) {
        queryFreq[token]++;
    }

    int totalQueryTerms = query_tokens.size();
    int totalDocs = docIdToFileName.size();

    std::vector<double> queryVec = getQueryVec(orderedQueryTokens, queryFreq, totalQueryTerms, totalDocs);
    

    std::unordered_map<int, std::vector<double>> docVecs = getDocVecs(orderedQueryTokens, totalDocs);

    //Now we need to do cosine and also rank them so we can return them like [TF-IDF Score] - DocName
    std::vector<std::pair<int, double>> scores;
    for (const auto& [docid, _]: docVecs) {
        double cosScore = calcCosine(queryVec, docVecs[docid]);
        scores.emplace_back(docid, cosScore);
    }

    //sort desc on cos score
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
              return a.second > b.second;
          });

    return scores;
}
