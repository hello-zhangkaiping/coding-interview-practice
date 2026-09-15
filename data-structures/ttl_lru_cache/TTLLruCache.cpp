#include <iostream>
#include <unordered_map>
#include <chrono>
#include <optional>
#include <random>
#include <queue>

namespace TTLLRU {
using TimePoint = std::int64_t; // 毫秒时间戳

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
        cleanHeapTop();

        auto it = cache_.find(key);
        if (it != cache_.end()) {
            // 找到了，修改值并重置节点在 链表 中的位置
            DLinkNode* node = it->second;
            auto now = nowMs();
            // TTL 过期判断
            if (node->expire_ms <= now) {
                remove(node);
                cache_.erase(node->key);
                delete node;
                return std::nullopt;
            }
            int res = node->val;
            moveToHead(node);
            return std::make_optional(res);
        }
        return std::nullopt;
    }
    bool insert(int key, int val, TimePoint ttl_ms) {
        cleanHeapTop();

        auto now = nowMs();
        TimePoint newExpire = now + ttl_ms;

        auto it = cache_.find(key);
        if (it != cache_.end()) {
            // key 已存在，更新值与过期时间，压新堆顶（旧项变成脏页数据留在堆）
            DLinkNode* node = it->second;
            node->val = val;
            node->expire_ms = newExpire;
            moveToHead(node);
            m_min_heap_.emplace(newExpire, key);
            return true;
        }
        DLinkNode* newNode = new DLinkNode(key, val, newExpire);
        addToHead(newNode);
        cache_[key] = newNode;
        m_min_heap_.emplace(newExpire, key);

        // 容量淘汰
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
            std::cout << cur->key << ":" << cur->val << "(exp:" << cur->expire_ms << ")->";
            cur = cur->next;
        }
        std::cout << "nullptr" << std::endl;
    }
    void build(int size, TimePoint ttl = 10000) {
        for (int i = 0; i < size; i++) {
            int random = std::rand();
            insert(i, i + random, ttl);
        }
    }

private:
    struct DLinkNode {
        int key;
        int val;
        TimePoint expire_ms;
        DLinkNode* prev;
        DLinkNode* next;
        DLinkNode(int key_ = -1, int val_ = -1, TimePoint expire_ms_ = 0) 
                : key(key_), val(val_), prev(nullptr), next(nullptr), expire_ms(expire_ms_) {}
    };

    struct HeapItem {
        TimePoint expire_ms;
        int key;
        HeapItem(TimePoint expire_ms_, int key_) : expire_ms(expire_ms_), key(key_) {}
    };

    struct HeapCmp {
        bool operator()(const HeapItem& a, const HeapItem& b) {
            return a.expire_ms > b.expire_ms;
        }
    };

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

    // 获取当前时间 ms
    static TimePoint nowMs() {
        auto tp = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    }
    // 清理堆顶：处理过期和堆脏条目
    void cleanHeapTop() {
        auto now = nowMs();
        while (!m_min_heap_.empty()) {
            const auto& topItem = m_min_heap_.top();
            if (topItem.expire_ms > now) {
                break;
            }
            int topKey = topItem.key;
            auto mapIt = cache_.find(topKey);

            // 脏条目分为两种情况：key 不存在；或者节点真实过期时间和堆记录不一致（被更新过ttl）
            if (mapIt == cache_.end() || mapIt->second->expire_ms != topItem.expire_ms) {
                m_min_heap_.pop();
                continue;
            }

            // 真正过期，删除节点
            DLinkNode* del = mapIt->second;
            remove(del);
            cache_.erase(del->key);
            delete del;
            m_min_heap_.pop();
        }
    }

private:
    int capacity_;
    std::unordered_map<int, DLinkNode*> cache_;
    DLinkNode* head_;
    DLinkNode* tail_;
    std::priority_queue<HeapItem, std::vector<HeapItem>, HeapCmp> m_min_heap_;
};

} // namespace TTLLRU


int main() {
    TTLLRU::LRUCache lru1(10);
    lru1.build(10);
    lru1.print();
    lru1.insert(15, 128, 10000);
    lru1.insert(65, 54, 20000);
    lru1.print();
    return 0;
}