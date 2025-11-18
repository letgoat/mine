"""
配置文件示例
复制此文件为 config.py 并填入您的真实 API 密钥
"""

# LLM API 配置
# 支持 OpenAI、DeepSeek、阿里云百炼等兼容 OpenAI API 格式的服务
API_KEY = "YOUR_API_KEY"                    # 您的 LLM API 密钥
BASE_URL = "https://api.openai.com/v1"      # API 基础 URL
MODEL_ID = "gpt-4"                          # 使用的模型 ID

# Tavily 搜索 API 配置
# 访问 https://tavily.com 注册获取免费 API 密钥
TAVILY_API_KEY = "YOUR_TAVILY_API_KEY"      # 您的 Tavily API 密钥

# 示例配置 - OpenAI
"""
API_KEY = "sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
BASE_URL = "https://api.openai.com/v1"
MODEL_ID = "gpt-4"
"""

# 示例配置 - DeepSeek
"""
API_KEY = "sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
BASE_URL = "https://api.deepseek.com/v1"
MODEL_ID = "deepseek-chat"
"""

# 示例配置 - 阿里云百炼
"""
API_KEY = "sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
BASE_URL = "https://dashscope.aliyuncs.com/compatible-mode/v1"
MODEL_ID = "qwen-plus"
"""

