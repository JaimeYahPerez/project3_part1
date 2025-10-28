#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>


#include "Scanner.hpp"
#include "utils.hpp"
#include "BinSearchTree.hpp"
#include "PriorityQueue.h"
#include "HuffmanTree.h"

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
    //input file declarations
    const std::string inputFileName = inputPath.string();
    const std::string dirName = inputPath.parent_path().string();
    const std::string inputFileBaseName = baseNameWithoutTxt(inputPath.filename().string());

    // build the path to the .tokens output file.
    const std::string tokensPath = (inputPath.parent_path() / (inputFileBaseName + ".tokens")).string();
    const std::string freqPath   = (inputPath.parent_path() / (inputFileBaseName + ".freq")).string();
    const std::string hdrPath    = (inputPath.parent_path() / (inputFileBaseName + ".hdr")).string();
    const std::string codePath   = (inputPath.parent_path() / (inputFileBaseName + ".code")).string();

    // The next several if-statement make sure that the input file, the directory exist
    // and that the output file is writeable.
    if (error_type st; (st = regularFileExistsAndIsAvailable(inputFileName)) != NO_ERROR)
        exitOnError(st, inputFileName);
    if (error_type st; (st = directoryExists(dirName)) != NO_ERROR)
        exitOnError(st, dirName);
    if (error_type st; (st = canOpenForWriting(tokensPath)) != NO_ERROR)
        exitOnError(st, tokensPath);
    if (error_type st; (st = canOpenForWriting(freqPath)) != NO_ERROR)
        exitOnError(st, freqPath);
    if (error_type st; (st = canOpenForWriting(hdrPath)) != NO_ERROR)
        exitOnError(st, hdrPath);
    if (error_type st; (st = canOpenForWriting(codePath)) != NO_ERROR)
        exitOnError(st, codePath);

    //scanner calls
    std::vector<std::string> words;
    Scanner scanner(inputPath);
    if (error_type st; (st = scanner.tokenize(words)) != NO_ERROR)
        exitOnError(st, inputFileName);

    if (error_type st; (st = writeVectorToFile(tokensPath, words)) != NO_ERROR)
        exitOnError(st, tokensPath);

    //setup for the BST outputs
    BinSearchTree bst;
    bst.bulkInsert(words);

    std::vector<std::pair<std::string,int>> freqLex;
    bst.inorderCollect(freqLex);

    int minf = 0, maxf = 0;
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

    std::ofstream out(freqPath);
    if (!out.is_open()) {
        exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, freqPath);
    }

    //buffer to hold output from Priorityqueue run
    std::vector<std::pair<std::string,int>> buff;
    buff.reserve(pq.size());
    while (!pq.empty()) {
        TreeNode* temp = pq.extractMin();
        buff.emplace_back(temp->key(), temp->value());
        delete temp;
    }
    for (auto it = buff.rbegin(); it != buff.rend(); ++it) {
        out << std::setw(10) << it->second << ' ' << it->first << '\n';
    }

    //Huffman Tree declarations
    HuffmanTree ht = HuffmanTree::buildFromCounts(freqLex);

    std::ofstream hdr(hdrPath);
    if (!hdr.is_open()) {
        exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, hdrPath);
    }
    auto st = ht.writeHeader(hdr);
    if (st != NO_ERROR) {
        exitOnError(st, hdrPath);
    }

    std::ofstream code(codePath);
    if (!code.is_open()) {
        exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, codePath);
    }
    st = ht.encode(words, code, 80);
    if (st != NO_ERROR) {
        exitOnError(st, codePath);
    }

    return 0;
}
