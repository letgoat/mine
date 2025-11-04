// 网站配置文件
const siteConfig = {
    // 文档分类配置
    categories: {
        home: {
            name: '首页'
        },
        career: {
            name: 'C++语法',
            sections: [
                {
                    title: '基础语法',
                    docs: [
                        { title: '数据类型与变量', file: 'career/data-types.md' },
                        { title: '指针与引用', file: 'career/pointer-reference.md' },
                        { title: '函数与重载', file: 'career/functions.md' }
                    ]
                },
                {
                    title: '面向对象',
                    docs: [
                        { title: '类与对象', file: 'career/class-object.md' },
                        { title: '继承与多态', file: 'career/inheritance.md' },
                        { title: '运算符重载', file: 'career/operator-overload.md' }
                    ]
                },
                {
                    title: '现代C++',
                    docs: [
                        { title: 'C++11新特性', file: 'career/cpp11.md' },
                        { title: '智能指针', file: 'career/smart-pointer.md' },
                        { title: 'STL容器', file: 'career/stl.md' }
                    ]
                }
            ]
        },
        cpp: {
            name: '操作系统',
            sections: [
                {
                    title: '操作系统基础',
                    docs: [
                        { title: '操作系统概述', file: 'cpp/os-intro.md' },
                        { title: '进程管理', file: 'cpp/process.md' },
                        { title: '线程管理', file: 'cpp/thread.md' }
                    ]
                },
                {
                    title: '内存管理',
                    docs: [
                        { title: '内存分配', file: 'cpp/memory-allocation.md' },
                        { title: '虚拟内存', file: 'cpp/virtual-memory.md' },
                        { title: '页面置换', file: 'cpp/page-replacement.md' }
                    ]
                },
                {
                    title: '文件系统',
                    docs: [
                        { title: '文件系统原理', file: 'cpp/filesystem.md' },
                        { title: 'IO管理', file: 'cpp/io-management.md' }
                    ]
                }
            ]
        },
        project: {
            name: '计算机网络',
            sections: [
                {
                    title: '网络基础',
                    docs: [
                        { title: '网络协议概述', file: 'project/network-intro.md' },
                        { title: 'OSI七层模型', file: 'project/osi-model.md' },
                        { title: 'TCP/IP协议族', file: 'project/tcp-ip.md' }
                    ]
                },
                {
                    title: '传输层协议',
                    docs: [
                        { title: 'TCP协议详解', file: 'project/tcp.md' },
                        { title: 'UDP协议详解', file: 'project/udp.md' },
                        { title: '流量控制与拥塞控制', file: 'project/flow-control.md' }
                    ]
                },
                {
                    title: '应用层协议',
                    docs: [
                        { title: 'HTTP协议', file: 'project/http.md' },
                        { title: 'HTTPS与SSL', file: 'project/https.md' },
                        { title: 'DNS解析', file: 'project/dns.md' }
                    ]
                }
            ]
        },
        communication: {
            name: '分布式理论',
            sections: [
                {
                    title: '分布式基础',
                    docs: [
                        { title: '分布式系统概述', file: 'communication/distributed-intro.md' },
                        { title: 'CAP理论', file: 'communication/cap.md' },
                        { title: 'BASE理论', file: 'communication/base.md' }
                    ]
                },
                {
                    title: '一致性算法',
                    docs: [
                        { title: 'Paxos算法', file: 'communication/paxos.md' },
                        { title: 'Raft算法', file: 'communication/raft.md' },
                        { title: '两阶段提交', file: 'communication/2pc.md' }
                    ]
                },
                {
                    title: '分布式架构',
                    docs: [
                        { title: 'CS架构详解', file: 'communication/cs-architecture.md' },
                        { title: 'DDS架构详解', file: 'communication/dds-architecture.md' },
                        { title: '微服务架构', file: 'communication/microservices.md' }
                    ]
                }
            ]
        },
        distributed: {
            name: '网络编程',
            sections: [
                {
                    title: 'Socket编程',
                    docs: [
                        { title: 'Socket基础', file: 'distributed/socket-basics.md' },
                        { title: 'TCP Socket编程', file: 'distributed/tcp-socket.md' },
                        { title: 'UDP Socket编程', file: 'distributed/udp-socket.md' }
                    ]
                },
                {
                    title: 'IO模型',
                    docs: [
                        { title: '阻塞IO与非阻塞IO', file: 'distributed/blocking-io.md' },
                        { title: 'IO多路复用', file: 'distributed/io-multiplexing.md' },
                        { title: 'epoll详解', file: 'distributed/epoll.md' }
                    ]
                },
                {
                    title: '高级编程',
                    docs: [
                        { title: 'Reactor模式', file: 'distributed/reactor.md' },
                        { title: 'Proactor模式', file: 'distributed/proactor.md' },
                        { title: '网络库实现', file: 'distributed/network-lib.md' }
                    ]
                }
            ]
        },
        golang: {
            name: '驱动编程',
            sections: [
                {
                    title: '驱动基础',
                    docs: [
                        { title: '驱动开发概述', file: 'golang/driver-intro.md' },
                        { title: '字符设备驱动', file: 'golang/char-device.md' },
                        { title: '块设备驱动', file: 'golang/block-device.md' }
                    ]
                },
                {
                    title: '内核模块',
                    docs: [
                        { title: '内核模块开发', file: 'golang/kernel-module.md' },
                        { title: '内核API', file: 'golang/kernel-api.md' },
                        { title: '设备树', file: 'golang/device-tree.md' }
                    ]
                },
                {
                    title: '驱动实战',
                    docs: [
                        { title: 'GPIO驱动', file: 'golang/gpio-driver.md' },
                        { title: 'I2C驱动', file: 'golang/i2c-driver.md' },
                        { title: 'SPI驱动', file: 'golang/spi-driver.md' }
                    ]
                }
            ]
        },
        linux: {
            name: 'ROS2机器人',
            sections: [
                {
                    title: 'ROS2基础',
                    docs: [
                        { title: 'ROS2简介', file: 'linux/ros2-intro.md' },
                        { title: 'ROS2安装与配置', file: 'linux/ros2-setup.md' },
                        { title: 'ROS2工作空间', file: 'linux/ros2-workspace.md' }
                    ]
                },
                {
                    title: 'ROS2核心概念',
                    docs: [
                        { title: '节点与话题', file: 'linux/ros2-nodes-topics.md' },
                        { title: '服务与参数', file: 'linux/ros2-services.md' },
                        { title: 'DDS通信机制', file: 'linux/ros2-dds.md' }
                    ]
                },
                {
                    title: '机器人应用',
                    docs: [
                        { title: '传感器接入', file: 'linux/ros2-sensors.md' },
                        { title: '运动控制', file: 'linux/ros2-motion.md' },
                        { title: '导航与定位', file: 'linux/ros2-navigation.md' }
                    ]
                }
            ]
        }
    }
};

