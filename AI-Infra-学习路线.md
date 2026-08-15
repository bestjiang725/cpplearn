# AI Infra 学习路线:推理引擎 / 算子开发 / 大模型部署优化

> 适用:28 届、211 本科、算法竞赛背景、C++ 方向
> 时间窗口:2026.8 → 2027.8(秋招);第一个硬 deadline:**2027 年 2~5 月暑期实习申请**
> 原则:每阶段必须有产出物(项目/博客/PR),没有产出物的学习不进简历

---

## 三条并行线

| 线 | 内容 | 精力占比 |
|---|---|---|
| **项目线**(主线) | GGUF 解析器 → Mini 推理引擎 → CUDA 算子 → Flash Attention → 开源贡献 | 60% |
| **理论线** | OS/体系结构 → Transformer → 量化与推理优化 → CUDA 原理 | 30% |
| **求职线**(按节点启动) | 简历 → 暑期实习 → 秋招 | 10% |

---

## 阶段 0:工程基础速成(2026.8 中下旬,~2 周)

**目标**:补齐竞赛选手的工程短板,能独立管理多文件项目。
**节奏**:每天 2~3 小时,每个工具当天配一个练习,不刷纯教程。
**心态**:llama.cpp 看不懂很正常——本阶段只要求"能构建、能运行、能观察",读懂是阶段 1~3 的事。

### 第 1~2 天:WSL2 + Linux 基础
- [ ] 装 WSL2:管理员 PowerShell 运行 `wsl --install -d Ubuntu-24.04`,重启
- [ ] shell 基本功:cd/ls/mkdir/mv/cp/rm、重定向与管道、grep/find、权限(chmod)、环境变量(PATH/export)、.bashrc
- [ ] apt 包管理;vscode 装 Remote-WSL 插件,代码全在 Linux 里写
- [ ] 练习:在 WSL 里建目录结构,用一条管道命令统计某目录下所有 .cpp 的总行数
- [ ] 验收:文件操作不再依赖 Windows 资源管理器

