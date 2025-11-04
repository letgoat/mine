# CS架构 - 温度监控系统

## 架构说明

这是一个基于**传统客户端-服务器（Client-Server）架构**的温度监控系统。

### 架构特点

- **集中式通信**：所有通信都必须经过中心服务器
- **请求-响应模式**：客户端发送请求，服务器返回响应
- **服务器中心化**：服务器负责接收传感器数据并转发给监控客户端
- **强耦合**：客户端必须知道服务器的地址和端口
- **有状态连接**：使用TCP长连接维护会话

### 系统组成

1. **服务器（server）**
   - 监听8080端口
   - 接受客户端连接
   - 接收传感器数据
   - 广播数据给所有监控客户端
   - 管理客户端会话

2. **传感器客户端（sensor_client）**
   - 连接到服务器
   - 定期上报温度数据
   - 等待服务器确认

3. **监控客户端（monitor_client）**
   - 连接到服务器
   - 接收服务器推送的温度数据
   - 显示温度和告警信息

## 编译方法

```bash
# 给予执行权限
chmod +x build.sh

# 编译
./build.sh
```

或手动编译：

```bash
g++ -std=c++11 -pthread server.cpp -o build/server
g++ -std=c++11 -pthread sensor_client.cpp -o build/sensor_client
g++ -std=c++11 -pthread monitor_client.cpp -o build/monitor_client
```

## 运行方法

### 1. 启动服务器

```bash
./build/server
```

服务器将监听在 `0.0.0.0:8080`

### 2. 启动监控客户端（可启动多个）

在新终端中：

```bash
# 启动第一个监控器
./build/monitor_client monitor_001

# 启动第二个监控器（可选）
./build/monitor_client monitor_002
```

### 3. 启动传感器客户端（可启动多个）

在新终端中：

```bash
# 启动第一个传感器
./build/sensor_client sensor_001

# 启动第二个传感器（可选）
./build/sensor_client sensor_002
```

## 架构优缺点

### 优点

✅ **实现简单** - 逻辑清晰，易于理解  
✅ **集中管理** - 便于统一控制和管理  
✅ **安全控制** - 可在服务器端统一认证和授权  
✅ **数据一致性** - 服务器保证数据的一致性

### 缺点

❌ **单点故障** - 服务器崩溃，整个系统停止工作  
❌ **扩展性差** - 客户端增多时服务器压力大  
❌ **延迟较高** - 数据需要经过服务器中转  
❌ **强依赖** - 客户端必须知道服务器地址  
❌ **资源浪费** - 服务器需要维护所有客户端连接

## 关键代码特点

### 服务器端

```cpp
// 必须维护所有客户端连接
std::vector<ClientInfo> clients;

// 接收传感器数据并转发给监控客户端
void broadcast_to_monitors(const TemperatureData& data) {
    for (auto& client : clients) {
        if (client.type == "monitor") {
            send(client.socket, message.c_str(), message.length(), 0);
        }
    }
}
```

### 客户端

```cpp
// 必须连接到已知的服务器地址
connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

// 发送数据并等待响应
send(sock, data_msg.c_str(), data_msg.length(), 0);
read(sock, buffer, 1024);  // 等待ACK
```

## 适用场景

- 小型局域网应用
- 需要集中管理的系统
- 客户端数量可控的场景
- 对实时性要求不高的应用
- Web应用、企业内部系统

