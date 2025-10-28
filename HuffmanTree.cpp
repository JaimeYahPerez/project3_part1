#include "HuffmanTree.h"

HuffmanTree HuffmanTree::buildFromCounts(const std::vector<std::pair<std::string, int>> &counts) {
    HuffmanTree ht;

    std::vector<TreeNode*> nodes;
    for (const auto& [w, c] : counts) {
        auto* leaf = new TreeNode(c);
        leaf->setKey(w);
        nodes.push_back(leaf);
    }

    PriorityQueue pq(std::move(nodes));

    // Edge cases: 0 or 1 leaves
    if (pq.size() == 0) {
        ht.root_ = nullptr;
        return ht;
    }
    if (pq.size() == 1) {
        ht.root_ = pq.extractMin();
        return ht;
    }

    while (pq.size() >= 2) {
        TreeNode* a = pq.extractMin();
        TreeNode* b = pq.extractMin();

        auto* parent = new TreeNode(a->value() + b->value());
        parent->setKey(a->key() < b->key() ? a->key() : b->key());

        parent->leftSubtree(a);
        parent->rightSubtree(b);

        pq.insert(parent);
    }

    ht.root_ = pq.extractMin();
    return ht;
}

HuffmanTree::~HuffmanTree() {
    destroy(root_);
    root_ = nullptr;
}

void HuffmanTree::assignCodes(std::vector<std::pair<std::string, std::string>> &out) const {
    out.clear();
    std::string prefix;
    assignCodesDFS(root_, prefix, out);
}

error_type HuffmanTree::writeHeader(std::ostream &os) const {
    if (!os.good()) {
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }

    std::string prefix;
    writeHeaderPreorder(root_, os, prefix);
    if (os.fail()) {
        return FAILED_TO_WRITE_FILE;
    }
    return NO_ERROR;
}

error_type HuffmanTree::encode(const std::vector<std::string> &tokens, std::ostream &os_bits, int wrap_cols) const {
    if (!os_bits.good()) {
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }


    //Vector instead of a map to simplify work key, code
    std::vector<std::pair<std::string,std::string>> codes;
    assignCodes(codes);

    int col = 0;
    for (const auto& w : tokens) {
        const std::string* bits = nullptr;
        for (const auto& kv : codes) {
            if (kv.first == w) {
                bits = &kv.second;
                break;
            }
        }
        if (!bits){
            return FAILED_TO_WRITE_FILE;
        }

        for (char b : *bits) {
            os_bits.put(b);
            if (!os_bits) {
                return FAILED_TO_WRITE_FILE;
            }
            if (++col == wrap_cols) {
                os_bits.put('\n');
                if (!os_bits) {
                    return FAILED_TO_WRITE_FILE;
                }
                col = 0;
            }
        }
    }
    if (col != 0) {
        os_bits.put('\n');
        if (!os_bits) {
            return FAILED_TO_WRITE_FILE;
        }
    }
    return NO_ERROR;
}

void HuffmanTree::destroy(TreeNode *n) noexcept {
    if (!n) {
        return;
    }
    destroy(n->leftSubtree());
    destroy(n->rightSubtree());
    delete n;
}

void HuffmanTree::assignCodesDFS(const TreeNode *n, std::string &prefix, std::vector<std::pair<std::string, std::string>> &out) {
    if (!n) {
        return;
    }

    if (n -> leftSubtree() == nullptr && n->rightSubtree() == nullptr) {
        const std::string code = prefix.empty() ? "0" : prefix; // single word edge case
        out.emplace_back(n->key(), code);
        return;
    }

    prefix.push_back('0');
    assignCodesDFS(n->leftSubtree(), prefix, out);


    prefix.back() = '1';
    assignCodesDFS(n->rightSubtree(), prefix, out);

    prefix.pop_back();
}

void HuffmanTree::writeHeaderPreorder(const TreeNode *n, std::ostream &os, std::string &prefix) {
    if (!n) {
        return;
    }

    if (n->leftSubtree() == nullptr && n->rightSubtree() == nullptr) {
        const std::string code = prefix.empty() ? "0" : prefix;
        os << n->key() << ' ' << code << '\n';
        return;
    }

    prefix.push_back('0');
    writeHeaderPreorder(n->leftSubtree(), os, prefix);

    prefix.back() = '1';
    writeHeaderPreorder(n->rightSubtree(), os, prefix);

    prefix.pop_back();
}
