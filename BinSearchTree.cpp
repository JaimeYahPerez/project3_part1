#include "BinSearchTree.hpp"

//helper function to streamline comparison tests
int cmp(std::string_view a, std::string_view b) {
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return  1;
    }
    return 0;
}

BinSearchTree::~BinSearchTree() {
    destroy(root_);
    root_ = nullptr;
}

void BinSearchTree::insert(const std::string &word) {
    root_ = insertHelper(root_, word);
}

void BinSearchTree::bulkInsert(const std::vector<std::string> &words) {
    for (const auto& w : words) {
        root_ = insertHelper(root_, w);
    }
}

void BinSearchTree::destroy(TreeNode* node) noexcept {
    if (!node) {
        return;
    }
    destroy(node->leftSubtree());
    destroy(node->rightSubtree());
    delete node;
}

bool BinSearchTree::contains(std::string_view word) const noexcept {
    return findNode(root_, word) != nullptr;
}

std::optional<int> BinSearchTree::countOf(std::string_view word) const noexcept {
    const TreeNode* node = findNode(root_, word);
    if (!node) {
        return std::nullopt;
    }
    return node->value();
}

void BinSearchTree::inorderCollect(std::vector<std::pair<std::string, int>> &out) const {
    out.clear();
    inorderHelper(root_,out);
}

std::size_t BinSearchTree::size() const noexcept {
    return sizeHelper(root_);
}

unsigned BinSearchTree::height() const noexcept {
    return heightHelper(root_);
}


TreeNode * BinSearchTree::insertHelper(TreeNode *node, const std::string &word) {
    if (!node) {
        /* Base case/new node */
        auto* n = new TreeNode(1);
        n->setKey(word);
        return n;
    }

    int test = cmp(word,node->key());
    switch (test) {
        case(0):
            node -> value() += 1;
            return node;
        case(-1):
            node->leftSubtree(insertHelper(node->leftSubtree(), word));
            return node;
        case(1):
            node->rightSubtree(insertHelper(node->rightSubtree(), word));
            return node;
        default:
            return node;
    }
}

const TreeNode * BinSearchTree::findNode(const TreeNode *node, std::string_view word) noexcept {
    if (!node) {
        return nullptr;
    }
    int test = cmp(word, node->key());
    switch (test) {
        case(0):
            return node;
        case(1):
            return findNode(node->rightSubtree(), word);
        case(-1):
            return findNode(node->leftSubtree(), word);
        default:
            return node;
    }
}

void BinSearchTree::inorderHelper(const TreeNode *node, std::vector<std::pair<std::string, int>> &out) {
    if (!node) {
        return;
    }

    inorderHelper(node->leftSubtree(), out);
    out.emplace_back(node->key(), node->value());
    inorderHelper(node->rightSubtree(), out);
}

std::size_t BinSearchTree::sizeHelper(const TreeNode *node) noexcept {
    if (!node) {
        return 0;
    }
    return 1 + sizeHelper(node->leftSubtree()) + sizeHelper(node->rightSubtree());
}

unsigned BinSearchTree::heightHelper(const TreeNode *node) noexcept {
    if (!node) {
        return 0;
    }
    const unsigned left = heightHelper(node->leftSubtree());
    const unsigned right = heightHelper(node->rightSubtree());
    return 1 + (left > right ? left : right);
}


