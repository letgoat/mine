#include "dds_common.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <random>

// DDS数据发布者（Publisher）
class TemperaturePublisher {
private:
    int sockfd;
    struct sockaddr_in multicast_addr;
    std::string publisher_id;
    QoSPolicy qos;
    
public:
    TemperaturePublisher(const std::string& id) : publisher_id(id) {
        // 创建UDP socket
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            throw std::runtime_error("创建socket失败");
        }
        
        // 设置组播地址
        memset(&multicast_addr, 0, sizeof(multicast_addr));
        multicast_addr.sin_family = AF_INET;
        multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);
        multicast_addr.sin_port = htons(MULTICAST_PORT);
        
        // 设置TTL（Time To Live）
        int ttl = 32;
        if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
            std::cerr << "警告: 设置TTL失败" << std::endl;
        }
        
        std::cout << "[DDS发布者 " << publisher_id << "] 初始化完成" << std::endl;
        std::cout << "[DDS发布者] 组播组: " << MULTICAST_GROUP << ":" << MULTICAST_PORT << std::endl;
        std::cout << "[DDS发布者] Topic: " << TEMPERATURE_TOPIC << std::endl;
    }
    
    ~TemperaturePublisher() {
        close(sockfd);
    }
    
    // 发布温度数据（核心DDS功能）
    bool publish(const TemperatureData& data) {
        std::string serialized_data = data.serialize();
        
        // 通过组播发送数据（无需知道订阅者是谁）
        ssize_t sent = sendto(sockfd, serialized_data.c_str(), serialized_data.length(), 0,
                             (struct sockaddr*)&multicast_addr, sizeof(multicast_addr));
        
        if (sent < 0) {
            std::cerr << "[DDS发布者] 发送失败" << std::endl;
            return false;
        }
        
        std::cout << "[DDS发布者] 发布数据: " << data.sensor_id 
                 << " -> " << data.temperature << "°C" << std::endl;
        return true;
    }
    
    void set_qos(const QoSPolicy& qos_policy) {
        qos = qos_policy;
    }
};

// 生成随机温度数据
float generate_temperature() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(18.0, 32.0);
    return dis(gen);
}

int main(int argc, char* argv[]) {
    std::string sen
    
    sor_id = "sensor_001";
    
    if (argc > 1) {
        sensor_id = argv[1];
    }
    
    try {
        std::cout << "========================================" << std::endl;
        std::cout << "   DDS架构 - 温度传感器发布者" << std::endl;
        std::cout << "========================================" << std::endl;
        
        // 创建DDS发布者（无需连接到服务器！）
        TemperaturePublisher publisher(sensor_id);
        
        // 设置QoS策略
        QoSPolicy qos;
        qos.reliability = ReliabilityQoS::BEST_EFFORT;
        qos.durability = DurabilityQoS::VOLATILE;
        publisher.set_qos(qos);
        
        std::cout << "[DDS发布者] 开始发布温度数据..." << std::endl;
        
        // 定期发布温度数据
        int count = 0;
        while (count < 20) {  // 发布20次后退出
            std::this_thread::sleep_for(std::chrono::seconds(2));
            
            float temperature = generate_temperature();
            long timestamp = get_timestamp();
            
            // 创建并发布数据
            TemperatureData data(sensor_id, temperature, timestamp);
            publisher.publish(data);
            
            count++;
        }
        
        std::cout << "[DDS发布者] 发布完成，退出" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}

