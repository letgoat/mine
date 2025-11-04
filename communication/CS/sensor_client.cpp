#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <random>

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;

// 生成随机温度数据（模拟传感器）
float generate_temperature() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(18.0, 32.0);
    return dis(gen);
}

// 获取当前时间戳
long get_timestamp() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

int main(int argc, char* argv[]) {
    std::string sensor_id = "sensor_001";
    
    if (argc > 1) {
        sensor_id = argv[1];
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
    std::cout << "   CS架构 - 温度传感器客户端" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "[传感器 " << sensor_id << "] 已连接到服务器" << std::endl;
    
    // 注册为传感器
    std::string register_msg = "REGISTER|sensor|" + sensor_id + "\n";
    send(sock, register_msg.c_str(), register_msg.length(), 0);
    
    // 接收注册响应
    read(sock, buffer, 1024);
    std::cout << "[传感器] 服务器响应: " << buffer << std::endl;
    
    // 定期发送温度数据
    int count = 0;
    while (count < 20) {  // 发送20次后退出
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        float temperature = generate_temperature();
        long timestamp = get_timestamp();
        
        // 发送温度数据
        std::string data_msg = "DATA|" + std::to_string(temperature) + "|" + 
                              std::to_string(timestamp) + "\n";
        send(sock, data_msg.c_str(), data_msg.length(), 0);
        
        std::cout << "[传感器] 上报温度: " << temperature << "°C" << std::endl;
        
        // 接收确认
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, 1024);
        
        count++;
    }
    
    std::cout << "[传感器] 断开连接" << std::endl;
    close(sock);
    
    return 0;
}

