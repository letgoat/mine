#ifndef DDS_COMMON_H
#define DDS_COMMON_H

#include <string>
#include <cstring>
#include <chrono>

// DDS Topic名称
const char* TEMPERATURE_TOPIC = "TemperatureData";

// 组播地址和端口（用于DDS自动发现和数据分发）
const char* MULTICAST_GROUP = "239.255.0.1";
const int MULTICAST_PORT = 9090;

// 温度数据结构（DDS Data Type）
struct TemperatureData {
    char sensor_id[64];
    float temperature;
    long timestamp;
    
    TemperatureData() : temperature(0.0f), timestamp(0) {
        memset(sensor_id, 0, sizeof(sensor_id));
    }
    
    TemperatureData(const std::string& id, float temp, long ts) 
        : temperature(temp), timestamp(ts) {
        strncpy(sensor_id, id.c_str(), sizeof(sensor_id) - 1);
    }
    
    // 序列化为字符串（用于网络传输）
    std::string serialize() const {
        return std::string("TOPIC:") + TEMPERATURE_TOPIC + "|" +
               "ID:" + sensor_id + "|" +
               "TEMP:" + std::to_string(temperature) + "|" +
               "TS:" + std::to_string(timestamp);
    }
    
    // 从字符串反序列化
    static TemperatureData deserialize(const std::string& data) {
        TemperatureData result;
        
        // 解析: "TOPIC:TemperatureData|ID:sensor_001|TEMP:25.5|TS:1234567890"
        size_t id_pos = data.find("ID:") + 3;
        size_t temp_pos = data.find("TEMP:") + 5;
        size_t ts_pos = data.find("TS:") + 3;
        
        size_t id_end = data.find('|', id_pos);
        size_t temp_end = data.find('|', temp_pos);
        
        std::string id = data.substr(id_pos, id_end - id_pos);
        float temp = std::stof(data.substr(temp_pos, temp_end - temp_pos));
        long ts = std::stol(data.substr(ts_pos));
        
        strncpy(result.sensor_id, id.c_str(), sizeof(result.sensor_id) - 1);
        result.temperature = temp;
        result.timestamp = ts;
        
        return result;
    }
};

// 获取当前时间戳
inline long get_timestamp() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

// QoS策略（服务质量）
enum class ReliabilityQoS {
    BEST_EFFORT,    // 尽力而为，允许丢失
    RELIABLE        // 可靠传输
};

enum class DurabilityQoS {
    VOLATILE,       // 易失性，不保存历史数据
    TRANSIENT_LOCAL // 暂态本地，保存最近的数据
};

struct QoSPolicy {
    ReliabilityQoS reliability;
    DurabilityQoS durability;
    int history_depth;  // 历史数据深度
    
    QoSPolicy() 
        : reliability(ReliabilityQoS::BEST_EFFORT),
          durability(DurabilityQoS::VOLATILE),
          history_depth(1) {}
};

#endif // DDS_COMMON_H

