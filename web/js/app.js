// 主应用逻辑
class TechDocs {
    constructor() {
        this.currentCategory = 'home';
        this.currentDoc = '';
        this.init();
    }

    init() {
        this.setupEventListeners();
        this.setupMarkdown();
        this.setupResizers();
        // 初始显示欢迎页面
        this.switchCategory('home');
    }

    setupMarkdown() {
        // 配置 marked
        if (typeof marked !== 'undefined') {
            marked.setOptions({
                highlight: function(code, lang) {
                    if (typeof hljs !== 'undefined' && lang && hljs.getLanguage(lang)) {
                        try {
                            return hljs.highlight(code, { language: lang }).value;
                        } catch (err) {}
                    }
                    return code;
                },
                breaks: true,
                gfm: true
            });
        }
    }

    setupEventListeners() {
        // 顶部导航切换
        document.querySelectorAll('.nav-item').forEach(item => {
            item.addEventListener('click', (e) => {
                e.preventDefault();
                const category = e.target.dataset.category;
                this.switchCategory(category);
            });
        });

        // Logo点击返回首页
        const logo = document.querySelector('.logo');
        if (logo) {
            logo.addEventListener('click', () => {
                this.switchCategory('home');
            });
        }

        // 移动端菜单
        const mobileMenuBtn = document.getElementById('mobileMenuBtn');
        const leftSidebar = document.getElementById('leftSidebar');
        
        if (mobileMenuBtn) {
            mobileMenuBtn.addEventListener('click', () => {
                leftSidebar.classList.toggle('active');
            });
        }

        // 点击内容区关闭侧边栏（移动端）
        const contentArea = document.getElementById('contentArea');
        if (contentArea) {
            contentArea.addEventListener('click', () => {
                if (window.innerWidth <= 768) {
                    leftSidebar.classList.remove('active');
                }
            });
        }

        // 搜索功能
        const searchInput = document.getElementById('searchInput');
        if (searchInput) {
            searchInput.addEventListener('input', (e) => {
                this.handleSearch(e.target.value);
            });
        }
    }

    switchCategory(category) {
        this.currentCategory = category;
        
        // 更新顶部导航激活状态
        document.querySelectorAll('.nav-item').forEach(item => {
            item.classList.remove('active');
            if (item.dataset.category === category) {
                item.classList.add('active');
            }
        });

        // 切换显示模式
        const welcomePage = document.getElementById('welcomePage');
        const docMode = document.getElementById('docMode');
        const footer = document.getElementById('footer');
        
        if (category === 'home') {
            // 显示欢迎页面
            welcomePage.style.display = 'flex';
            docMode.style.display = 'none';
            footer.style.display = 'block';
        } else {
            // 显示文档模式
            welcomePage.style.display = 'none';
            docMode.style.display = 'flex';
            footer.style.display = 'none';
            
            // 加载分类内容
            this.loadCategory(category);
        }
    }

    loadCategory(category) {
        const config = siteConfig.categories[category];
        if (!config) return;

        // 渲染左侧导航
        this.renderDocNav(config);

        // 加载第一个文档
        if (config.sections && config.sections[0] && config.sections[0].docs[0]) {
            this.loadDoc(config.sections[0].docs[0].file);
        } else if (config.docs && config.docs[0]) {
            this.loadDoc(config.docs[0].file);
        }
    }

