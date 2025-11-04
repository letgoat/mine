# C++指针与引用

## 指针基础

### 什么是指针？

指针是存储内存地址的变量。

\`\`\`cpp
int value = 42;
int* ptr = &value;  // ptr存储value的地址

std::cout << value << std::endl;   // 输出: 42
std::cout << &value << std::endl;  // 输出: 0x7ffc...（地址）
std::cout << ptr << std::endl;     // 输出: 0x7ffc...（地址）
std::cout << *ptr << std::endl;    // 输出: 42（解引用）
\`\`\`

### 指针操作符

\`\`\`cpp
int x = 10;
int* p = &x;   // & 取地址运算符
int y = *p;    // * 解引用运算符
\`\`\`

## 引用

### 引用与指针的区别

\`\`\`cpp
int value = 100;

// 指针
int* ptr = &value;
*ptr = 200;  // 需要解引用

// 引用
int& ref = value;
ref = 200;   // 直接使用
\`\`\`

### 对比表

| 特性 | 指针 | 引用 |
|------|------|------|
| 可否为空 | 可以（nullptr） | 不可以 |
| 可否重新赋值 | 可以 | 不可以 |
| 语法 | 需要 * 解引用 | 直接使用 |

## 常见用法

### 函数参数

\`\`\`cpp
// 传值
void func1(int x) {
    x = 100;  // 不影响原变量
}

// 传指针
void func2(int* x) {
    *x = 100;  // 修改原变量
}

// 传引用
void func3(int& x) {
    x = 100;  // 修改原变量
}
\`\`\`

---

*更多内容待补充...*

