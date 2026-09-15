#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <vector>


namespace producer_consumer {

template<typename T>
class BlockingQueue {
public:
    explicit BlockingQueue(size_t size) : m_max_size_(size), m_stop_flag_(false) {}

    // 生产者
    bool push(const T& item) {
        std::unique_lock<std::mutex> lock(m_mtx_);
    
        // 队列满且未停止，则等待 “队列不满”
        m_not_full_.wait(lock, [this](){
            return m_stop_flag_ || (m_queue_.size() < m_max_size_);
        });

        if (m_stop_flag_) {
            return false;
        }

        m_queue_.emplace(item);
        m_not_empty_.notify_one(); // 通知消费者
        return true;
    }

    // 消费者：取出元素；stop 时返回 false
    bool pop(T& out_item) {
        std::unique_lock<std::mutex> lock(m_mtx_);

        // 队列为空且未停止，则等待 “队列非空”
        m_not_empty_.wait(lock, [this](){
            return m_stop_flag_ || !m_queue_.empty();
        });

        if (m_stop_flag_ && m_queue_.empty()) {
            return false;
        }

        out_item = m_queue_.front();
        m_queue_.pop();
        m_not_full_.notify_one();
        return true;
    }

    void stop() {
        std::unique_lock<std::mutex> lock(m_mtx_);
        m_stop_flag_ = true;
        m_not_full_.notify_all();
        m_not_empty_.notify_all();
    }
    

private:
    std::queue<T> m_queue_;
    size_t m_max_size_;

    std::mutex m_mtx_;
    std::condition_variable m_not_full_;     // 队列不为满，可以生产
    std::condition_variable m_not_empty_;    // 队列不为空，可以消费
    bool m_stop_flag_;                       // 停止标记
};


} // namespace producer_consumer


int main() {
    producer_consumer::BlockingQueue<int> queue(5);

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // 无限生产者
    producers.emplace_back([&queue](){
        int id = 0;
        while(true) {
            if(!queue.push(id)){
                std::cout << "[生产者] 收到停止信号，退出\n";
                break; // 必须判断push返回值退出！！
            }
            std::cout << "[生产者] produce " << id << "\n";
            id++;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });

    // 消费者
    consumers.emplace_back([&queue](){
        int val{};
        while(queue.pop(val)){
            std::cout << "[消费者] consume " << val << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
        std::cout << "[消费者] 队列已停止且空，退出\n";
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "\n>>>调用stop\n";
    queue.stop();

    for(auto& t : producers) if(t.joinable()) t.join();
    for(auto& t : consumers) if(t.joinable()) t.join();

    std::cout << "all thread joined\n";
    return 0;
}
