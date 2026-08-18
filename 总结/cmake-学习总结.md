# CMake 学习总结

> 学习日期:2026-08-16 ~ 08-17(阶段 0,Day 6~8)
> 仓库:https://github.com/bestjiang725/cpplearn
> 验收项目:cp-test(数论库 gcd/lcm/qpow/is_prime,独立完成一次通过)

---

## 一、核心心智模型(面试必考)

### 1. CMake 不编译代码,它"生成编译指令"

CMake 是构建系统的**生成器**。两阶段:

```bash
cmake -B build          # ①配置:读 CMakeLists.txt,生成 Makefile 等底层构建文件
cmake --build build     # ②构建:调用真正的编译器(g++)干活
```

改 CMakeLists.txt 后要**重新配置**;只改了 .cpp 直接 --build 即可。

### 2. target 是主角,不是文件

现代 CMake 抛弃全局参数(-I、-l 满天飞),一切挂在 **target** 上:

- `add_library` / `add_executable` = 创建 target
- `target_include_directories` / `target_link_libraries` = 给 target **挂属性**
- 使用这个 target 的人,**自动继承**它挂出去的属性

一个规范项目的最小骨架(验收项目 cp-test 就是它):

```cmake
cmake_minimum_required(VERSION 3.16)
project(cp-lib CXX)

add_library(num STATIC src/num.cpp)          # 库 target:num
target_include_directories(num PUBLIC include) # num 的头文件位置,公开给使用者

add_executable(demo src/demo.cpp)            # 可执行 target:demo
target_link_libraries(demo PRIVATE num)      # demo 依赖 num
```

### 3. 依赖传播三关键字(实验验证过)

| 关键字 | 含义 | 谁看得见 |
|---|---|---|
| `PUBLIC` | 自己用,也传给使用者 | 自己 + 依赖我的人 |
| `PRIVATE` | 只自己用,不外传 | 只有自己 |
| `INTERFACE` | 自己不直接编译,只传给使用者 | 只有依赖我的人(header-only 库专用) |

**实验结论**:把 `target_include_directories(num PUBLIC include)` 改成 `PRIVATE` 后,demo 编译立刻报 `fatal error: mylib/math_utils.h: No such file or directory`——因为头文件路径不外传了。一句话:**"谁该看见,就挂给谁"**。

---

## 二、命令速查表

### CMakeLists.txt 里写的(生成指令)

```cmake
cmake_minimum_required(VERSION 3.16)     # 声明最低 CMake 版本(第一行)
project(名字 CXX)                        # 项目名 + 语言(混 C 用 C CXX)

add_library(库名 STATIC 源文件...)       # 静态库(STATIC)/动态库(SHARED)
add_executable(程序名 源文件...)         # 可执行文件
target_include_directories(t PUBLIC 目录) # 头文件搜索路径
target_link_libraries(t PRIVATE 库名)     # 链接依赖

option(开关名 "说明" ON)                 # 构建开关,用户可用 -D 覆盖
add_subdirectory(子目录)                 # 递归组装子模块
find_package(包名 REQUIRED)              # 找已安装的第三方库
if (条件) ... endif()                    # 条件构建
message(STATUS "信息")                  # 构建时打印提示
```

### 终端里敲的

```bash
cmake -B build                  # 配置(默认选项)
cmake -B build -D开关名=OFF     # 配置(关掉某开关)
cmake --build build             # 构建(等价于进 build 目录 make)
cmake --build build -j8         # 并行构建,快很多
```

---

## 三、从手写 g++ 到 CMake 的对应关系

| 手写 g++(Day 6 学的) | CMake(Day 7 学的) | 干的活 |
|---|---|---|
| `g++ -c src/num.cpp` | `add_library(num STATIC ...)` | 编译成 .o 打包成库 |
| `g++ -Iinclude` | `target_include_directories(... include)` | 告诉编译器头文件在哪 |
| `g++ main.o num.o -o app` | `target_link_libraries(demo PRIVATE num)` | 把 .o 拼成可执行文件(链接) |
| 一条条命令手敲 | `cmake --build build` | 自动按依赖顺序全部搞定 |

### 三类错误分类(排查问题的第一反应)

| 阶段 | 典型报错 | 出问题时先想 |
|---|---|---|
| 编译期 | `fatal error: xxx.h: No such file` | 头文件路径(-I/include)不对 |
| 链接期 | `undefined reference to 'main'` / 某函数 | 源文件没编进去 / 库没链接 / 拼写错误 |
| 运行期 | 段错误、输出不对 | 逻辑 bug(→ gdb 的舞台) |

---

## 四、工业项目观察(llama.cpp 阅读课收获)

读 llama.cpp 顶层 CMakeLists(307 行)+ ggml 子项目(505 行)学到的:

1. **`option()` 开关 + `-D` 覆盖**:如 `-DLLAMA_BUILD_TOOLS=OFF` 一条命令切换构建形态
2. **`add_subdirectory()` 总装车间**:顶层只做组装,ggml/src/common/tools 每个目录管自己的 target——你的 cp-test 长大了就该这么拆
3. **`if() 条件构建`**:关掉的模块根本不编译
4. **`CMAKE_EXPORT_COMPILE_COMMANDS ON`**:生成 compile_commands.json,VSCode 补全/跳转靠它
5. **`find_package`**:系统里有现成的就用,没有再编内置的
6. **分层纪律**:顶层找不到 LLAMA_CUDA 开关——因为 CUDA 是 ggml 的事,开关声明在 ggml 自己的 CMakeLists 里。**"谁的东西,谁声明"**
7. **`message(STATUS)` 打印版本**:排查问题必备

---

## 五、踩过的坑(最值钱的部分)

| 坑 | 教训 |
|---|---|
| 创建了 0 字节 CMakeLists.txt | **创建文件 ≠ 写完内容**;CMake 警告 "No project() command"——警告要认真读 |
| 在仓库根目录运行 `cmake -B build` | 先 `cd` 到 CMakeLists.txt 所在目录再配置 |
| 目录写成 `scr/` | 一个字母的笔误 = 源文件没编进去 = 链接期 `undefined reference to 'main'` |
| `#pragma Once` 大小写错误 | GCC 对未知 pragma **静默忽略**,include guard 静默失效,不报错——手写头文件模板要背熟 |
| g++ 报"文件不存在"但文件明明在 | shell 停在已删除目录(inode 悬空),`cd` 到绝对路径重新解析 |
| 头文件里 `using ll = long long` | 库头文件污染使用者的命名空间,写库不要写 using |
| 两条相同提交 | 命令块执行了两遍——git 忠实记录每一次 |

---

## 六、遗留(以后补)

- `install` 命令、`ctest` 测试集成、动态库 `SHARED`
- compile_commands.json 实测:装 clangd 验证跳转
- `CMakePresets.json`(多配置场景管理)
- 写正式项目时把 cp-test 升级成 add_subdirectory 模块化结构

---

## 七、阶段 0 进度对照

| 天 | 内容 | 状态 |
|---|---|---|
| Day 1~2 | WSL2 + Linux 基础 | ✅ |
| Day 3~5 | git(总结文档:git-学习总结.md) | ✅ |
| Day 6~8 | CMake(本文档) | ✅ |
| Day 9~11 | gdb 调试 | 进行中 |
| Day 12~14 | clone+编译 llama.cpp,跑通 llama-cli | 待开始 |
