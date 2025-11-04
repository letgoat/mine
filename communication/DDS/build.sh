#!/bin/bash

echo "======================================"
echo "  编译DDS架构温度监控系统"
echo "======================================"

# 创建build目录
mkdir -p build

# 编译发布者
echo "[1/2] 编译温度传感器发布者..."
g++ -std=c++11 -pthread sensor_publisher.cpp -o build/sensor_publisher
if [ $? -eq 0 ]; then
    echo "✓ 发布者编译成功"
else
    echo "✗ 发布者编译失败"
    exit 1
fi

# 编译订阅者
echo "[2/2] 编译温度监控订阅者..."
g++ -std=c++11 -pthread monitor_subscriber.cpp -o build/monitor_subscriber
if [ $? -eq 0 ]; then
    echo "✓ 订阅者编译成功"
else
    echo "✗ 订阅者编译失败"
    exit 1
fi

echo ""
echo "======================================"
echo "  编译完成！"
echo "======================================"
echo "可执行文件位置："
echo "  - build/sensor_publisher"
echo "  - build/monitor_subscriber"
echo ""

