# 带过期时间的 LRU Cache

实现一个容量固定、支持独立 TTL 的 LRU 缓存。

## 接口

```go
func NewTTLLRU(capacity int, now func() time.Time) (*TTLLRU, error)
func (c *TTLLRU) Put(key string, value int, ttl time.Duration) error
func (c *TTLLRU) Get(key string) (int, bool)
func (c *TTLLRU) Len() int
```

## 基本语义

- `capacity` 必须大于零。
- TTL 从 `Put` 成功时开始计算，`Get` 不续期。
- `ttl <= 0` 时返回错误，不写入缓存。
- `Get` 命中未过期元素时，将其提升为最近使用。
- `Get` 访问过期元素时，应删除它并返回未命中。
- 更新已有 key 时覆盖值和过期时间，并提升为最近使用。
- 写入新 key 且容量已满时，先清理能发现的过期元素；仍满则淘汰最久未使用元素。
- `Len` 只统计调用时尚未过期的元素。
- 当前题目默认不要求线程安全，进阶要求可在澄清后增加。

## 性能目标

- `Get`：平均 `O(1)`。
- `Put`：平均 `O(1)`；允许为清理过期元素付出额外开销，但需要解释策略。

## 可测试性

构造函数接收 `now`，测试可以注入假时钟，不应通过真实休眠等待过期。

