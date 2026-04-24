---
on:
  issues:
    types: [labeled]

engine:
  id: codex
  model: gpt-5.4
  env:
    OPENAI_BASE_URL: "https://api.zhizengzeng.com/v1"
    OPENAI_API_KEY: ${{ secrets.OPENAI_API_KEY }}

network:
  allowed:
    - api.zhizengzeng.com

permissions:
  contents: read
  issues: read
  pull-requests: read

safe-outputs:
  create-pull-request:
    base-branch: main
  add-comment:
    max: 1
---

# 自动修复 Bug

当 Issue 被打上 `bug` 标签时触发。

## 任务

1. 阅读 Issue #${{ github.event.issue.number }} 的完整内容
2. 在仓库代码中定位问题所在的文件和函数
3. 分析根本原因
4. 实现修复方案，注意：
   - 保持代码风格与现有代码一致
   - 只修改必要的部分，不做无关改动
   - 如果涉及多个文件，逐一修改
5. 创建一个 Pull Request，标题格式：`fix: <Issue 标题>`
6. 在原 Issue 下回复一条中文评论，说明已提交 PR 及修复思路

如果问题过于复杂或信息不足，在 Issue 下用中文说明原因，不要创建 PR。