#include "dds_common.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <functional>
#include <vector>

// DDS数据订阅者（Subscriber）
class TemperatureSubscriber {
private:
    int sockfd;
    struct sockaddr_in local_addr;
    struct ip_mreq multicast_request;
    std::string subscriber_id;
    QoSPolicy qos;
    bool running;
    std::vector<TemperatureData> history;
    
    // 回调函数类型
    using DataCallback = std::function<void(const TemperatureData&)>;
    DataCallback on_data_available;
    
public:
    TemperatureSubscriber(const std::string& id) 
        : subscriber_id(id), running(false) {
        
        // 创建UDP socket
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            throw std::runtime_error("创建socket失败");
        }
        
        // 允许多个订阅者绑定到同一端口
        int reuse = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
            throw std::runtime_error("设置SO_REUSEADDR失败");
        }
        
        // 绑定到本地地址
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_family = AF_INET;
        local_addr.sin_addr.s_addr = INADDR_ANY;
        local_addr.sin_port = htons(MULTICAST_PORT);
        
        if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
            throw std::runtime_error("绑定失败");
        }
        
        // 加入组播组（DDS自动发现机制）
        multicast_request.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);
        multicast_request.imr_interface.s_addr = INADDR_ANY;
        
        if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                      &multicast_request, sizeof(multicast_request)) < 0) {
            throw std::runtime_error("加入组播组失败");
        }
        
        std::cout << "[DDS订阅者 " << subscriber_id << "] 初始化完成" << std::endl;
        std::cout << "[DDS订阅者] 监听组播组: " << MULTICAST_GROUP << ":" << MULTICAST_PORT << std::endl;
        std::cout << "[DDS订阅者] 订阅Topic: " << TEMPERATURE_TOPIC << std::endl;
    }
    
    ~TemperatureSubscriber() {
        stop();
        
        // 离开组播组
        setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, 
                  &multicast_request, sizeof(multicast_request));
        
        close(sockfd);
    }
    
    // 设置数据到达时的回调函数
    void set_listener(DataCallback callback) {
        on_data_available = callback;
    }
    
    void set_qos(const QoSPolicy& qos_policy) {
        qos = qos_policy;
    }
    
    // 开始监听数据（非阻塞）
    void start() {
        running = true;
        std::thread(&TemperatureSubscriber::receive_loop, this).detach();
    }
    
    void stop() {
        running = false;
    }
    
    // 获取历史数据
    const std::vector<TemperatureData>& get_history() const {
        return history;
    }
    
private:
    // 接收循环
    void receive_loop() {
        char buffer[1024];
        
        while (running) {
            memset(buffer, 0, sizeof(buffer));
            
            // 接收组播数据（来自任何发布者）
            ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, nullptr, nullptr);
            
            if (recv_len > 0) {
                buffer[recv_len] = '\0';
                std::string data_str(buffer);
                
                // 检查是否是正确的Topic
                if (data_str.find(std::string("TOPIC:") + TEMPERATURE_TOPIC) == 0) {
                    try {
                        // 反序列化数据
                        TemperatureData data = TemperatureData::deserialize(data_str);
                        
                        // 存储历史数据（根据QoS策略）
                        if (qos.durability == DurabilityQoS::TRANSIENT_LOCAL) {
                            history.push_back(data);
                            if (history.size() > (size_t)qos.history_depth) {
                                history.erase(history.begin());
                            }
                        }
                        
                        // 调用回调函数
                        if (on_data_available) {
                            on_data_available(data);
                        }
                        
                    } catch (const std::exception& e) {
                        std::cerr << "[DDS订阅者] 解析数据失败: " << e.what() << std::endl;
                    }
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    std::string monitor_id = "monitor_001";
    
    if (argc > 1) {
        monitor_id = argv[1];
    }
    
    try {
        std::cout << "========================================" << std::endl;
        std::cout << "   DDS架构 - 温度监控订阅者" << std::endl;
        std::cout << "========================================" << std::endl;
        
        // 创建DDS订阅者（无需连接到服务器！）
        TemperatureSubscriber subscriber(monitor_id);
        
        // 设置QoS策略
        QoSPolicy qos;
        qos.reliability = ReliabilityQoS::BEST_EFFORT;
        qos.durability = DurabilityQoS::TRANSIENT_LOCAL;
        qos.history_depth = 10;
        subscriber.set_qos(qos);
        
        // 设置数据监听器（回调函数）
        subscriber.set_listener([](const TemperatureData& data) {
            std::cout << "[DDS订阅者] 收到数据: " << data.sensor_id 
                     << " -> " << data.temperature << "°C";
            
            // 温度告警
            if (data.temperature > 30.0) {
                std::cout << " ⚠️ 高温警告！";
            } else if (data.temperature < 20.0) {
                std::cout << " ❄️ 低温警告！";
            }
            std::cout << std::endl;
        });
        
        // 开始监听
        subscriber.start();
        std::cout << "[DDS订阅者] 开始接收数据..." << std::endl;
        
        // 保持运行
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}