### 第 3~5 天:git
- [ ] 先玩交互式 [Learn Git Branching](https://learngitbranching.js.org/)(有中文),2 小时通关主线
- [ ] 概念:工作区/暂存区/HEAD 三者关系;commit 是快照不是差异
- [ ] 命令主线:init/add/commit/status/log/diff → branch/checkout/merge → remote/push/pull/fetch → rebase 与冲突解决
- [ ] 练习:把算法模板库推到 GitHub;开分支练 merge 和 rebase;故意制造一次冲突并解决
- [ ] 验收:能答出"merge 和 rebase 的区别"、"HEAD/index/工作区的关系"

### 第 6~8 天:CMake
- [ ] 核心概念:target 是主角;add_library/add_executable;PUBLIC/PRIVATE/INTERFACE 三种依赖传播;find_package
- [ ] 读 [Modern CMake 中文版](https://modern-cmake-cn.github.io/Modern-CMake-zh_CN/)(很短,一天能翻完)
- [ ] 练习:从零搭三目录项目:include/ + src/ 静态库 + main.cpp 可执行 + 测试目录;会 `cmake -B build` 构建、写 install
- [ ] 验收:不看文档从零写一个规范 CMakeLists.txt

### 第 9~11 天:gdb
- [ ] 命令主线:break/run/next/step/print/backtrace/watch/info locals
- [ ] 必练场景:段错误(空指针/越界)、逻辑错误(观察变量变化)、core dump 分析
- [ ] 练习:写一个递归栈溢出程序用 bt 看调用栈;写一个数组越界程序用 watch 定位
- [ ] 验收:不用 printf 定位一个 bug

### 第 12~14 天:综合实战(衔接下一阶段)
- [ ] `git clone --depth 1` llama.cpp,cmake 编译(先默认 CPU,有 N 卡再开 CUDA)
- [ ] `git log --oneline` 看最近 100 个提交,感受工业项目迭代方式
- [ ] 下载 Qwen2.5-0.5B-Instruct GGUF(~500MB,HuggingFace 上找),跑通 llama-cli 对话
- [ ] gdb 在 llama_decode 入口打断点,跑一步观察
- [ ] 整个学习仓库 push 到 GitHub,写好 README

**阶段 0 总验收**:Linux 下独立完成"clone → 配置 → 构建 → 运行 → 调试 → 提交"完整闭环。

---

## 阶段 1:GGUF 解析器 + Transformer 理论(2026.9~10 中,~6 周)

**理论**:
- Transformer:3Blue1Brown 神经网络系列(有中文版)+ Karpathy Zero to Hero 讲 GPT 的几集 → 读 nanoGPT 代码
- 对照 llama.cpp 源码:llm_build_llama 如何把 Transformer 变成计算图

**项目 A:GGUF 解析器**(C++17,单文件 ~500 行)
- 读 GGUF spec(ggml/docs/gguf.md)+ examples/gguf/gguf.cpp
- 实现:解析 header / KV 元数据 / tensor 信息表;校验 magic 和版本
- 加分:mmap 加载、写单元测试

**验收**:自己的解析器正确读出 Qwen2.5-0.5B 全部元信息,与官方工具输出一致。
**产出**:博客 1《GGUF 文件格式拆解》

---

## 阶段 2:Mini 推理引擎(2026.10~12,~8 周)★ 核心项目

**目标**:简历主项目。纯手写(不依赖 ggml)实现能跑的 Transformer 推理。

- **v1(~3 周)**:手写 naive matmul(float)→ embedding → 朴素 attention(softmax)→ FFN → lm_head;复用项目 A 加载 GGUF;跑出第一个 token(贪心采样)
- **v2(~2 周)**:加 KV cache;实现 prefill/decode 两阶段;能生成连贯文本
- **v3(~3 周)**:支持量化权重(Q4_0/Q4_K 反量化);batch 推理;与 llama.cpp 对比性能,写 benchmark

**面试必问点(边写边想清楚)**:
- KV cache 为什么把每 token 计算从 O(n²) 降到 O(n)?
- prefill 是计算密集(compute-bound),decode 是访存密集(memory-bound),为什么?怎么优化?
- 量化为什么省内存且精度损失小?块量化 vs 逐层量化的区别?

**验收**:Qwen2.5-0.5B 用自己的引擎生成流畅文本;能讲清每一层的数据流和内存占用。
**产出**:博客 2~3《手写 Transformer 推理》系列

---

## 阶段 3:CUDA 主线 + 开源贡献(2026.12~2027.2 寒假,~8 周)

**目标**:简历溢价标签 + 第一个 merged PR。**必须用真 GPU**(无卡用 Colab 免费额度或便宜云 GPU)。

**学习顺序**:
1. [CUDA C++ Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/) 前 5 章
2. 教材:PMPP《并行处理器编程实战》(有中译本),重点:线程组织、内存模型、共享内存
3. 动手项目(递进,每步测速 + 写博客):
   - **CUDA matmul**:naive → 共享内存分块 → 2D block tiling →(可选)tensor core
   - **简化版 Flash Attention**:tiling + online softmax,理解为什么省显存
4. llama.cpp 源码深读:llama_decode 完整调用链、ggml 后端分派机制
5. **开源 PR**:llama.cpp low complexity 标签的 issue/PR(改注释、修 warning、小 bugfix),目标 1 个 merged

**验收**:matmul 跑到可用性能(如 4096³ 达 TFLOP 级);博客 4~6 篇;1 个 merged PR。

---

## 阶段 4:暑期实习申请 + 面试准备(2027.2~5)★ 第一战役

**目标**:拿到暑期实习 offer(主投芯片公司算子岗、云厂商推理团队)。

- [ ] 2~3 月:简历定稿(项目 B + CUDA 项目 + 博客 + merged PR + 竞赛奖项放显眼位置);海投提前批
- [ ] 3~4 月:面试冲刺——C++/OS 八股查漏、把项目 B 每个设计决策写成 Q&A、Transformer/量化/推理八股
- [ ] 每场面试后复盘,不会的题回填知识库
- [ ] 并行:继续维护项目(README、开源)
- [ ] **保底策略**:若 AI Infra 实习不中,立刻转投基础软件/系统研发实习(技能 70% 重叠,转正路径同样有效)

---

## 阶段 5:实习 + 秋招并行(2027.6~8)

- 实习期间表现优先,争取转正(实习转正不看学历,这是 211 本科最大的破局点)
- 7~8 月秋招提前批同步投递:冲刺(DeepSeek/MiniMax 类头部 AI Infra)+ 主攻(芯片公司/云厂商/二线 AI 公司)全投

---

## 每日节奏(大三上学期)

- **课程内**:OS、计组、编译原理认真学——它们就是理论线本身,别划水
- **课外**:每周 12~15 小时,项目线优先
- **习惯**:每完成一个小模块就写博客,输出倒逼理解

---

## 面试考点速查(三类岗)

| 岗位 | 核心考点 |
|---|---|
| 推理引擎 | C++ 八股、KV cache、prefill/decode 特征、量化原理(GGUF/GPTQ/AWQ)、batch 调度、vLLM PagedAttention 思想 |
| 算子开发 | CUDA 内存层级、bank conflict、occupancy、warp 原理、手写 kernel 思路、roofline 性能分析 |
| 部署优化 | 延迟 vs 吞吐、模型压缩手段、推理服务架构(Triton/vLLM server)、端侧 vs 云端差异 |

---

## 风险与调整

- **没 GPU**:阶段 3 用 Colab/云 GPU 解决,不要跳过(算子岗没 GPU 经验等于没入门)
- **课程压力大**:阶段 2 可延至寒假,但阶段 4 截止时间不动
- **竞赛思维惯性**:工程不追求"一次写对最优解",先能跑、再优化、最后重构

---

## 简历最终形态(2027.3 目标)

- 项目:Mini 推理引擎(GitHub 公开,star/README/benchmark 数据)+ CUDA matmul & Flash Attention + GGUF 解析器
- 开源:llama.cpp 至少 1 个 merged PR
- 输出:技术博客 8 篇以上
- 竞赛:ICPC/CCPC 奖项
- 实习:2027 暑期实习(尽量 AI Infra 方向)