    renderDocNav(config) {
        const docNav = document.getElementById('docNav');
        docNav.innerHTML = '';

        if (config.docs) {
            // 简单的文档列表
            config.docs.forEach(doc => {
                const link = document.createElement('a');
                link.href = '#';
                link.className = 'doc-link';
                link.textContent = doc.title;
                link.addEventListener('click', (e) => {
                    e.preventDefault();
                    this.loadDoc(doc.file);
                });
                docNav.appendChild(link);
            });
        } else if (config.sections) {
            // 分节的文档列表
            config.sections.forEach(section => {
                const sectionDiv = document.createElement('div');
                sectionDiv.className = 'doc-section';

                const sectionTitle = document.createElement('div');
                sectionTitle.className = 'section-title';
                sectionTitle.textContent = section.title;
                sectionDiv.appendChild(sectionTitle);

                section.docs.forEach(doc => {
                    const link = document.createElement('a');
                    link.href = '#';
                    link.className = 'doc-link';
                    link.textContent = doc.title;
                    link.dataset.file = doc.file;
                    link.addEventListener('click', (e) => {
                        e.preventDefault();
                        this.loadDoc(doc.file);
                    });
                    sectionDiv.appendChild(link);
                });

                docNav.appendChild(sectionDiv);
            });
        }
    }

    async loadDoc(filePath) {
        this.currentDoc = filePath;
        
        // 更新左侧导航激活状态
        document.querySelectorAll('.doc-link').forEach(link => {
            link.classList.remove('active');
            if (link.dataset.file === filePath) {
                link.classList.add('active');
            }
        });

        try {
            const response = await fetch(`docs/${filePath}`);
            if (!response.ok) {
                throw new Error('文档加载失败');
            }
            
            const markdown = await response.text();
            this.renderMarkdown(markdown);
        } catch (error) {
            console.error('加载文档失败:', error);
            this.renderMarkdown(this.getDefaultContent(filePath));
        }
    }

    renderMarkdown(markdown) {
        const contentArea = document.getElementById('markdownContent');
        
        if (typeof marked !== 'undefined') {
            contentArea.innerHTML = marked.parse(markdown);
            
            // 代码高亮
            if (typeof hljs !== 'undefined') {
                contentArea.querySelectorAll('pre code').forEach((block) => {
                    hljs.highlightElement(block);
                });
            }
            
            // 生成右侧导航
            this.generatePageNav();
            
            // 滚动到顶部
            document.getElementById('contentArea').scrollTop = 0;
        } else {
            contentArea.innerHTML = `<pre>${markdown}</pre>`;
        }
    }

    generatePageNav() {
        const pageNav = document.getElementById('pageNav');
        if (!pageNav) return;

        pageNav.innerHTML = '';
        
        const headings = document.querySelectorAll('.markdown-content h2, .markdown-content h3, .markdown-content h4');
        
        headings.forEach((heading, index) => {
            const id = `heading-${index}`;
            heading.id = id;
            
            const link = document.createElement('a');
            link.href = `#${id}`;
            link.textContent = heading.textContent;
            link.className = `level-${heading.tagName.toLowerCase().charAt(1)}`;
            
            link.addEventListener('click', (e) => {
                e.preventDefault();
                heading.scrollIntoView({ behavior: 'smooth' });
                
                // 更新激活状态
                document.querySelectorAll('.page-nav a').forEach(a => a.classList.remove('active'));
                link.classList.add('active');
            });
            
            pageNav.appendChild(link);
        });
    }

    handleSearch(query) {
        if (!query) {
            // 重置显示
            document.querySelectorAll('.doc-link').forEach(link => {
                link.style.display = 'block';
            });
            return;
        }

        // 简单的搜索过滤
        const lowerQuery = query.toLowerCase();
        document.querySelectorAll('.doc-link').forEach(link => {
            const text = link.textContent.toLowerCase();
            if (text.includes(lowerQuery)) {
                link.style.display = 'block';
            } else {
                link.style.display = 'none';
            }
        });
    }

    getDefaultContent(filePath) {
        return `# ${filePath}

## 文档创建提示

这个文档还没有创建。请在 \`docs/${filePath}\` 路径下创建 Markdown 文件。

### Markdown 语法示例

#### 标题
使用 \`#\` 创建标题

#### 代码块
\`\`\`cpp
#include <iostream>

int main() {
    std::cout << "Hello World!" << std::endl;
    return 0;
}
\`\`\`

#### 列表
- 项目1
- 项目2
- 项目3

#### 表格
| 列1 | 列2 | 列3 |
|-----|-----|-----|
| 数据1 | 数据2 | 数据3 |

### 开始编写
在项目的 \`docs\` 目录下创建对应的 Markdown 文件即可！
`;
    }

