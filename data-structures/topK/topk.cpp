#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace topK {
class TopKManager {
public:
    void add(int val) {
        data_.emplace_back(val);
    }
    std::vector<int> getTopK(size_t k) {
        std::vector<int> res;
        if (k == 0 || data_.empty()) return res;
        k = std::min(k, size());

        std::unordered_map<int, int> map;
        for (int x : data_) {
            map[x]++;
        }

        using PairT = std::pair<int, int>;
        std::priority_queue<PairT,
                            std::vector<PairT>,
                            std::greater<PairT>> pq;
        for (auto& item : map) {
            int num = item.first;
            int cnt = item.second;
            pq.emplace(cnt, num);
            if (pq.size() > k) {
                pq.pop();
            }
        }

        while (!pq.empty()) {
            res.emplace_back(pq.top().second);
            pq.pop();
        }

        return res;
    }    
    const std::vector<int>& getAllData() const {
        return data_;
    }
    size_t size() const {
        return data_.size();
    }
    void clear() {
        data_.clear();
    }

private:
    std::vector<int> data_;
};

} // namespace topK

int main() {
    topK::TopKManager tk;
    std::vector<int> arr{3,1,4,1,5,9,2,6,8,7};
    for(auto v : arr) tk.add(v);
    auto top3 = tk.getTopK(3);
    std::cout << "top3: ";
    for(auto x : top3) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}