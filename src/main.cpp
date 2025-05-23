#include <iostream>
#include <fstream>
#include <string>
#include "ingest.h"
#include "utils.h"
#include "tfidf.h"
#include "data.h"
#include <filesystem>
namespace fs = std::filesystem;

void printUsage() {
    std::cout << "Usage:\n"
              << "  ./main -f [file1.txt file2.txt ...]         Ingest individual files\n"
              << "  ./main -F [folder/]                         Ingest all .txt files in a folder\n"
              << "Optional flags:\n"
              << "  -D                                          Display contents of matched files\n"
              << "  -K [number]                                 Show only top K results\n"
              << "  --help                                      Show this help message\n";
}

std::vector<std::string> getTxtFilesInFolder(const std::string& folderPath) {
    std::vector<std::string> txtFiles;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            txtFiles.push_back(entry.path().string());
        }
    }
    return txtFiles;
}

bool showDocs = false;
int topK = -1;

int main(int argc, char* argv[]) {
    if (argc < 2 || std::string(argv[1]) == "--help") {
        printUsage();
        return 0;
    }

    std::vector<std::string> filenames;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-f") {
            while (++i < argc && !std::string(argv[i]).starts_with("-")) {
                if (!std::string(argv[i]).ends_with(".txt")) {
                    std::cerr << "Invalid file: " << argv[i] << "\n";
                    return 1;
                }
                filenames.push_back(argv[i]);
            }
            --i;
        } else if (arg == "-F") {
            if (i + 1 >= argc) {
                std::cerr << "Folder path required after -F\n";
                return 1;
            }
            filenames = getTxtFilesInFolder(argv[++i]);
        } else if (arg == "-D") {
            showDocs = true;
        } else if (arg == "-K") {
            if (i + 1 >= argc) {
                std::cerr << "Value required after -K\n";
                return 1;
            }
            topK = std::stoi(argv[++i]);
        } else {
            std::cerr << "Unknown flag: " << arg << "\n";
            printUsage();
            return 1;
        }
    }

    if (filenames.empty()) {
        std::cerr << "No files found to ingest.\n";
        return 1;
    }

    std::cout << "Ingesting Documents...\n"; 
    int i = 0;
    for (const auto& filename : filenames) {
        int err = readFile(i++, filename);
        if (err==1) {
            std::cerr << "Issue Reading Document.\n";
        }
    }
    std::cout << "Completed!\n";

    while (true) {
        std::string query;
        std::cout << "\nPlease Enter Query (or type 'exit'): ";
        std::getline(std::cin, query);
        if (query == "exit") break;

        std::vector<std::string> q_tokens = tokenize(query);
        auto scores = getScores(q_tokens);

        if (topK > 0 && topK < static_cast<int>(scores.size())) {
            scores.resize(topK);
        }

        for (const auto& [docId, score] : scores) {
            std::cout << "[" << score << "] - " << docIdToFileName[docId] << "\n";

            if (showDocs) {
                std::ifstream file(docIdToFileName[docId]);
                std::cout << "---- Begin Document ----\n";
                std::string line;
                while (std::getline(file, line)) {
                    std::cout << line << "\n";
                }
                std::cout << "---- End Document ----\n";
            }
        }
    }

    return 0;
}