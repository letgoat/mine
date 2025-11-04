# DDS架构 - 温度监控系统

## 架构说明

这是一个基于**DDS（Data Distribution Service）架构**的温度监控系统，采用发布-订阅模式。

### 架构特点

- **去中心化**：无需中心服务器，节点之间直接通信
- **发布-订阅模式**：发布者和订阅者松耦合
- **数据中心**：以数据为中心，而非服务为中心
- **自动发现**：通过组播机制自动发现其他节点
- **QoS保障**：支持多种服务质量策略
- **实时性强**：低延迟的点对点数据分发

### 系统组成

1. **传感器发布者（sensor_publisher）**
   - 发布温度数据到DDS域
   - 无需知道订阅者是谁
   - 通过组播自动分发数据
   - 配置QoS策略

2. **监控订阅者（monitor_subscriber）**
   - 订阅温度数据Topic
   - 自动接收所有匹配的数据
   - 无需连接到特定地址
   - 支持历史数据缓存

### DDS核心概念

- **Topic（主题）**：数据的逻辑分类，本例为"TemperatureData"
- **Publisher（发布者）**：发布数据的节点
- **Subscriber（订阅者）**：订阅数据的节点
- **QoS（服务质量）**：定义数据传输的可靠性、持久性等策略
- **Domain（域）**：通过组播地址实现的逻辑通信域

## 编译方法

```bash
# 给予执行权限
chmod +x build.sh

# 编译
./build.sh
```

或手动编译：

```bash
g++ -std=c++11 -pthread sensor_publisher.cpp -o build/sensor_publisher
g++ -std=c++11 -pthread monitor_subscriber.cpp -o build/monitor_subscriber
```

## 运行方法

### 特别说明

**无需启动服务器！** DDS是去中心化架构，可以按任意顺序启动节点。

### 1. 启动监控订阅者（可启动多个）

在终端中：

```bash
# 启动第一个订阅者
./build/monitor_subscriber monitor_001

# 启动第二个订阅者（可选，在新终端）
./build/monitor_subscriber monitor_002
```

### 2. 启动传感器发布者（可启动多个）

在新终端中：

```bash
# 启动第一个发布者
./build/sensor_publisher sensor_001

# 启动第二个发布者（可选，在新终端）
./build/sensor_publisher sensor_002
```

### 3. 观察效果

- 所有订阅者都会收到所有发布者的数据
- 可以随时启动或停止任何节点
- 节点之间自动发现，无需配置

## 架构优缺点

### 优点

✅ **高可用性** - 无单点故障，任何节点失败不影响其他节点  
✅ **良好扩展性** - 轻松添加新的发布者或订阅者  
✅ **低延迟** - 点对点通信，无需中间服务器  
✅ **松耦合** - 发布者和订阅者互不感知  
✅ **灵活部署** - 可按任意顺序启动节点  
✅ **多对多通信** - 支持多个发布者和订阅者同时工作

### 缺点

❌ **学习曲线陡** - 概念较多，理解成本高  
❌ **配置复杂** - QoS策略配置需要经验  
❌ **调试困难** - 去中心化导致问题定位较难  
❌ **资源开销** - 每个节点都需要运行发现和通信协议  
❌ **安全挑战** - 去中心化的安全管理更复杂

## 关键代码特点

### 发布者

```cpp
// 无需连接到服务器，直接发布数据
TemperaturePublisher publisher(sensor_id);

// 通过组播发送数据，无需知道订阅者
TemperatureData data(sensor_id, temperature, timestamp);
publisher.publish(data);  // 自动分发给所有订阅者
```

### 订阅者

```cpp
// 无需连接，加入组播组即可自动接收数据
TemperatureSubscriber subscriber(monitor_id);

// 设置回调函数处理收到的数据
subscriber.set_listener([](const TemperatureData& data) {
    // 处理数据
});

subscriber.start();  // 开始监听
```

### QoS策略配置

```cpp
QoSPolicy qos;
qos.reliability = ReliabilityQoS::BEST_EFFORT;     // 尽力而为
qos.durability = DurabilityQoS::TRANSIENT_LOCAL;   // 保存历史
qos.history_depth = 10;                             // 历史深度
```

## DDS与CS架构对比

| 特性 | CS架构 | DDS架构 |
|------|--------|---------|
| 通信模式 | 请求-响应 | 发布-订阅 |
| 服务器 | 必需（中心化） | 不需要（去中心化） |
| 耦合度 | 强耦合 | 松耦合 |
| 单点故障 | 有（服务器） | 无 |
| 扩展性 | 受限 | 优秀 |
| 实时性 | 一般 | 优秀 |
| 配置复杂度 | 简单 | 较复杂 |
| 启动顺序 | 必须先启动服务器 | 任意顺序 |
| 多对多通信 | 通过服务器中转 | 原生支持 |

## 适用场景

- 分布式实时系统
- 物联网（IoT）平台
- 工业控制系统
- 自动驾驶车辆通信
- 金融交易系统
- 军事指挥系统
- 航空航天系统
- 大规模传感器网络

## 技术细节

### 组播通信

本实现使用UDP组播（Multicast）实现DDS的核心功能：

- **组播地址**：239.255.0.1:9090
- **自动发现**：节点加入组播组后自动接收数据
- **无需配置**：发布者和订阅者无需知道对方地址

### QoS实现

简化实现了两个关键QoS策略：

- **Reliability（可靠性）**：BEST_EFFORT（本例）/ RELIABLE
- **Durability（持久性）**：VOLATILE / TRANSIENT_LOCAL（缓存历史数据）

## 注意事项

1. **防火墙设置**：确保允许组播通信（239.255.0.1:9090）
2. **网络支持**：确认网络环境支持组播
3. **多网卡环境**：可能需要指定网卡接口
4. **生产环境**：建议使用成熟的DDS实现（如FastDDS、RTI Connext DDS）

## 扩展建议

如需在生产环境使用DDS，推荐以下开源实现：

- **FastDDS**（eProsima）
- **CycloneDDS**（Eclipse）
- **OpenDDS**

