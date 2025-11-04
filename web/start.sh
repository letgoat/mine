#!/bin/bash

echo "=========================================="
echo "  五毛仔学技术 - 快速启动脚本"
echo "=========================================="
echo ""

# 检测可用的HTTP服务器
if command -v python3 &> /dev/null; then
    echo "✓ 使用 Python3 启动服务器..."
    echo "访问地址: http://localhost:8000"
    echo "按 Ctrl+C 停止服务器"
    echo ""
    python3 -m http.server 8000
elif command -v python &> /dev/null; then
    echo "✓ 使用 Python 启动服务器..."
    echo "访问地址: http://localhost:8000"
    echo "按 Ctrl+C 停止服务器"
    echo ""
    python -m SimpleHTTPServer 8000
elif command -v php &> /dev/null; then
    echo "✓ 使用 PHP 启动服务器..."
    echo "访问地址: http://localhost:8000"
    echo "按 Ctrl+C 停止服务器"
    echo ""
    php -S localhost:8000
else
    echo "❌ 未找到可用的HTTP服务器"
    echo ""
    echo "请安装以下工具之一："
    echo "  - Python3: sudo apt install python3"
    echo "  - PHP: sudo apt install php"
    echo "  - Node.js http-server: npm install -g http-server"
    echo ""
    echo "或者使用VS Code的Live Server插件"
    exit 1
fi

