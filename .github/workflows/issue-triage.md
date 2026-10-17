---
on:
  issues:
    types: [opened, reopened]

engine:
  id: copilot
  max-turns: 5
  env:
    COPILOT_PROVIDER_BASE_URL: ${{ vars.MODEL_AI_AGENT_ADDR }}
    COPILOT_PROVIDER_BEARER_TOKEN: ${{ secrets.OPENAI_API_KEY }}
    COPILOT_MODEL: ${{ vars.MODEL_AI_AGENT_NAME }}
    COPILOT_PROVIDER_TYPE: ${{ vars.MODEL_AI_AGENT_TYPE }}
    COPILOT_PROVIDER_WIRE_API: ${{ vars.MODEL_AI_AGENT_API }}

features:
  dangerously-disable-sandbox-agent: true
sandbox:
  agent: false

strict: false

network:
  allowed:
    - defaults
    - ark.cn-beijing.volces.com

tools:
  github:
    min-integrity: none
  bash: ["grep", "cat", "head"] 

permissions:
  issues: read
  contents: read
  pull-requests: read
  copilot-requests: write

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

## 任务执行限制
- 根据需求查找相对应的可能关联的代码文件
- 尽量只读取相关代码文件和文档,不去操作无关代码和文件
- 尽量减少操作时间和步骤,减少TOKEN和时间消耗

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

3. 使用用户提问的语言回复一条友好的评论，包含：
   - 感谢用户提交 Issue
   - 说明你打的标签和原因
   - 如果是 bug，请用户补充：复现步骤、环境信息、错误日志
   - 如果是 question，尝试根据仓库代码和文档给出初步解答
   - 预计处理时间（普通问题 3 个工作日内）