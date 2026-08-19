# cpplearn

算法竞赛选手转 AI Infra 的学习仓库(2026.8 起)。学习主线见 `AI-Infra-学习路线.md`。

## 阶段 0:工程基础速成 ✅(2026.8 完成)

- [x] Linux / WSL2:文件操作、管道、包管理
- [x] git:工作区/暂存区/HEAD、merge vs rebase、冲突解决
- [x] CMake:target 思想、PUBLIC/PRIVATE 依赖传播、条件构建
- [x] gdb:断点/观察点/bt/帧切换/core dump/卡死定位
- [x] llama.cpp:CPU 构建(-march=native + OpenMP)、Qwen2.5-0.5B Q4_K_M 推理 60~70 t/s
- [x] gdb 调试 llama.cpp:llama_decode 调用链、预热 vs 真推理判别、后端分派、线程模型

## 目录结构

- `总结/` — git / CMake / gdb 学习总结与笔记
- `day01/` — Linux 练习
- `cmake-day01/` — 手写 g++ → CMake 演进练习
- `cp-test/` — CMake 验收项目:数论库(gcd/lcm/qpow/is_prime)
- `gdb-day01~03/` — gdb 练习:递归栈、越界 watch、段错误/core dump/卡死三案
- `conflict.txt` — 冲突解决练习遗迹

## 下一步

阶段 1:GGUF 解析器 + Transformer 理论 → Mini 推理引擎(2026.9~12)