    setupResizers() {
        const leftResizer = document.getElementById('leftResizer');
        const rightResizer = document.getElementById('rightResizer');
        const leftSidebar = document.getElementById('leftSidebar');
        const rightSidebar = document.getElementById('rightSidebar');

        // 加载保存的宽度
        this.loadSidebarWidths();

        // 左侧分隔条拖动
        if (leftResizer && leftSidebar) {
            this.initResizer(leftResizer, leftSidebar, 'left');
        }

        // 右侧分隔条拖动
        if (rightResizer && rightSidebar) {
            this.initResizer(rightResizer, rightSidebar, 'right');
        }
    }

    initResizer(resizer, sidebar, side) {
        let isResizing = false;
        let startX = 0;
        let startWidth = 0;

        // 设置宽度限制 - 更严格的限制
        const minWidth = side === 'left' ? 160 : 140;
        const maxWidth = side === 'left' ? 320 : 200;

        resizer.addEventListener('mousedown', (e) => {
            isResizing = true;
            startX = e.clientX;
            startWidth = sidebar.offsetWidth;

            resizer.classList.add('active');
            document.body.classList.add('resizing');

            e.preventDefault();
        });

        document.addEventListener('mousemove', (e) => {
            if (!isResizing) return;

            const deltaX = e.clientX - startX;
            let newWidth;

            if (side === 'left') {
                newWidth = startWidth + deltaX;
            } else {
                newWidth = startWidth - deltaX;
            }

            // 限制宽度范围
            newWidth = Math.max(minWidth, Math.min(maxWidth, newWidth));

            sidebar.style.width = `${newWidth}px`;
        });

        document.addEventListener('mouseup', () => {
            if (isResizing) {
                isResizing = false;
                resizer.classList.remove('active');
                document.body.classList.remove('resizing');

                // 保存宽度到 localStorage
                this.saveSidebarWidths();
            }
        });
    }

    saveSidebarWidths() {
        const leftSidebar = document.getElementById('leftSidebar');
        const rightSidebar = document.getElementById('rightSidebar');

        if (leftSidebar) {
            localStorage.setItem('leftSidebarWidth', leftSidebar.offsetWidth);
        }
        if (rightSidebar) {
            localStorage.setItem('rightSidebarWidth', rightSidebar.offsetWidth);
        }
    }

    loadSidebarWidths() {
        const leftSidebar = document.getElementById('leftSidebar');
        const rightSidebar = document.getElementById('rightSidebar');

        // 默认值和范围
        const leftDefault = 200;
        const rightDefault = 160;
        const leftMin = 160, leftMax = 320;
        const rightMin = 140, rightMax = 200;

        // 加载并验证左侧边栏宽度
        if (leftSidebar) {
            const savedLeft = localStorage.getItem('leftSidebarWidth');
            if (savedLeft) {
                const width = parseInt(savedLeft);
                if (width >= leftMin && width <= leftMax) {
                    leftSidebar.style.width = `${width}px`;
                } else {
                    leftSidebar.style.width = `${leftDefault}px`;
                    localStorage.setItem('leftSidebarWidth', leftDefault);
                }
            }
        }

        // 加载并验证右侧边栏宽度
        if (rightSidebar) {
            const savedRight = localStorage.getItem('rightSidebarWidth');
            if (savedRight) {
                const width = parseInt(savedRight);
                if (width >= rightMin && width <= rightMax) {
                    rightSidebar.style.width = `${width}px`;
                } else {
                    rightSidebar.style.width = `${rightDefault}px`;
                    localStorage.setItem('rightSidebarWidth', rightDefault);
                }
            }
        }
    }
}

// 页面加载完成后初始化
document.addEventListener('DOMContentLoaded', () => {
    window.techDocs = new TechDocs();
});

