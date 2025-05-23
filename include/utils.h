#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <unordered_map>
#include <iostream>

template<typename T>
void printVector(const std::vector<T>& vec) {
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

template<typename K, typename V>
void printMap(const std::unordered_map<K, V>& map) {
    for (const auto& [key, val] : map) {
        std::cout << key << " => " << val << "\n";
    }
}

template<typename OuterKey, typename InnerKey, typename InnerVal>
void printNestedMap(const std::unordered_map<OuterKey, std::unordered_map<InnerKey, InnerVal>>& nestedMap) {
    for (const auto& [outerKey, innerMap] : nestedMap) {
        std::cout << outerKey << ":\n";
        for (const auto& [innerKey, val] : innerMap) {
            std::cout << "  " << innerKey << " => " << val << "\n";
        }
        std::cout << "----------------------\n";
    }
}


#endif
