# 线程安全的生产者消费者队列

实现一个支持多生产者、多消费者的有界阻塞队列。

## 接口

```go
func NewBoundedQueue[T any](capacity int) (*BoundedQueue[T], error)
func (q *BoundedQueue[T]) Put(ctx context.Context, value T) error
func (q *BoundedQueue[T]) Take(ctx context.Context) (T, error)
func (q *BoundedQueue[T]) Close()
```

## 基本语义

- 容量必须大于零。
- 队列满时，`Put` 阻塞，直到有空间、上下文取消或队列关闭。
- 队列为空时，`Take` 阻塞，直到有元素、上下文取消，或队列关闭且缓冲区已排空。
- `Close` 幂等，并唤醒所有等待中的生产者和消费者。
- 关闭后不接受新的写入。
- 关闭时已经入队的元素仍可继续被取出。
- 缓冲区排空后，`Take` 返回 `ErrClosed`。
- 等待期间上下文取消时返回 `ctx.Err()`。

## 正确性要求

- 不丢失、不重复消费已经成功写入的元素。
- 不出现数据竞争、死锁或永久无法唤醒的等待者。
- 需要解释所选同步方案的公平性和关闭语义。

## 验证建议

```powershell
go test ./concurrency/producer_consumer
go test -race ./concurrency/producer_consumer
```

