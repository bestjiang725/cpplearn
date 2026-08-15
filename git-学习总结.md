# Git 学习总结

> 学习日期:2026-08-14 ~ 08-15(阶段 0,Day 3~5)
> 仓库:https://github.com/bestjiang725/cpplearn

---

## 一、核心心智模型(面试必考)

### 1. commit 是快照,不是差异
每个提交保存的是**那一刻整个项目的完整状态**。git 展示给你看的是"差异",内部存的是快照。

### 2. 三分区

| 区域 | 位置 | 作用 |
|---|---|---|
| 工作区 | 磁盘上的文件 | 你正在改的地方 |
| 暂存区(index) | `.git/index` | "购物车",决定下次提交装什么 |
| 版本库 | `.git/objects` | 提交历史(快照堆) |

流程:`git add`(工作区→暂存区)→ `git commit`(暂存区→版本库)

### 3. branch 是指针,HEAD 是当前位置
- 分支只是**指向某个 commit 的指针**,切分支 = 移动 HEAD
- 提交历史是一张 DAG(有向无环图),每个 commit 记录父指针
- `git log --oneline --graph --all` 是看懂这张图的眼睛

### 4. 快进 vs 三方合并
- **fast-forward**:一边没提交,指针直接前移,树是直线
- **三方合并**:两边都动了,git 创建一个有两个父提交的"合并提交"

---

## 二、命令速查表

### 配置(一次性)
```bash
git config --global user.name "你的名字"
git config --global user.email "邮箱"
git config --global init.defaultBranch main    # 新仓库默认分支名
git config --global credential.helper store    # 记住 GitHub 凭证
```

### 本地日常
```bash
git init                          # 建仓库
git status                        # 看状态(最常用的命令,没有之一)
git add 文件 / git add .          # 加入暂存区
git commit -m "说明"              # 提交
git commit -am "说明"             # 已跟踪文件的修改,免 add
git log --oneline --graph --all   # 看提交树
git diff                          # 看未暂存的改动
```

### 分支
```bash
git switch -c 分支名              # 建分支并切过去
git switch main                   # 切回
git branch -v                     # 看所有分支
git branch -d 分支名              # 安全删除(未合并会拒绝)
git branch -D 分支名              # 强制删除
```

### 合并
```bash
git merge 分支名                  # 保留真实历史,产生合并提交
git rebase main                   # 把当前分支的提交"搬到"main 顶点重演
git merge --abort                 # 放弃合并/解决冲突,反悔用
```

### 远程
```bash
git remote add origin <仓库URL>   # 关联远程
git remote -v                     # 查看远程
git push -u origin main           # 首次推送并建立跟踪
git push                          # 之后直接推
```

---

## 三、merge vs rebase(面试高频)

| | merge | rebase |
|---|---|---|
| 历史形状 | 保留分叉,有合并提交 | 线性,无合并提交 |
| 历史真实性 | 真实 | **重写**(哈希全变) |
| 适用场景 | 公共分支 | 个人分支整理 |
| 坑 | 树形复杂 | 公共分支 rebase 会坑队友 |

一句话:merge 是"如实记录",rebase 是"把故事讲成直线"。

---

## 四、冲突处理五步流程

1. `git status` → 看到 `both modified`,进入未合并状态
2. `cat 文件` → 看到标记:
   ```
   <<<<<<< HEAD      ← 你这边的内容
   你的版本
   =======           ← 分隔线
   对方的版本
   >>>>>>> 分支名     ← 对方的内容
   ```
3. 用编辑器**人工裁决**(保留谁、怎么保留),删掉三个标记行
4. `git add 冲突文件`(add = 告诉 git "我已裁决")
5. `git commit`(完成合并提交)

关键理解:git 只圈出分歧,**内容裁决永远是人来做**。冲突的条件:分叉后两边改了同一文件的同一行。

---

## 五、今天踩过的坑(最值钱的部分)

| 坑 | 教训 |
|---|---|
| `cd~` 报错 | 命令和参数之间**必须有空格** |
| `> main` 手滑 | 重定向符指向哪输出就去哪,目标写错会创建奇怪的文件 |
| 同名提交出现两次 | 命令块执行了两遍,git 忠实记录每一次 |
| nano 弹出来 | `Ctrl+X` → `Y` → `Enter`;merge/commit 没带 -m 都会进编辑器 |
| 默认分支叫 master | 行业标准已改为 main,记得配 `init.defaultBranch` |
| `-d` 拒绝删分支 | 未合并的分支受保护,强删用 `-D` |
| 以为会冲突结果快进 | 冲突需要"分叉后两边都改同一行",一边没动就是快进 |
| push 时密码不显示 | 输 token 时屏幕无回显是正常的,不是卡了 |

---

## 六、GitHub 工作流备忘

1. 网页建仓库(New repository → Public)
2. 生成 PAT:Settings → Developer settings → Fine-grained tokens → 只勾该仓库 → Contents 读写 → 复制 `github_pat_` 开头的串(只显示一次)
3. `git remote add origin <URL>` → `git push -u origin main`
4. 提示 Username 输用户名,Password 粘贴 PAT
5. 以后 `add` → `commit` → `push` 三步走,小步提交,message 写清楚

---

## 遗留(以后补)

- `git diff` 各参数、`git stash`(暂存现场)
- `fetch` vs `pull`、多人协作流(PR、review)
- `git reset` / `git reflog`(后悔药)
