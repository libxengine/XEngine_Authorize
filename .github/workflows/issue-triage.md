---
on:
  issues:
    types: [opened, reopened]

engine:
  id: copilot
  env:
    COPILOT_PROVIDER_BASE_URL: "https://ark.cn-beijing.volces.com/api/v3"
    COPILOT_MODEL: doubao-seed-2-0-code-preview-260215
    COPILOT_PROVIDER_API_KEY: ${{ secrets.OPENAI_API_KEY }}
    COPILOT_PROVIDER_TYPE: openai

network:
  allowed:
    - defaults
    - ark.cn-beijing.volces.com

sandbox:
  agent: false
strict: false

tools:
  github:
    min-integrity: none

permissions:
  issues: read
  contents: read
  pull-requests: read

safe-outputs:
  threat-detection: false
  add-comment:
    max: 1
  add-labels:
    allowed:
      - bug
      - feature
      - enhancement
      - documentation
      - question
      - duplicate
      - invalid
---

# Issue 自动分类与回复

分析仓库 ${{ github.repository }} 中新开的 Issue。

## 任务

1. 阅读 Issue 的标题和正文
2. 判断类型，添加合适的标签：
   - `bug`：程序出错、功能异常
   - `feature`：新功能请求
   - `enhancement`：改进现有功能
   - `documentation`：文档问题
   - `question`：使用疑问
   - `duplicate`：与已有 Issue 重复
   - `invalid`：信息不完整或无效

3. 用中文回复一条友好的评论，包含：
   - 感谢用户提交 Issue
   - 说明你打的标签和原因
   - 如果是 bug，请用户补充：复现步骤、环境信息、错误日志
   - 如果是 question，尝试根据仓库代码和文档给出初步解答
   - 预计处理时间（普通问题 3 个工作日内）