# CS架构详解

## 什么是CS架构？

**CS架构**（Client-Server Architecture，客户端-服务器架构）是最经典的分布式系统架构模式，所有客户端的通信都必须通过中心服务器进行。

## 架构特点

### 核心特征

1. **集中式通信** - 所有数据必须经过服务器
2. **请求-响应模式** - 客户端发送请求，服务器返回响应
3. **服务器中心化** - 服务器负责业务逻辑和数据管理
4. **强耦合** - 客户端必须知道服务器地址

### 通信模型

\`\`\`
传感器1 ──┐
传感器2 ──┼──> [中心服务器] ──┼──> 监控器1
传感器3 ──┘                   └──> 监控器2
\`\`\`

## 实现示例：温度监控系统

### 服务器端实现

服务器是CS架构的核心，负责：
- 监听端口，接受客户端连接
- 管理所有客户端会话
- 接收传感器数据并转发给监控客户端

\`\`\`cpp
// 服务器核心流程
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    
    // 1. 创建socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. 设置地址复用（重要！）
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
               &opt, sizeof(opt));
    
    // 3. 绑定地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // 监听所有接口
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    // 4. 开始监听
    listen(server_fd, MAX_CLIENTS);
    
    // 5. 循环接受连接
    while (true) {
        new_socket = accept(server_fd, ...);
        
        // 为每个客户端创建线程
        std::thread(handle_client, new_socket).detach();
    }
    
    return 0;
}
\`\`\`

### 客户端实现

客户端必须主动连接服务器：

\`\`\`cpp
// 客户端连接流程
int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // 1. 创建socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. 配置服务器地址（硬编码！）
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);
    
    // 3. 连接到服务器（关键步骤）
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    // 4. 发送数据
    send(sock, data, len, 0);
    
    // 5. 接收响应
    read(sock, buffer, size);
    
    return 0;
}
\`\`\`

## 关键技术点

### 1. Socket编程

#### 地址转换
\`\`\`cpp
// 为什么需要 inet_pton？
const char* SERVER_IP = "127.0.0.1";  // 字符串格式

// Socket需要二进制格式 + 网络字节序
inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);
// "127.0.0.1" → 0x7F000001 (网络字节序)
\`\`\`

#### 端口转换
\`\`\`cpp
// htons = Host TO Network Short
serv_addr.sin_port = htons(8080);
// 主机字节序 → 网络字节序（大端序）
\`\`\`

### 2. 多线程处理

服务器使用多线程处理多个客户端：

\`\`\`cpp
// 主线程：接受连接
while (true) {
    new_socket = accept(server_fd, ...);
    
    // 子线程：处理客户端
    std::thread(handle_client, new_socket).detach();
}
\`\`\`

**优势**：
- 并发处理多个客户端
- 主线程继续接受新连接

**问题**：
- 线程数量有限
- 高并发时性能下降

### 3. TCP三次握手

\`\`\`
客户端                          服务器
   |                               |
   |------- SYN（同步）---------->  |
   |<------ SYN-ACK -------------|  
   |------- ACK（确认）--------->  |
   |                               |
   [连接建立]                      |
\`\`\`

## 优缺点分析

### ✅ 优点

| 优点 | 说明 |
|------|------|
| **实现简单** | 逻辑清晰，容易理解 |
| **集中管理** | 便于统一控制和管理 |
| **安全控制** | 可在服务器端统一认证 |
| **数据一致性** | 服务器保证数据一致 |

### ❌ 缺点

| 缺点 | 说明 |
|------|------|
| **单点故障** | 服务器崩溃导致系统瘫痪 |
| **性能瓶颈** | 所有数据必须经过服务器 |
| **扩展性差** | 客户端增多时服务器压力大 |
| **强依赖** | 客户端必须知道服务器地址 |

## 适用场景

### ✅ 适合使用CS架构

- 小型系统（客户端 < 100）
- 需要集中管理和控制
- 实时性要求不高
- Web应用、企业内部系统

### ❌ 不适合使用CS架构

- 大规模分布式系统
- 高实时性要求（< 10ms）
- 需要高可用性
- 多对多通信场景

## 与DDS架构对比

| 特性 | CS架构 | DDS架构 |
|------|--------|---------|
| **通信模式** | 请求-响应 | 发布-订阅 |
| **服务器依赖** | 必需 | 不需要 |
| **耦合度** | 强耦合 | 松耦合 |
| **单点故障** | 有 | 无 |
| **扩展性** | 受限 | 优秀 |
| **实时性** | 一般 | 优秀 |
| **实现复杂度** | 简单 | 较复杂 |

## 实践建议

1. **开发阶段**：设置 `SO_REUSEADDR` 允许快速重启
2. **生产环境**：考虑负载均衡和高可用
3. **性能优化**：使用线程池替代动态创建线程
4. **错误处理**：完善的异常处理和重连机制

## 总结

CS架构是最经典、最简单的分布式架构，适合中小型系统。但在高并发、高可用场景下，应考虑更现代的架构如微服务、DDS等。

---

**相关文档**：
- [DDS架构详解](./dds-architecture.md)
- [Socket编程](../network/socket.md)
- [TCP/IP协议](../network/tcp-ip.md)

