# 智能旅游推荐 Agent

一个基于 **ReAct（Reasoning + Acting）** 模式的智能旅游助手，能够根据用户需求自动查询天气信息并推荐合适的旅游景点。

## ✨ 功能特点

- 🌤️ **实时天气查询**：通过 wttr.in API 获取全球城市的实时天气信息
- 🏖️ **智能景点推荐**：基于天气和城市，使用 Tavily Search API 智能推荐旅游景点
- 🤖 **ReAct 推理模式**：采用 Thought（思考）→ Action（行动）→ Observation（观察）循环，让 AI 自主推理和决策
- 🔌 **兼容多种 LLM**：支持任何兼容 OpenAI API 格式的大语言模型服务（OpenAI、DeepSeek、阿里云等）
- 🛡️ **健壮的错误处理**：完善的异常捕获和错误提示机制

## 📁 项目结构

```
mytravlagent/
├── FirstAgentTest.py      # 主程序（包含所有代码）
├── myvenv/                # Python 虚拟环境
├── requirements.txt       # 项目依赖
├── README.md              # 项目说明文档
├── config.example.py      # 配置文件示例
└── .gitignore             # Git 忽略规则
```

## 🚀 快速开始

### 1. 克隆项目

```bash
cd mytravlagent
```

### 2. 创建虚拟环境（如果还没有）

```bash
python -m venv myvenv
```

### 3. 激活虚拟环境

```bash
# Linux/Mac
source myvenv/bin/activate

# Windows
myvenv\Scripts\activate
```

### 4. 安装依赖

```bash
pip install -r requirements.txt
```

### 5. 配置 API 密钥

编辑 `FirstAgentTest.py` 文件的第 139-142 行，填入您的真实 API 凭证：

```python
API_KEY = "your_actual_api_key"           # 替换为您的 LLM API 密钥
BASE_URL = "https://api.openai.com/v1"    # 替换为 LLM API 基础 URL
MODEL_ID = "gpt-4"                        # 替换为使用的模型 ID
os.environ['TAVILY_API_KEY'] = "your_tavily_key"  # 替换为 Tavily API 密钥
```

