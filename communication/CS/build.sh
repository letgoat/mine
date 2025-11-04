#!/bin/bash

echo "======================================"
echo "  编译CS架构温度监控系统"
echo "======================================"

# 创建build目录
mkdir -p build

# 编译服务器
echo "[1/3] 编译服务器..."
g++ -std=c++11 -pthread server.cpp -o build/server
if [ $? -eq 0 ]; then
    echo "✓ 服务器编译成功"
else
    echo "✗ 服务器编译失败"
    exit 1
fi

# 编译传感器客户端
echo "[2/3] 编译传感器客户端..."
g++ -std=c++11 -pthread sensor_client.cpp -o build/sensor_client
if [ $? -eq 0 ]; then
    echo "✓ 传感器客户端编译成功"
else
    echo "✗ 传感器客户端编译失败"
    exit 1
fi

# 编译监控客户端
echo "[3/3] 编译监控客户端..."
g++ -std=c++11 -pthread monitor_client.cpp -o build/monitor_client
if [ $? -eq 0 ]; then
    echo "✓ 监控客户端编译成功"
else
    echo "✗ 监控客户端编译失败"
    exit 1
fi

echo ""
echo "======================================"
echo "  编译完成！"
echo "======================================"
echo "可执行文件位置："
echo "  - build/server"
echo "  - build/sensor_client"
echo "  - build/monitor_client"
echo ""

