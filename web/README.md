# 五毛仔学技术 - 技术文档站

一个简洁优雅的个人技术文档网站，用于记录和分享技术学习笔记。

## ✨ 特性

- 📱 **响应式设计** - 完美适配桌面端和移动端
- 🎨 **现代化UI** - 简洁美观的界面设计，橙色主题
- 📝 **Markdown支持** - 使用Markdown编写文档
- 🔍 **实时搜索** - 快速查找文档内容
- 🎯 **代码高亮** - 支持多种编程语言语法高亮
- 🗂️ **分类导航** - 多级目录结构，清晰易用
- ⚡ **快速加载** - 纯静态页面，无需后端
- 🖱️ **可调整布局** - 侧边栏宽度可拖动调整

## 🚀 快速开始

### 1. 启动网站

使用提供的启动脚本（推荐）：

```bash
cd web
./start.sh
```

或者使用Python直接启动：

```bash
cd web
python3 -m http.server 8000
```

### 2. 访问网站

打开浏览器访问：`http://localhost:8000`

### 3. 清除缓存

如果布局显示异常，访问：`http://localhost:8000/clear-cache.html`

## 📂 项目结构

```
web/
├── index.html          # 主页面
├── clear-cache.html    # 清除缓存页面
├── start.sh           # 启动脚本
├── css/
│   └── style.css      # 样式文件（全新 Flexbox 布局）
├── js/
│   ├── config.js      # 配置文件（文档结构）
│   └── app.js         # 核心逻辑
├── docs/              # 文档目录
│   ├── home/          # 首页文档
│   ├── career/        # C++语法
│   ├── cpp/           # 操作系统
│   ├── project/       # 计算机网络
│   ├── communication/ # 分布式理论
│   ├── distributed/   # 网络编程
│   ├── golang/        # 驱动编程
│   └── linux/         # ROS2机器人
├── assets/
│   └── images/        # 图片资源
└── README.md          # 本文件
```

## 📚 文档分类

网站包含以下技术专栏：

### 1. **C++语法**
- 基础语法：数据类型、指针引用、函数重载
- 面向对象：类与对象、继承多态、运算符重载
- 现代C++：C++11新特性、智能指针、STL容器

### 2. **操作系统**
- 操作系统基础：进程管理、线程管理
- 内存管理：内存分配、虚拟内存、页面置换
- 文件系统：文件系统原理、IO管理

### 3. **计算机网络**
- 网络基础：OSI七层模型、TCP/IP协议族
- 传输层协议：TCP详解、UDP详解、流量控制
- 应用层协议：HTTP、HTTPS、DNS

### 4. **分布式理论**
- 分布式基础：CAP理论、BASE理论
- 一致性算法：Paxos、Raft、两阶段提交
- 分布式架构：CS架构、DDS架构、微服务

### 5. **网络编程**
- Socket编程：TCP/UDP Socket编程
- IO模型：阻塞IO、非阻塞IO、IO多路复用
- 高级编程：Reactor模式、Proactor模式

### 6. **驱动编程**
- 驱动基础：字符设备、块设备
- 内核模块：内核模块开发、内核API、设备树
- 驱动实战：GPIO、I2C、SPI驱动

### 7. **ROS2机器人**
- ROS2基础：安装配置、工作空间
- 核心概念：节点话题、服务参数、DDS通信
- 机器人应用：传感器接入、运动控制、导航定位

## ✍️ 编写文档

### 1. 创建文档文件

在对应分类的 `docs/` 子目录下创建Markdown文件：

```bash
# 例如：创建一个C++智能指针的文档
mkdir -p docs/career
touch docs/career/smart-pointer.md
```

### 2. 编辑配置文件

在 `js/config.js` 中已经配置好了文档结构，只需要创建对应的 `.md` 文件即可。

### 3. 编写Markdown内容

使用标准Markdown语法编写文档：

```markdown
# C++智能指针

## unique_ptr

\`\`\`cpp
#include <memory>

std::unique_ptr<int> ptr = std::make_unique<int>(42);
\`\`\`

## shared_ptr

\`\`\`cpp
std::shared_ptr<int> ptr1 = std::make_shared<int>(100);
std::shared_ptr<int> ptr2 = ptr1;  // 引用计数+1
\`\`\`
```

## 🎨 自定义配置

### 修改主题色

编辑 `css/style.css`：

```css
:root {
    --primary-color: #ff7700;  /* 橙色主题 */
    --hover-color: #ff9933;
}
```

### 调整布局

- **左侧边栏**：默认 200px，可拖动调整（160px - 320px）
- **右侧边栏**：默认 160px，可拖动调整（140px - 200px）
- **主内容区**：自动填充剩余空间

拖动侧边栏右侧的细线即可调整宽度，宽度会自动保存。

## 🔧 技术栈

- **HTML5** - 页面结构
- **CSS3** - Flexbox 布局
- **JavaScript (ES6+)** - 交互逻辑
- **Marked.js** - Markdown 解析和渲染
- **Highlight.js** - 代码语法高亮（支持C++）

## 📱 响应式设计

- **桌面端** (> 1200px)：三栏布局（左侧栏 + 内容 + 右侧栏）
- **平板端** (768px - 1200px)：两栏布局（左侧栏 + 内容）
- **移动端** (< 768px)：单栏布局，侧边栏可切换

## 🌟 功能特色

### 1. 三栏智能导航
- **左侧栏**：文档目录树形导航
- **中间栏**：Markdown文档内容展示
- **右侧栏**：页面内标题导航（自动提取h2-h4）

### 2. 拖动调整布局
- 鼠标悬停在分隔线上会高亮显示
- 拖动分隔线可调整侧边栏宽度
- 调整的宽度自动保存到浏览器本地

### 3. 实时搜索
在顶部搜索框输入关键词（支持 `Ctrl+K` 快捷键）

### 4. 代码高亮
支持的语言：
- C/C++
- JavaScript
- Python
- Shell
- 等更多...

## 🛠️ 常见问题

### 1. 布局显示异常？

访问 `http://localhost:8000/clear-cache.html` 清除缓存

或在浏览器控制台（F12）执行：
```javascript
localStorage.clear(); location.reload();
```

### 2. 如何修改网站名称？

1. 修改 `index.html` 中的 `<title>` 和 `.logo-text`
2. 修改 `index.html` 中的欢迎页面文字

### 3. 如何添加新的文档分类？

1. 在 `js/config.js` 中添加新分类配置
2. 在 `index.html` 的导航栏添加对应链接
3. 在 `docs/` 目录下创建对应文件夹

## 💡 使用建议

1. **定期更新** - 养成记录技术笔记的习惯
2. **分类清晰** - 合理组织文档结构
3. **代码示例** - 多写代码示例便于理解
4. **图文并茂** - 适当添加图片增强说明

## 📄 许可

MIT License - 自由使用和修改

---

**开始编写你的技术文档吧！** 📖✨

如有问题，欢迎关注微信公众号反馈。
