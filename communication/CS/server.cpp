#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

const int PORT = 8080;
const int MAX_CLIENTS = 10;

// 温度数据结构
struct TemperatureData {
    std::string sensor_id;
    float temperature;
    long timestamp;
};

// 客户端信息
struct ClientInfo {
    int socket;
    std::string type; // "sensor" 或 "monitor"
    std::string id;
};

// 全局变量
std::vector<ClientInfo> clients;
std::mutex clients_mutex;
TemperatureData latest_temperature;
std::mutex data_mutex;

// 广播温度数据给所有监控客户端
void broadcast_to_monitors(const TemperatureData& data) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    
    std::string message = "TEMP|" + data.sensor_id + "|" + 
                         std::to_string(data.temperature) + "|" + 
                         std::to_string(data.timestamp) + "\n";
    
    for (auto& client : clients) {
        if (client.type == "monitor") {
            send(client.socket, message.c_str(), message.length(), 0);
        }
    }
}

// 处理客户端连接
void handle_client(int client_socket) {
    char buffer[1024] = {0};
    ClientInfo client_info;
    client_info.socket = client_socket;
    
    // 首先接收客户端类型
    int valread = read(client_socket, buffer, 1024);
    if (valread <= 0) {
        close(client_socket);
        return;
    }
    
    std::string msg(buffer);
    
    // 解析客户端类型: "REGISTER|sensor|sensor_001" 或 "REGISTER|monitor|monitor_001"
    if (msg.find("REGISTER|") == 0) {
        size_t pos1 = msg.find('|', 9);
        size_t pos2 = msg.find('|', pos1 + 1);
        
        client_info.type = msg.substr(9, pos1 - 9);
        client_info.id = msg.substr(pos1 + 1, pos2 - pos1 - 1);
        
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_info);
        }
        
        std::cout << "[服务器] " << client_info.type << " " << client_info.id << " 已连接" << std::endl;
        
        std::string response = "OK|已注册成功\n";
        send(client_socket, response.c_str(), response.length(), 0);
    }
    
    // 持续处理客户端消息
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        valread = read(client_socket, buffer, 1024);
        
        if (valread <= 0) {
            break;
        }
        
        std::string message(buffer);
        
        // 处理传感器上报的温度数据: "DATA|25.5|1234567890"
        if (message.find("DATA|") == 0) {
            size_t pos1 = message.find('|', 5);
            size_t pos2 = message.find('|', pos1 + 1);
            
            TemperatureData temp_data;
            temp_data.sensor_id = client_info.id;
            temp_data.temperature = std::stof(message.substr(5, pos1 - 5));
            temp_data.timestamp = std::stol(message.substr(pos1 + 1, pos2 - pos1 - 1));
            
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                latest_temperature = temp_data;
            }
            
            std::cout << "[服务器] 收到温度数据: " << temp_data.sensor_id 
                     << " -> " << temp_data.temperature << "°C" << std::endl;
            
            // 广播给所有监控客户端
            broadcast_to_monitors(temp_data);
            
            std::string response = "ACK\n";
            send(client_socket, response.c_str(), response.length(), 0);
        }
        // 处理监控客户端的查询请求: "QUERY"
        else if (message.find("QUERY") == 0) {
            std::lock_guard<std::mutex> lock(data_mutex);
            std::string response = "TEMP|" + latest_temperature.sensor_id + "|" + 
                                 std::to_string(latest_temperature.temperature) + "|" + 
                                 std::to_string(latest_temperature.timestamp) + "\n";
            send(client_socket, response.c_str(), response.length(), 0);
        }
    }
    
    // 客户端断开连接
    std::cout << "[服务器] " << client_info.type << " " << client_info.id << " 已断开" << std::endl;
    
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(
            std::remove_if(clients.begin(), clients.end(),
                [client_socket](const ClientInfo& c) { return c.socket == client_socket; }),
            clients.end()
        );
    }
    
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // 创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // 设置socket选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // 绑定
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // 监听
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "========================================" << std::endl;
    std::cout << "   CS架构 - 温度监控服务器" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "[服务器] 正在监听端口 " << PORT << "..." << std::endl;
    
    // 接受连接
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
        
        // 为每个客户端创建一个线程
        std::thread(handle_client, new_socket).detach();
    }
    
    return 0;
}

