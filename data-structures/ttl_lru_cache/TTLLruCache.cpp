#include <iostream>
#include <unordered_map>
#include <chrono>
#include <optional>
#include <random>

struct DLinkNode {
    int key;
    int val;
    DLinkNode* prev;
    DLinkNode* next;
    DLinkNode(int key_ = -1, int val_ = -1) 
            : key(key_), val(val_), prev(nullptr), next(nullptr) {}
};

class LRUCache {
public:
    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

public:
    LRUCache(int capacity) {
        capacity_ = capacity;
        head_ = new DLinkNode();
        tail_ = new DLinkNode();
        head_->next = tail_;
        tail_->prev = head_;
    }
    ~LRUCache() {
        DLinkNode* cur = head_;
        while (cur) {
            DLinkNode* next = cur->next;
            delete cur;
            cur = next;
        }
    }
    std::optional<int> get(int key) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            // 找到了，修改值并重置节点在 链表 中的位置
            DLinkNode* node = it->second;
            int res = node->val;
            moveToHead(node);
            return std::make_optional(res);
        }
        return std::nullopt;
    }
    bool insert(int key, int val) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            // 找到了，修改值并重置节点在 链表 中的位置
            DLinkNode* node = it->second;
            node->val = val;
            moveToHead(node);
            return true;
        }
        DLinkNode* newNode = new DLinkNode(key, val);
        addToHead(newNode);
        cache_[key] = newNode;
        if (cache_.size() > capacity_) {
            DLinkNode* del = tail_->prev;
            remove(del);
            cache_.erase(del->key);
            delete del;
        }
        return true;
    }
    size_t size() {
        return cache_.size();
    }
    void print() {
        DLinkNode* cur = head_->next;
        while (cur != tail_) {
            std::cout << cur->key << ":" << cur->val << "->";
            cur = cur->next;
        }
        std::cout << "nullptr" << std::endl;
    }
    void build(int size) {
        for (int i = 0; i < size; i++) {
            int random = std::rand();
            insert(i, i + random);
        }
    }

private:
    void addToHead(DLinkNode* node) {
        node->next = head_->next;
        node->prev = head_;
        head_->next->prev = node;
        head_->next = node;
    }
    DLinkNode* remove(DLinkNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        return node;
    }
    void moveToHead(DLinkNode* node) {
        remove(node);
        addToHead(node);
    }

private:
    int capacity_;
    std::unordered_map<int, DLinkNode*> cache_;
    DLinkNode* head_;
    DLinkNode* tail_;
};


int main() {
    LRUCache lru1(10);
    lru1.build(10);
    lru1.print();
    lru1.insert(15, 128);
    lru1.insert(65, 54);
    lru1.print();
    return 0;
}