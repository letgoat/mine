#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;

// 持续接收服务器推送的温度数据
void receive_updates(int sock) {
    char buffer[1024];
    
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, 1024);
        
        if (valread <= 0) {
            std::cout << "[监控器] 连接已断开" << std::endl;
            break;
        }
        
        std::string message(buffer);
        
        // 解析温度数据: "TEMP|sensor_001|25.5|1234567890"
        if (message.find("TEMP|") == 0) {
            size_t pos1 = message.find('|', 5);
            size_t pos2 = message.find('|', pos1 + 1);
            size_t pos3 = message.find('|', pos2 + 1);
            
            std::string sensor_id = message.substr(5, pos1 - 5);
            float temperature = std::stof(message.substr(pos1 + 1, pos2 - pos1 - 1));
            
            std::cout << "[监控器] 收到温度数据: " << sensor_id 
                     << " -> " << temperature << "°C";
            
            // 温度告警
            if (temperature > 30.0) {
                std::cout << " ⚠️ 高温警告！";
            } else if (temperature < 20.0) {
                std::cout << " ❄️ 低温警告！";
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    std::string monitor_id = "monitor_001";
    
    if (argc > 1) {
        monitor_id = argv[1];
    }
    
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    // 创建socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket创建失败" << std::endl;
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    
    // 转换IP地址
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "无效的地址" << std::endl;
        return -1;
    }
    
    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "连接失败" << std::endl;
        return -1;
    }
    
    std::cout << "========================================" << std::endl;
    std::cout << "   CS架构 - 温度监控客户端" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "[监控器 " << monitor_id << "] 已连接到服务器" << std::endl;
    
    // 注册为监控器
    std::string register_msg = "REGISTER|monitor|" + monitor_id + "\n";
    send(sock, register_msg.c_str(), register_msg.length(), 0);
    
    // 接收注册响应
    read(sock, buffer, 1024);
    std::cout << "[监控器] 服务器响应: " << buffer << std::endl;
    std::cout << "[监控器] 开始接收温度数据..." << std::endl;
    
    // 持续接收温度更新
    receive_updates(sock);
    
    close(sock);
    
    return 0;
}

