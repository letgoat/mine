# DDS架构详解

## 什么是DDS？

**DDS**（Data Distribution Service，数据分发服务）是一种**去中心化**的分布式系统架构，采用**发布-订阅模式**，节点之间直接通信，无需中心服务器。

## 架构特点

### 核心特征

1. **去中心化** - 无需中心服务器
2. **发布-订阅模式** - 数据驱动的通信
3. **松耦合** - 发布者和订阅者互不感知
4. **自动发现** - 节点自动发现和连接
5. **QoS保障** - 灵活的服务质量策略

### 通信模型

\`\`\`
发布者1 ──┐
发布者2 ──┼──> [DDS总线/组播] <──┼── 订阅者1
发布者3 ──┘                     └── 订阅者2
\`\`\`

## 核心概念

### 1. Topic（主题）

数据的逻辑分类，发布者和订阅者通过Topic关联：

\`\`\`cpp
const char* TEMPERATURE_TOPIC = "TemperatureData";
\`\`\`

### 2. Publisher（发布者）

发布数据的节点，无需知道谁在订阅：

\`\`\`cpp
class TemperaturePublisher {
public:
    bool publish(const TemperatureData& data) {
        // 通过组播发送，无需知道订阅者
        sendto(sockfd, data, len, 0, &multicast_addr, ...);
    }
};
\`\`\`

### 3. Subscriber（订阅者）

订阅数据的节点，自动接收匹配的数据：

\`\`\`cpp
class TemperatureSubscriber {
public:
    void start() {
        // 加入组播组，自动接收数据
        setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, ...);
    }
};
\`\`\`

### 4. QoS（服务质量）

定义数据传输的策略：

\`\`\`cpp
enum class ReliabilityQoS {
    BEST_EFFORT,    // 尽力而为，允许丢失
    RELIABLE        // 可靠传输
};

enum class DurabilityQoS {
    VOLATILE,       // 易失性，不保存历史
    TRANSIENT_LOCAL // 暂态本地，保存历史
};
\`\`\`

## 实现示例

### 发布者实现

\`\`\`cpp
int main() {
    // 1. 创建UDP socket（不是TCP！）
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // 2. 配置组播地址
    struct sockaddr_in multicast_addr;
    multicast_addr.sin_addr.s_addr = inet_addr("239.255.0.1");
    multicast_addr.sin_port = htons(9090);
    
    // 3. 直接发布数据（无需连接！）
    TemperatureData data("sensor_001", 25.5, timestamp);
    std::string msg = data.serialize();
    
    sendto(sockfd, msg.c_str(), msg.length(), 0,
           (struct sockaddr*)&multicast_addr, sizeof(multicast_addr));
    
    return 0;
}
\`\`\`

### 订阅者实现

\`\`\`cpp
int main() {
    // 1. 创建UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // 2. 绑定到本地端口
    struct sockaddr_in local_addr;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(9090);
    bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr));
    
    // 3. 加入组播组（自动发现！）
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("239.255.0.1");
    mreq.imr_interface.s_addr = INADDR_ANY;
    setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    
    // 4. 接收数据（来自任何发布者）
    while (true) {
        recvfrom(sockfd, buffer, size, 0, nullptr, nullptr);
        // 处理数据
    }
    
    return 0;
}
\`\`\`

## 关键技术

### 1. 组播通信（Multicast）

\`\`\`cpp
// 组播地址范围：224.0.0.0 ~ 239.255.255.255
const char* MULTICAST_GROUP = "239.255.0.1";
const int MULTICAST_PORT = 9090;

// 发布者：发送到组播地址
sendto(sockfd, data, len, 0, &multicast_addr, ...);

// 订阅者：加入组播组自动接收
setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, ...);
\`\`\`

**优势**：
- 一次发送，多个接收者
- 自动发现机制
- 减少网络带宽

### 2. enum class（强类型枚举）

\`\`\`cpp
// 传统 enum（不推荐）
enum ReliabilityQoS {
    BEST_EFFORT,
    RELIABLE
};
ReliabilityQoS qos = BEST_EFFORT;  // 可能命名冲突

// enum class（推荐）
enum class ReliabilityQoS {
    BEST_EFFORT,
    RELIABLE
};
ReliabilityQoS qos = ReliabilityQoS::BEST_EFFORT;  // 类型安全
\`\`\`

**优势**：
- ✅ 作用域隔离
- ✅ 类型安全
- ✅ 防止隐式转换

### 3. 数据序列化

\`\`\`cpp
struct TemperatureData {
    char sensor_id[64];
    float temperature;
    long timestamp;
    
    // 序列化为字符串
    std::string serialize() const {
        return "TOPIC:TemperatureData|ID:" + 
               std::string(sensor_id) + "|TEMP:" + 
               std::to_string(temperature) + "|TS:" + 
               std::to_string(timestamp);
    }
    
    // 反序列化
    static TemperatureData deserialize(const std::string& data);
};
\`\`\`

## DDS vs CS 架构对比

### 启动顺序

**CS架构**：必须按顺序启动
\`\`\`bash
# 1. 必须先启动服务器
./server

# 2. 然后启动客户端
./client
\`\`\`

**DDS架构**：任意顺序启动
\`\`\`bash
# 可以先启动订阅者
./subscriber

# 后启动发布者（订阅者自动接收）
./publisher
\`\`\`

### 代码对比

| 操作 | CS架构 | DDS架构 |
|------|--------|---------|
| **发送数据** | `connect()` + `send()` | 直接 `sendto()` |
| **接收数据** | `accept()` + `read()` | 直接 `recvfrom()` |
| **连接管理** | 需要维护连接 | 无需连接 |
| **地址配置** | 硬编码服务器地址 | 组播地址 |

## 优缺点分析

### ✅ 优点

| 优点 | 说明 |
|------|------|
| **高可用性** | 无单点故障 |
| **低延迟** | 点对点通信，无需中转 |
| **良好扩展性** | 轻松添加节点 |
| **松耦合** | 发布者和订阅者互不感知 |
| **灵活部署** | 任意顺序启动 |

### ❌ 缺点

| 缺点 | 说明 |
|------|------|
| **学习曲线陡** | 概念较多 |
| **配置复杂** | QoS策略需要经验 |
| **调试困难** | 去中心化导致问题定位难 |
| **安全挑战** | 去中心化的安全管理复杂 |

## 适用场景

### ✅ 适合使用DDS

- **分布式实时系统** - 工业控制、交易系统
- **物联网（IoT）** - 大量传感器节点
- **自动驾驶** - 车辆间通信
- **航空航天** - 高可靠性要求
- **金融交易** - 超低延迟要求

### 典型应用

- **ROS 2**（机器人操作系统）使用DDS作为通信中间件
- **工业4.0** 智能制造系统
- **智能交通** 车联网V2X通信

## 实践建议

1. **网络配置**：确保支持组播通信
2. **QoS选择**：根据业务需求选择合适的QoS策略
3. **数据模型**：设计清晰的Topic和数据结构
4. **测试验证**：充分测试节点动态加入/退出场景

## 生产级DDS实现

本文档的示例是简化实现，生产环境推荐使用成熟的DDS实现：

- **FastDDS**（eProsima）- 开源，性能优秀
- **CycloneDDS**（Eclipse）- 轻量级
- **OpenDDS** - 完整的DDS规范实现
- **RTI Connext DDS** - 商业产品，功能强大

## 总结

DDS架构是现代分布式系统的重要模式，特别适合**高实时性、高可用性、大规模**的应用场景。虽然实现复杂度较高，但带来的架构优势在关键应用中非常明显。

---

**相关文档**：
- [CS架构详解](./cs-architecture.md)
- [分布式系统概述](./intro.md)
- [网络编程基础](../network/socket.md)

