---
on:
  issues:
    types: [labeled]

engine:
  id: copilot
  env:
    COPILOT_PROVIDER_BASE_URL: "https://ark.cn-beijing.volces.com/api/v3"
    COPILOT_PROVIDER_BEARER_TOKEN: ${{ secrets.OPENAI_API_KEY }}
    COPILOT_MODEL: doubao-seed-2-0-code-preview-260215
    COPILOT_PROVIDER_TYPE: openai
    COPILOT_PROVIDER_WIRE_API: responses

features:
  dangerously-disable-sandbox-agent: "controlled environment for issue triage automation"
sandbox:
  agent: false
strict: false

network:
  allowed:
    - defaults
    - ark.cn-beijing.volces.com
    - github.com        
    - api.github.com 

tools:
  github:
    min-integrity: none

permissions:
  contents: read
  issues: read
  pull-requests: read

safe-outputs:
  threat-detection: false
  create-pull-request:
    base-branch: develop
  add-comment:
    max: 1
---

# 自动处理 Issue

当 Issue 被打上 `bug` 或 `enhancement` 标签时触发。其他标签直接退出，不做任何操作。

## 判断任务类型

读取 Issue #${{ github.event.issue.number }} 当前的标签：
- 如果包含 `bug` 标签 → 执行【Bug 修复流程】
- 如果包含 `enhancement` 标签 → 执行【新功能开发流程】
- 其他情况 → 直接退出

---

## Bug 修复流程

1. 阅读 Issue 的完整标题和正文，理解问题现象
2. 浏览仓库代码和文档，定位问题所在的文件和函数
3. 分析根本原因
4. 实现修复方案，注意：
   - 保持与现有代码风格一致
   - 只修改必要的部分，不做无关改动
5. 创建 Pull Request，标题格式：`fix: <Issue 标题>`，描述中说明：
   - 问题根因
   - 修复方式
   - 如何验证
6. 在原 Issue 下用中文回复，说明已提交 PR 及修复思路

如果问题过于复杂或信息不足，在 Issue 下用中文说明原因，不创建 PR。

---

## 新功能开发流程

1. 阅读 Issue 的完整标题和正文，理解需求目标
2. 浏览仓库现有代码结构和文档，找到最相关的模块和文件
3. 制定实现方案：
   - 需要新增哪些文件或函数
   - 需要修改哪些现有文件
4. 按方案实现代码，注意：
   - 保持与现有代码风格一致
   - 新增函数/类需要添加注释
   - 如果涉及接口变更，同步更新相关调用方
5. 创建 Pull Request，标题格式：`feat: <Issue 标题>`，描述中说明：
   - 实现了哪些功能
   - 涉及哪些文件改动
   - 如何验证/测试
6. 在原 Issue 下用中文回复，说明已提交 PR、实现思路和测试建议

如果需求描述不清晰或实现风险过大，在 Issue 下用中文说明原因，不创建 PR。