# Coding Interview Practice

这是一个面向后端、基础架构和数据库岗位的手撕代码训练库。训练范围不仅包括算法，还包括数据结构设计、并发编程、小型系统组件、SQL 和语言基础。

## 目录结构

```text
.
├─ AGENTS.md
├─ README.md
├─ algorithms/
├─ data-structures/
├─ concurrency/
├─ system-components/
├─ sql/
├─ language-basics/
└─ review-notes/
```

每道题建议使用独立目录：

```text
topic/problem_name/
├─ problem.md
├─ solution.go
├─ solution_test.go
└─ notes.md              # 完成训练后按需创建
```

`solution` 是作答区，`reference_solution` 只在主动要求参考答案后创建。

## 推荐训练流程

1. 让 Codex 打开某道题并进入模拟面试模式。
2. 先澄清需求、接口和边界，再说明设计。
3. 独立完成 `solution` 文件。
4. 请求代码评审或运行测试。
5. 根据提示逐步修改，不立即查看完整答案。
6. 结束后评分，将关键问题记录到 `review-notes/`。

示例指令：

```text
开始 ttl_lru_cache 模拟面试，使用 Go，先不要给提示。
```

```text
检查我的实现，每次只指出一个最关键的问题。
```

```text
运行并发测试和 race detector，然后帮我复盘。
```

## 测试说明

仓库中的起始解法包含 `TODO`，因此对应测试在完成实现前失败是正常现象。

```powershell
go test ./...
go test -race ./concurrency/...
```

## 分类说明

- `algorithms`：数组、链表、树、图、搜索、动态规划等。
- `data-structures`：LRU、TTL 缓存、堆、阻塞队列等完整数据结构设计。
- `concurrency`：生产者消费者、线程安全队列、工作池和并发控制。
- `system-components`：限流器、连接池、时间轮、一致性哈希等小型组件。
- `sql`：查询、索引、事务、窗口函数和数据库场景题。
- `language-basics`：Go、C++、Python 等语言专项手写练习。
- `review-notes`：跨题目的错误模式和周期复盘。

