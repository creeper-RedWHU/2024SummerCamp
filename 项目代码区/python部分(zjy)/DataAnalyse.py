import pymysql
import numpy
import matplotlib.pyplot as plt

'''
接口函数：
1.平均最高气温(ok)
2.平均最低气温(ok)
3.平均气温(ok)
4.预测：过去24小时（预测） + 根据年际数据预测今日 （显示往年数据）（先爬取历年当月数据：GetData.py中的函数）
'''


def getMaxAverage(city, startYear, startMonth, startday, endYear, endMonth, endday):
    conn = pymysql.connect(
        host='localhost',
        port=3306,
        user='root',
        password='zhoujin@MySQL',
        charset='utf8',
        database="data"
    )
    cursor = conn.cursor()
    SQLSentence = "SELECT * FROM climate where city='%s' and year=%d and month=%d"
    days = 0
    ans = 0
    for year in range(startYear, endYear + 1):
        if year == startYear:
            for month in range(startMonth, 13):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    if item[5] >= startday:
                        ans += item[0]
                        days += 1
        elif year == endYear:
            for month in range(1, endMonth + 1):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    if item[5] <= endday:
                        ans += item[0]
                        days += 1
                    else:
                        break
        else:
            for month in range(1, endMonth + 1):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    ans += item[0]
                    days += 1
    cursor.close()
    conn.close()
    return ans / days


def getMinAverage(city, startYear, startMonth, startday, endYear, endMonth, endday):
    conn = pymysql.connect(
        host='localhost',
        port=3306,
        user='root',
        password='zhoujin@MySQL',
        charset='utf8',
        database="data"
    )
    cursor = conn.cursor()
    SQLSentence = "SELECT * FROM climate where city='%s' and year=%d and month=%d"
    days = 0
    ans = 0
    for year in range(startYear, endYear + 1):
        if year == startYear:
            for month in range(startMonth, 13):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    if item[5] >= startday:
                        ans += item[1]
                        days += 1
        elif year == endYear:
            for month in range(1, endMonth + 1):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    if item[5] <= endday:
                        ans += item[1]
                        days += 1
                    else:
                        break
        else:
            for month in range(1, endMonth + 1):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    ans += item[1]
                    days += 1
    cursor.close()
    conn.close()
    return ans / days


def getAverage(city, startYear, startMonth, startday, endYear, endMonth, endday):
    conn = pymysql.connect(
        host='localhost',
        port=3306,
        user='root',
        password='zhoujin@MySQL',
        charset='utf8',
        database="data"
    )
    cursor = conn.cursor()
    SQLSentence = "SELECT * FROM climate where city='%s' and year=%d and month=%d"
    days = 0
    ans = 0
    for year in range(startYear, endYear + 1):
        if year == startYear:
            for month in range(startMonth, 13):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    if item[5] >= startday:
                        ans += (item[0] + item[1]) / 2
                        days += 1
        elif year == endYear:
            for month in range(1, endMonth + 1):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    if item[5] <= endday:
                        ans += (item[0] + item[1]) / 2
                        days += 1
                    else:
                        break
        else:
            for month in range(1, endMonth + 1):
                dev = (city, year, month)
                thisSentence = SQLSentence % dev
                cursor.execute(thisSentence)
                Result = cursor.fetchall()
                for item in Result:
                    ans += (item[0] + item[1]) / 2
                    days += 1
    cursor.close()
    conn.close()
    return ans / days

