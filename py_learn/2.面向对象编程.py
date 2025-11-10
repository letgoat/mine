#! usr/bin/env python3

"""
例子1 -- 平面上的点
要求：定义一个类描述平面上的点，提供计算到另一个点距离的方法
"""
class Point:
    """平面上的点"""

    def __init__(self, x=0, y=0):
        """初始化方法
        :param x:横坐标
        :param y:纵坐标
        """
        self.x, self.y = x, y

    def distance_to(self, other):
        """计算与另一个点的距离
        :param other: 另一个点
        :return: 两点之间的距离
        """
        dx = self.x - other.x
        dy = self.y - other.y
        return (dx * dx +dy * dy) ** 0.5

    def __str__(self):
        return f'({self.x}, {self.y})'

p1 = Point(3, 5)
p2 = Point(6, 9)
print(p1)
print(p2)
print(p1.distance_to(p2))


"""
python中对象可见性__name表示私有属性，_name表示受保护的属性，name表示公共属性
python是动态语言，可以为对象动态添加属性和方法
如果要限制对象动态添加属性，可以为对象添加__slots__属性
"""

class Student:
    __slots__ = ('name', 'age')

    def __init__(self, name, age):
        self.name = name
        self.age = age

stu =Student('五毛仔', 24)
# stu.sex = '男' #报错，因为Student类中没有sex属性


"""
继承和多态
super()函数是专门为获取当前对象的父类对象而设计的
"""


"""
例子 -- 工资结算系统
要求：某公司有三种类型的员工，分别是部门经理、程序员和销售员。
需要设计一个工资结算系统，根据提供的员工信息来计算员工的月薪。
其中，部门经理的月薪是固定 15000 元；程序员按工作时间（以小时为单位）支付月薪，每小时 200 元；
销售员的月薪由 1800 元底薪加上销售额 5% 的提成两部分构成。
"""

from abc import ABCMeta, abstractmethod

class Employee(metaclass=ABCMeta):
    """ 员工 """

    def __init__(self, name):
        self.name = name
    
    @abstractmethod
    def get_salary(self):
        """ 结算月薪 """ 
        pass

class Manager(Employee):
    """ 部门经理 """

    def get_salary(self):
        return 15000

class Programmer(Employee):
    """ 程序员 """

    def __init__(self, name, working_hour=0):
        super().__init__(name)
        self.working_hour = working_hour

    def get_salary(self):
        return 200 * self.working_hour

class Salesman(Employee):
    """ 销售员 """

    def __init__(self, name, sales=0):
        super().__init__(name)
        self.sales = sales

    def get_salary(self):
        return 1800 + self.sales * 0.05
