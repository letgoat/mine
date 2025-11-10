#! /usr/bin/env python3

file = None
try:
    file = open('致橡树.txt', 'r', encoding='utf-8')
    print(file.read())
except FileNotFoundError:
    print('无法打开指定的文件')
except LookupError:
    print('指定了未知的编码')
except UnicodeDecodeError:
    print('读取文件时解码错误')
finally:
    if file:
        file.close()


try:
    with open('csgo.png', 'rb') as file1:
        data = file1.read()
    with open('copy.png', 'wb') as file2:
        file2.write(data)
except FileNotFoundError:
    print('指定的文件无法打开')
except IOError:
    print('读写文件时出错')
print('程序执行结束')


""" 对象的序列化和反序列化 """
""" json模块中存在4个重要函数：
    dump: 将python对象按照json格式序列化到文件中
    dumps: 将python对象处理成json格式的字符串
    load: 将文件中的json数据反序列化成对象
    loads: 将json格式的字符串反序列化成python对象
"""
""" json模块中的dumps可以将字典处理成json格式（以字符串形式存在）,dump将字典处理成json格式并写入文件"""
import json

my_dict = {
    'name': '五毛仔',
    'age': 24,
    'friends': ['小明', '小红', '小刚'],
    'cars':[
        {'brand':'BMW', 'max_speed':240},
        {'brand':'Audi', 'max_speed':280},
    ]
}
with open('data.json', 'w', encoding='utf-8') as file:
    json.dump(my_dict, file)


""" python读写CSV文件 """
""" CSV文件有以下特征：
    1. 纯文本
    2. 由一条条的记录组成(典型的是每行一条记录)
    3. 每条记录被分隔符(如逗号，分号，制表符等)分隔成字段
    4. 每条记录都有相同的字段序列
"""
import csv
import random

""" 将数据写入CSV文件，csv模块中的writer对象提供了writerow方法，可以写入一行数据"""
with open('scores.cvs', 'w') as file:
    writer = csv.writer(file)
    writer.writerow(['姓名', '语文', '数学', '英语'])
    names = ['关羽', '张飞', '赵云', '马超', '黄忠']
    for name in names:
        scores = [random.randrange(50, 101) for _ in range(3)]
        scores.insert(0, name)
        writer.writerow(scores)

""" 从CSV文件中读取数据，csv模块中的reader函数可以创建处一个csvreader对象，
该对象是一个迭代器，可以通过next或者for-in循环读出数据"""
import csv
with open('scores.cvs', 'r') as file:
    reader = csv.reader(file, delimiter='|')
    for data_list in reader:
        print(reader.line_num, end='\t')
        for elem in data_list:
            print(elem, end='\t')
        print()


""" python读写Excel文件 """
""" python读取Excel文件 """
import xlrd
# 使用xlrd模块的open_workbook函数打开指定Excel文件并获取Book对象(工作簿)
wb = xlrd.open_workbook('阿里巴巴2020年股票数据.xls')
# 通过Book对象的sheet_names方法获取所有表单名称
sheetnames = wb.sheet_names()
print(sheetnames)
# 通过指定的表单名称获取sheet对象(工作表)
sheet = wb.sheet_by_name(sheetnames[0])
# 通过sheet对象的nrows和ncols属性获取表单的行数和列数
print(sheet.nrows, sheet.ncols)
for row in range(sheet.nrows):
    for col in range(sheet.ncols):
        # 通过sheet对象的cell方法获取指定cell对象
        # 通过cell对象的value属性获取单元格中的值
        value = sheet.cell(row, col).value
        # 对除首行外的其他行进行数据格式化处理
        if row > 0:
            # 第1列的xldate类型先转换成元组再格式化为“年 月 日”的格式
            if col == 







""" python写入Excel文件 """
import random

import xlwt

student_names = ['关羽', '张飞', '赵云', '马超', '黄忠']
scores = [[random.randrange(50, 101) for _ in range(3)] for _ in range(5)]
# 创建工作簿对象（Workbook）
wb = xlwt.Workbook()
# 创建工作表对象（Worksheet）
sheet = wb.add_sheet('一年级二班')
# 添加表头数据
titles = ('姓名', '语文', '数学', '英语')
for index, title in enumerate(titles):
    sheet.write(0, index, title)
# 将学生姓名和考试成绩写入单元格
for row in range(len(scores)):
    sheet.write(row + 1, 0, student_names[row])
    for col in range(len(scores[row])):
        sheet.write(row + 1, col + 1, scores[row][col])
# 保存Excel工作簿
wb.save('考试成绩表.xls')
