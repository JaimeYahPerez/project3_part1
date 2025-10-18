#include "PriorityQueue.h"

bool PriorityQueue::higherPriority(const TreeNode* a, const TreeNode* b) noexcept {
    const int af = a->value();
    const int bf = b->value();

    // higher freq first
    if (af != bf) {
        return af > bf;
    }

    return a->key() < b->key();                   // tie: smaller word first
}

bool PriorityQueue::isSorted() const {
    for (std::size_t i = 1; i < items_.size(); i++) {
        if (!higherPriority(items_[i-1], items_[i])) {
            return false;
        }
    }
    return true;
}

PriorityQueue::PriorityQueue(std::vector<TreeNode *> nodes) {
    items_ = std::move(nodes);

    int n = items_.size();
    if (n <= 1) {
        return;
    }

    int i = 1;
    while (i < n) {
        TreeNode* item = items_[i];
        int j = i - 1;

        while (j >= 0 && higherPriority(item, items_[j])){
            items_[j + 1] = items_[j];
            j--;
        }
        items_[j + 1] = item;
        i++;
    }

}

std::size_t PriorityQueue::size() const noexcept {
    return items_.size();
}

bool PriorityQueue::empty() const noexcept {
    return items_.empty();
}


TreeNode * PriorityQueue::findMin() const noexcept {
    return items_.empty() ? nullptr : items_.back();
}

TreeNode * PriorityQueue::extractMin() noexcept {
    if (items_.empty()) {
        return nullptr;
    }
    TreeNode* out = items_.back();
    items_.pop_back();
    return out;
}

void PriorityQueue::deleteMin() noexcept {
    if (!items_.empty()) {
        items_.pop_back();
    }
    return;
}

void PriorityQueue::insert(TreeNode *node) {
    if (!node) {
        return;
    }

    items_.push_back(node);
    int n = items_.size();
    if (n == 1) {
        return;
    }

    int j = n - 2;
    while (j >= 0 && higherPriority(node, items_[j])){
        items_[j + 1] = items_[j];
        j--;
    }
    items_[j + 1] = node;
}

void PriorityQueue::print(std::ostream &os) const {
    for (const TreeNode* n : items_) {
        os << n->key() << ' ' << n->value() << '\n';
    }
}