**获取 API 密钥：**
- **OpenAI API**：访问 [platform.openai.com](https://platform.openai.com)
- **DeepSeek API**：访问 [platform.deepseek.com](https://platform.deepseek.com)
- **阿里云百炼**：访问 [bailian.aliyun.com](https://bailian.aliyun.com)
- **Tavily API**：访问 [tavily.com](https://tavily.com)（提供免费套餐）

### 6. 运行程序

```bash
python FirstAgentTest.py
```

## 🔧 工作原理

本项目采用 **ReAct（Reasoning + Acting）** 模式，让 AI Agent 能够：

### ReAct 循环流程

```
用户请求
   ↓
[思考 Thought] → 分析当前情况，规划下一步行动
   ↓
[行动 Action] → 调用工具获取信息（如查天气、搜景点）
   ↓
[观察 Observation] → 接收工具返回的结果
   ↓
[继续循环] → 根据观察结果继续思考和行动
   ↓
[完成 Finish] → 给出最终答案
```

### 示例运行流程

```
用户：请帮我查询北京的天气，然后推荐一个合适的旅游景点

━━━ 循环 1 ━━━
Thought: 首先需要查询北京的天气情况
Action: get_weather(city="北京")
Observation: 北京当前天气：Sunny，气温 25 摄氏度

━━━ 循环 2 ━━━
Thought: 天气晴朗且温度适宜，现在可以推荐景点
Action: get_attraction(city="北京", weather="Sunny")
Observation: 推荐游览颐和园、故宫、天坛等景点...

━━━ 循环 3 ━━━
Thought: 已获得足够信息，可以给出最终答案
Action: finish(answer="根据查询，北京今天天气晴朗，气温25度...")
✓ 任务完成
```

## 📚 可用工具

### `get_weather(city: str) -> str`
查询指定城市的实时天气信息。

**参数：**
- `city` (str): 城市名称（支持中英文）

**返回：**
- 天气描述字符串，包含天气状况和温度

**示例：**
```python
get_weather(city="北京")
# 返回: "北京当前天气：Sunny，气温25摄氏度"
```

### `get_attraction(city: str, weather: str) -> str`
根据城市和天气推荐旅游景点。

**参数：**
- `city` (str): 城市名称
- `weather` (str): 天气状况

**返回：**
- 景点推荐信息字符串

**示例：**
```python
get_attraction(city="北京", weather="Sunny")
# 返回景点推荐信息
```

## ⚙️ 自定义配置

### 修改用户请求

在 `FirstAgentTest.py` 第 151 行修改：

```python
user_prompt = "你的自定义请求"
```

### 调整最大循环次数

在 `FirstAgentTest.py` 第 157 行修改：

```python
for i in range(5):  # 改为你想要的最大循环次数
```

### 添加新工具

1. 定义新的工具函数：

```python
def get_hotel(city: str, price_range: str) -> str:
    """查询酒店信息"""
    # 实现代码
    pass
```

2. 在 `available_tools` 字典中注册：

```python
available_tools = {
    "get_weather": get_weather,
    "get_attraction": get_attraction,
    "get_hotel": get_hotel,  # 添加新工具
}
```

3. 在 `AGENT_SYSTEM_PROMPT` 中添加工具说明：

```python
AGENT_SYSTEM_PROMPT = """
...
# 可用工具:
- `get_weather(city: str)`: 查询指定城市的实时天气。
- `get_attraction(city: str, weather: str)`: 根据城市和天气搜索推荐的旅游景点。
- `get_hotel(city: str, price_range: str)`: 查询指定价格范围的酒店。
...
"""
```

## 🛠️ 技术栈

- **Python 3.7+**
- **OpenAI SDK**：用于调用 LLM API
- **Requests**：用于 HTTP 请求
- **Tavily Python SDK**：用于智能搜索
- **正则表达式**：用于解析 LLM 输出

## 📝 注意事项

1. **API 密钥安全**：
   - 不要将包含真实 API 密钥的代码提交到公共代码仓库
   - 建议使用环境变量或配置文件管理密钥
   - 已在 `.gitignore` 中排除 `config.py`

2. **网络连接**：
   - 程序需要稳定的网络连接以访问外部 API
   - 确保可以访问 OpenAI API、wttr.in 和 Tavily API

3. **API 配额**：
   - 注意各服务的 API 调用限制和配额
   - Tavily 提供免费套餐，但有调用次数限制
   - LLM API 通常按 token 计费

4. **城市名称**：
   - 天气查询支持中英文城市名
   - 建议使用标准城市名称以提高准确率

5. **错误处理**：
   - 程序已添加完善的错误处理机制
   - 如果工具调用失败，Agent 会收到错误信息并尝试继续执行

## 🐛 常见问题

### 1. ModuleNotFoundError: No module named 'openai'

**解决方案：** 确保已激活虚拟环境并安装了依赖
```bash
source myvenv/bin/activate
pip install -r requirements.txt
```

### 2. API 调用失败

**解决方案：**
- 检查 API 密钥是否正确
- 检查网络连接
- 查看 API 服务是否正常运行

### 3. 天气查询返回错误

**解决方案：**
- 确认城市名称拼写正确
- 尝试使用英文城市名
- 检查 wttr.in 服务是否可访问

### 4. Agent 循环次数用尽但任务未完成

**解决方案：**
- 增加最大循环次数（修改第 157 行）
- 简化用户请求
- 检查 LLM 输出格式是否符合要求

## 🔄 项目改进方向

- [ ] 支持从配置文件读取 API 密钥
- [ ] 添加日志记录功能
- [ ] 支持流式输出显示
- [ ] 添加更多旅游相关工具（酒店、餐厅、交通等）
- [ ] 支持多轮对话和上下文记忆
- [ ] 添加单元测试
- [ ] 创建 Web 界面

## 📄 许可证

MIT License

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 📧 联系方式

如有问题或建议，欢迎联系。

---

**祝您使用愉快！🎉**

