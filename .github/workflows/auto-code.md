---
on:
  issues:
    types: [labeled]

engine:
  id: copilot
  env:
    COPILOT_PROVIDER_BASE_URL: "https://ark.cn-beijing.volces.com/api/v3"
    COPILOT_PROVIDER_BEARER_TOKEN: ${{ secrets.OPENAI_API_KEY }}
    COPILOT_MODEL: doubao-seed-evolving
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

tools:
  github:
    min-integrity: none

permissions:
  contents: read
  issues: read
  pull-requests: read
  copilot-requests: write

safe-outputs:
  threat-detection: false
  create-pull-request:
    base-branch: develop
    protected-files: allowed
  add-comment:
    max: 1
---

# 自动处理 Issue

当 Issue 被打上 `bug` 或 `enhancement` 或 `feature` 标签时触发。其他标签直接退出，不做任何操作。

## 判断任务类型

读取 Issue #${{ github.event.issue.number }} 当前的标签：
- 如果包含 `bug` 标签 → 执行【Bug 修复流程】
- 如果包含 `feature` 标签 → 执行【新功能开发流程】
- 如果包含 `enhancement` 标签 → 执行【功能改进开发流程】
- 其他情况 → 直接退出

## 任务执行限制
- 尽量只读取相关代码文件和文档,不去操作无关代码和文件
- 尽量减少操作时间和步骤,减少TOKEN和时间消耗

---


## Bug 修复流程

1. 阅读 Issue 的完整标题和正文，理解问题现象
2. 浏览仓库相关代码和文档，定位问题所在的文件和函数
3. 分析根本原因
4. 实现修复方案，注意：
   - 保持与现有代码风格一致
   - 只修改必要的部分，不做无关改动
5. 创建 Pull Request，标题格式：`fix: <Issue 标题>`，描述中说明：
   - 问题根因
   - 修复方式
   - 如何验证
6. 在原 Issue 下使用用户提问的语言进行回复，说明已提交 PR 及修复思路

如果问题过于复杂或信息不足，在 Issue 下用中文说明原因，不创建 PR。

---

## 新功能开发流程

1. 阅读 Issue 的完整标题和正文，理解需求目标
2. 浏览仓库相关代码结构和文档，找到最相关的模块和文件
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
6. 在原 Issue 下使用用户提问的语言进行回复，说明已提交 PR、实现思路和测试建议

如果需求描述不清晰或实现风险过大，在 Issue 下用中文说明原因，不创建 PR。

---

## 功能改进开发流程

1. 阅读 Issue 的完整标题和正文，明确功能改进与优化的具体目标（如性能提升、代码结构重构、用户体验优化等）。
2. 浏览仓库相关代码，评估受影响的范围，定位需要进行优化或重构的核心文件及函数。
3. 制定改进方案，需特别注意：
   - **向下兼容性**：确保本次改进不会破坏现有的公开接口（API）和已有功能。
   - **防御性编程**：优化逻辑的同时，不能降低代码的健壮性。
4. 按照方案实施代码改动，注意：
   - 保持与现有代码风格高度一致。
   - 仅针对性能、可读性或结构进行局部优化，严禁引入未经需求的无关大范围改动。
5. 创建 Pull Request，根据改进的核心侧重点选择标题格式：
   - 侧重于代码重构/可读性优化：`refactor: <Issue 标题>`
   - 侧重于运行运行效率/性能优化：`perf: <Issue 标题>`
   在 PR 描述中说明：
   - 优化的动机与改进点
   - 改进前后的对比或预期收益
   - 验证优化是否生效的测试方法
6. 在原 Issue 下使用用户提问的语言进行回复，说明已提交 PR、优化思路及预期的提升效果。

如果功能改进可能带来重大的破坏性变更（Breaking Changes）或者现有代码结构不支持盲目优化，在 Issue 下用中文说明原因，保持维持原状，不创建 PR。