# 线程安全的生产者消费者队列

使用 C++17 实现一个支持多生产者、多消费者的有界阻塞队列。

## 接口

```cpp
#include <cstddef>
#include <optional>

template <typename T>
class BoundedBlockingQueue {
public:
    explicit BoundedBlockingQueue(std::size_t capacity);

    BoundedBlockingQueue(const BoundedBlockingQueue&) = delete;
    BoundedBlockingQueue& operator=(const BoundedBlockingQueue&) = delete;

    bool Put(T value);
    std::optional<T> Take();
    void Close();
    std::size_t Size() const;
};
```

## 基本语义

- 使用 C++17，队列遵循 FIFO。
- `capacity` 必须大于零，否则构造失败。
- 队列满时，`Put` 阻塞，直到有空间或队列关闭。
- `Put` 成功写入返回 `true`；队列已经关闭或等待期间被关闭时返回 `false`。
- 队列为空且尚未关闭时，`Take` 阻塞，直到有元素或队列关闭。
- `Close` 幂等，并唤醒所有等待中的生产者和消费者。
- 关闭后不接受新的写入。
- 关闭时已经入队的元素仍可继续被取出。
- 队列关闭且缓冲区排空后，`Take` 返回 `std::nullopt`。
- `Size` 返回调用时的元素数量，并且必须线程安全。
- 当前阶段不要求超时、取消和严格公平性。
- 调用方必须在队列析构前停止并回收所有仍可能访问队列的线程。

## 正确性要求

- 不丢失、不重复消费已经成功写入的元素。
- 不出现数据竞争、死锁或永久无法唤醒的等待者。
- 禁止通过忙等或循环休眠等待队列状态变化。
- 忽略阻塞等待时间，`Put` 和 `Take` 的时间复杂度应为 `O(1)`。
- 空间复杂度应为 `O(capacity)`。
- 需要解释所选同步方案的公平性和关闭语义。

## 验证建议

在 Linux 或 WSL 中编译：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread solution.cpp -o producer_consumer
./producer_consumer
```

使用支持 ThreadSanitizer 的 Linux G++ 检查数据竞争：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread \
    -fsanitize=thread -g solution.cpp -o producer_consumer_tsan
./producer_consumer_tsan
```
