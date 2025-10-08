//
// Created by Ali Kooshesh on 9/27/25.
//

#include "Scanner.hpp"

#include <utility>
#include <iostream>
#include <fstream>

#include "utils.hpp"

bool is_ascii(int ch) {
    auto u = static_cast<char>(ch);
    return u <= 127;
}
bool is_ascii_letter(int ch) {
    if (!is_ascii(ch)) {
        return false;
    }
    auto u = static_cast<char>(ch);
    return (u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z');
}

char to_ascii_lower(int ch) {
    return (ch >= 'A' && ch <= 'Z') ? (ch - 'A' + 'a') : ch;
}
bool is_apostrophe(int ch) {
    return ch == '\'';
}


Scanner::Scanner(std::filesystem::path inputPath) {
    // You complete this...
    std::string base = inputPath.stem().string();
    inputPath_ = inputPath.parent_path() / (base + ".txt");

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

    std::ifstream in(inputPath_);
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

error_type Scanner::tokenize(std::vector<std::string> &words, const std::filesystem::path &outputFile) {
    error_type err = tokenize(words);
    if (err != NO_ERROR) {
        return err;
    }

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }
    if (words.empty()) {
        out << '\n';
        if (!out) {
            return FAILED_TO_WRITE_FILE;
        }
    } else {
        size_t i = 0;
        size_t n = words.size();
        while (i < n) {
            out << words[i] << '\n';
            if (!out) {
                return FAILED_TO_WRITE_FILE;
            }
            i++;
        }
    }
    out.flush();
    if (!out.good()) {
        return FAILED_TO_WRITE_FILE;
    }

    return NO_ERROR;

}

std::string Scanner::readWord(std::istream &in) {
    std::string tok;
    int c = in.get();
    while (true) {
        if (c == EOF) {
            return tok;
        }

        if (!is_ascii(c)) {
            if (!tok.empty()) {
                return tok;
            }
            c = in.get();
            continue;
        }

        if (is_apostrophe(c)) {
            if (tok.empty()) {
                c = in.get();
                continue;
            }
            int nxt = in.peek();
            if (nxt != EOF && is_ascii(nxt) && is_ascii_letter(nxt)) {
                tok.push_back('\'');
                in.get();
                tok.push_back(to_ascii_lower(nxt));
                c = in.get();
                continue;
            } else {
                return tok;
            }
        }

        if (is_ascii_letter(c)) {
            tok.push_back(to_ascii_lower(c));
            c = in.get();
            continue;
        }

        if (!tok.empty()) {
            return tok;
        }
        c = in.get();
    }
}
