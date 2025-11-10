#! usr/bin/env python3
"""
输入m和n, 计算C(m,n)的值

Version: 1.0
Author: 五毛仔
"""
m = int(input('m = '))
n = int(input('n = '))
# 计算m的阶乘
fm = 1
for num in range(1, m + 1):
    fm *= num
# 计算n的阶乘
fn = 1
for num in range(1, n + 1):
    fn *= num
# 计算m-n的阶乘
fk = 1
for num in range(1, m - n + 1):
    fk *= num
# 计算C(m,n)的值
print(fm // (fn * fk))


"""
以上代码过于重复，可以简化
"""
def fun(num):
    result = 1
    for n in range(2, num + 1):
        result *= n
    return result

x = int(input('x = '))
y = int(input('y = '))
print(fun(x) // (fun(y) * fun(x - y)))  


"""
摇色子获得点数
"""
from random import randrange

#定义摇色子的函数
#函数的自变量为（参数）n表示色子的个数，默认值为3
#函数的返回值表示要n个色子一起掷出点数的总和
def roll_dice(n=3):
    total = 0
    for _ in range(n):
        total += randrange(1, 7)
    return total

"""
函数实战应用
"""

"""
例子1 -- 随机验证码
"""
import random
import string

ALL_CHARS = string.digits + string.ascii_letters

def generate_code(*, code_lent =4):
    """
    生成制定长度的验证码
    :param code_lent: 验证码的长度(默认4字符)
    :return: 由大小写英文字符和数字构成的随机验证码字符串
    """
    return ''.join(random.choices(ALL_CHARS, k=code_lent))

"""
使用下面5组随机验证码测试以上函数
"""
for _ in range(5):
    print(generate_code())


"""
例子2 -- 判断素数
"""
def is_prime(num: int) -> bool:
    """
    判断一个正整数是否为素数
    :param num: 大于1的正整数
    :return: 如果num是素数返回True, 否则返回False
    """
    for i in range(2, int(num ** 0.5) + 1):
        if num % i == 0:
            return False
    return True


"""
例子3 -- 最大公约数和最小公倍数
"""

"""
例子4 -- 求和多参数
"""
def calc(*args, **kwargs):
    items = list(args) + list(kwargs.values())
    result = 0
    for item in items:
        if type(item) in (int, float):
            result += item
    return result


"""
装饰器 -- 用一个函数装饰另一个函数并为其提供额外的能力
"""
import time
import random

def record_time(func):

    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f'{func.__name__}执行时间：{end - start:.2f}秒')
        return result
    
    return wrapper

@record_time # 用@record_time装饰download函数，使其执行时间被记录
def download(filename):
    print(f'开始现在{filename}.')
    time.sleep(random.random() *6)
    print(f'{filename}下载完成.')

@record_time # 用@record_time装饰upload函数，使其执行时间被记录
def upload(filename):
    print(f'开始上传{filename}.')
    time.sleep(random.random() *6)
    print(f'{filename}上传完成.')

download('MySQL从删库到跑路.avi')
upload('Python从入门到住院.pdf')


"""
functools.wraps(func)去掉装饰器的作用执行原函数
"""
import random
import time

from functools import wraps

def record_time(func):

    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f'{func.__name__}执行时间：{end - start:.2f}秒')
        return result
    
    return wrapper

@record_time # 用@record_time装饰download函数，使其执行时间被记录
def download(filename):
    print(f'开始现在{filename}.')
    time.sleep(random.random() *6)
    print(f'{filename}下载完成.')

@record_time # 用@record_time装饰upload函数，使其执行时间被记录
def upload(filename):
    print(f'开始上传{filename}.')
    time.sleep(random.random() *6)
    print(f'{filename}上传完成.')

# 调用修饰后的函数会记录执行时间
download('MySQL从删库到跑路.avi')
upload('Python从入门到住院.pdf')
# 取消装饰器的作用不会记录时间
download.__wrapped__('MySQL从删库到跑路.avi')
upload.__wrapped__('Python从入门到住院.pdf')
