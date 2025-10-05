//
// Created by Ali Kooshesh on 9/27/25.
//

#include "Scanner.hpp"

#include <utility>
#include <iostream>
#include <fstream>

#include "utils.hpp"

static bool is_ascii(int ch) {
    auto u = static_cast<unsigned char>(ch);
    return u <= 0x7F;
}
static bool is_ascii_letter(int ch) {
    if (!is_ascii(ch)) {
        return false;
    }
    auto u = static_cast<unsigned char>(ch);
    return (u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z');
}

static char to_ascii_lower(char ch) {
    return (ch >= 'A' && ch <= 'Z') ? static_cast<char>(ch - 'A' + 'a') : ch;
}
static bool is_apostrophe(int ch) {
    return ch == '\'';
}


Scanner::Scanner(std::filesystem::path inputPath) {
    // You complete this...
    const std::string base = inputPath.stem().string();
    inputPath_ = std::filesystem::path(base + ".txt");

}
error_type Scanner::tokenize(std::vector<std::string>& words) {
    words.clear();

    const std::filesystem::path parent = inputPath_.parent_path();
    if (!parent.empty() && !std::filesystem::exists(parent)) {
        return DIR_NOT_FOUND;
    }
    if (!std::filesystem::exists(inputPath_) || !std::filesystem::is_regular_file(inputPath_)) {
        return FILE_NOT_FOUND;
    }

    std::ifstream in(inputPath_, std::ios::binary);
    if (!in.is_open()) {
        return UNABLE_TO_OPEN_FILE;
    }

    while (in) {
        std::string w = readWord(in);
        if (w.empty()) {
            break;
        }
        words.push_back(std::move(w));
    }
    return NO_ERROR;
}

std::string Scanner::readWord(std::istream &in) {
    std::string tok;
    for (;;) {
        int c = in.get();
        if (c == EOF) {
            return tok;
        }

        if (!is_ascii(c)) {                // non-ASCII => separator
            if (!tok.empty()) return tok;
            continue;
        }

        char ch = static_cast<char>(c);

        if (is_ascii_letter(ch)) {         // letters extend token
            tok.push_back(to_ascii_lower(ch));
            continue;
        }

        if (is_apostrophe(ch)) {
            if (tok.empty()) {
                continue;
            }     // drop leading apostrophes
            int nxt = in.peek();
            if (nxt != EOF && is_ascii(nxt) && is_ascii_letter(nxt)) {
                tok.push_back('\'');       // keep only if between letters
                in.get();
                tok.push_back(to_ascii_lower(static_cast<char>(nxt)));
                continue;
            } else {
                return tok;                // trailing/apostrophe not followed by letter
            }
        }
        if (!tok.empty()) return tok;
    }
}
