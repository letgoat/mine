# Socket编程基础

## 什么是Socket？

Socket（套接字）是网络通信的端点，提供进程间通信的接口。

## Socket类型

### 1. TCP Socket（SOCK_STREAM）

面向连接、可靠传输：

\`\`\`cpp
int sock = socket(AF_INET, SOCK_STREAM, 0);
\`\`\`

### 2. UDP Socket（SOCK_DGRAM）

无连接、不可靠传输：

\`\`\`cpp
int sock = socket(AF_INET, SOCK_DGRAM, 0);
\`\`\`

## 服务器端流程

\`\`\`cpp
// 1. 创建socket
int server_fd = socket(AF_INET, SOCK_STREAM, 0);

// 2. 绑定地址
struct sockaddr_in address;
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);
bind(server_fd, (struct sockaddr*)&address, sizeof(address));

// 3. 监听
listen(server_fd, BACKLOG);

// 4. 接受连接
int client_fd = accept(server_fd, nullptr, nullptr);

// 5. 通信
read(client_fd, buffer, size);
write(client_fd, data, len);

// 6. 关闭
close(client_fd);
close(server_fd);
\`\`\`

## 客户端流程

\`\`\`cpp
// 1. 创建socket
int sock = socket(AF_INET, SOCK_STREAM, 0);

// 2. 连接服务器
struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(PORT);
inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

// 3. 通信
write(sock, data, len);
read(sock, buffer, size);

// 4. 关闭
close(sock);
\`\`\`

## 常见问题

### 为什么需要 inet_pton？

\`\`\`cpp
// IP地址从字符串转换为二进制网络字节序
inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
\`\`\`

### 为什么需要 htons？

\`\`\`cpp
// 端口从主机字节序转换为网络字节序
serv_addr.sin_port = htons(8080);
\`\`\`

---

**相关文档**：
- [TCP/IP协议](./tcp-ip.md)
- [CS架构详解](../distributed/cs-architecture.md)

