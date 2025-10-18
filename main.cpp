#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>


#include "Scanner.hpp"
#include "utils.hpp"
#include "BinSearchTree.hpp"
#include "PriorityQueue.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    //fix to ensure that the input file is correctly found
    std::filesystem::path argPath(argv[1]);
    std::filesystem::path inputPath = argPath;
    if (inputPath.parent_path().empty()) {
        inputPath = std::filesystem::path("input_output") / inputPath.filename();
    }

    const std::string inputFileName = inputPath.string();
    const std::string dirName = std::string("input_output");
    const std::string inputFileBaseName = baseNameWithoutTxt(inputFileName);

    // build the path to the .tokens output file.
    const std::string wordTokensFileName = dirName + "/" + inputFileBaseName + ".tokens";


    // The next several if-statement make sure that the input file, the directory exist
    // and that the output file is writeable.
     if( error_type status; (status = regularFileExistsAndIsAvailable(inputFileName)) != NO_ERROR )
        exitOnError(status, inputFileName);


    if (error_type status; (status = directoryExists(dirName)) != NO_ERROR )
        exitOnError(status, dirName);

    if (error_type status; (status = canOpenForWriting(wordTokensFileName)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);


    std::vector<std::string> words;
    namespace fs = std::filesystem;
    fs::path tokensFilePath(wordTokensFileName); // create a file system path using the output file.

    auto fileToWords = Scanner(tokensFilePath);
    if( error_type status; (status = fileToWords.tokenize(words)) != NO_ERROR)
	    exitOnError(status, inputFileName);

    if (error_type status; (status = writeVectorToFile(wordTokensFileName, words)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);

    //setup for the BST outputs
    BinSearchTree bst;
    bst.bulkInsert(words);

    std::vector<std::pair<std::string,int>> freqLex;
    bst.inorderCollect(freqLex);

    int minf = 0;
    int maxf = 0;
    if (!freqLex.empty()) {
        minf = maxf = freqLex.front().second;
        for (const auto& p : freqLex) {
            minf = std::min(minf, p.second);
            maxf = std::max(maxf, p.second);
        }
    }

    std::cout << "BST height: "       << bst.height()   << "\n"
              << "BST unique words: " << bst.size()     << "\n"
              << "Total tokens: "     << words.size()   << "\n"
              << "Min frequency: "    << (freqLex.empty()?0:minf) << "\n"
              << "Max frequency: "    << (freqLex.empty()?0:maxf) << "\n";

    // Priority queue input: convert freqLex -> TreeNode* leaves
    std::vector<TreeNode *> leaves;
    leaves.reserve(freqLex.size());
    for (const auto& [w, c] : freqLex) {
        auto* n = new TreeNode(c);
        n->setKey(w);
        leaves.push_back(n);
    }
    PriorityQueue pq(std::move(leaves));

    // .freq: write in priority order (freq desc, word asc)
    const std::string freqPath = dirName + "/" + inputFileBaseName + ".freq";
    std::ofstream out(freqPath, std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, freqPath);
    }

    std::vector<std::pair<std::string,int>> buf;
    buf.reserve(pq.size());
    while (!pq.empty()) {
        TreeNode* m = pq.extractMin();
        buf.emplace_back(m->key(), m->value());
        delete m;
    }
    for (auto it = buf.rbegin(); it != buf.rend(); ++it) {
        out << std::setw(10) << it->second << ' ' << it->first << '\n'; // high -> low (10 spaces, num, string)
    }
    return 0;
}
