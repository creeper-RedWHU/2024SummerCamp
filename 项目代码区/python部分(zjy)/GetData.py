import re
import string
from urllib.parse import quote
import pymysql
import numpy as np
import urllib.request, urllib.error
import json
import multiprocessing
import requests
from bs4 import BeautifulSoup
import time
import datetime
import os
import cv2
'''
数据库标准说明：
MySQL型数据库
data库中climate表存放气象数据，users表存放用户数据
climate:max_temperature INT,min_temperature INT,city TEXT,year INT,month INT,day INT,weather TEXT,wind_direction TEXT,wind_strength TEXT
users:user_name TEXT,password TEXT,identity INT
'''

'''
集成函数说明(使用c++描述)：
1.void update(string ct,int startYear,int startMonth,int endYear,int endMonth)XQt不用
    参数说明：传入参数含义依次为：城市名字，开始年，开始月，结束年，结束月。
    功能说明：实现数据库数据增加：某城市从[(startYear,startMonth)-->(endYear,endMonth)]的气象数据更新
2.void fetchData(string ct,int startYear,int startMonth,int endYear,int endMonth)※Qt用
    参数说明：传入参数含义依次为：城市名字，开始年，开始月，结束年，结束月。
    应用场景说明：每次Qt端需要查询某个界面的数据即可调用这个函数
    功能说明：检[(startYear,startMonth)-->(endYear,endMonth)]漏，如果没有就调用更新函数。
3.void GetDataByHours(string ct)XQt不用
    参数说明：城市名字
    应用场景说明：Qt端每次查询某一个城市最近24小时的数据先调用这个函数
    功能说明：爬取过去24小时的温度和空气质量，每次爬取前清除数据
    
4.void fetch12future(string ct)

'''

'''
连接数据库
conn = pymysql.connect(
    host='localhost',
    port=3306,
    user='root',
    password='zhoujin@MySQL',
    charset='utf8'
)
'''

urlFirst = "https://lishi.tianqi.com/"
urlThirdOfDate = "/"
urlFinal = ".html"
cityList = {"长沙": "changsha", "武汉": "wuhan", "北京": "beijing", "杭州": "hangzhou", "上海": "shanghai",
            "南京": "nanjing"}
findLinkDate = re.compile(r'<div class="th200">(.*?)</div>')
findDataEach = re.compile(r'<div class="th140">(.*?)</div>')
findHoursData = re.compile(r'<tr>(.*?)</tr>', re.S)
findHoursDataSec = re.compile(r'<td class="text-center">(.*?)</td>', re.S)
findHoursDataThird = re.compile(r'<td>(.*?)</td>', re.S)
dateMax = {1: 31, 2: 28, 3: 31, 4: 30, 5: 31, 6: 30, 7: 31, 8: 31, 9: 30, 10: 31, 11: 30, 12: 31, 20: 29}
cityToProvince = {"北京": "54511", "武汉": "57494", "杭州": "58457"}


def getURL(address, year, month):
    url = urlFirst
    url += cityList[address]
    url += urlThirdOfDate
    url += str(year)
    if (month < 10):
        url += '0' + str(month)
    else:
        url += str(month)
    url += urlFinal
    return url


def askURL(url):
    head = {
        "User-Agent": "Mozilla / 5.0(Windows NT 10.0; Win64; x64) AppleWebKit / 537.36(KHTML, like Gecko) Chrome / 80.0.3987.122  Safari / 537.36"
    }

    request = urllib.request.Request(url, headers=head)
    html = ""
    try:
        response = urllib.request.urlopen(request)
        html = response.read().decode("utf-8")
    except urllib.error.URLError as e:
        if hasattr(e, "code"):
            print(e.code)
        if hasattr(e, "reason"):
            print(e.reason)
    return html


def toint(s):
    if (s[0] == '0'):
        return int(s[1])
    return int(s)


def getdouble(s):
    a = 0
    for i in range(len(s)):
        if s[i] == '.':
            a += int(s[i + 1]) * 0.1
            return a
        elif s[i]!='℃':
            a = a * 10 + int(s[i])
    return a


def get_data(url, city, year, month):
    html = askURL(url)
    soup = BeautifulSoup(html, "html.parser")
    yc = 0
    while yc <= 10:
        if soup.find_all('ul', class_="thrui").__len__() != 0:
            break
        else:
            print(city, ":", year, ".", month, "的数据获取异常。正在重试，请稍等...", end='')
            print()
            time.sleep(1)
            yc += 2
            html = askURL(url)
            soup = BeautifulSoup(html, "html.parser")
    if yc >= 10:
        return False
    for item in soup.find_all('ul', class_="thrui"):
        item = str(item)
        link = re.findall(findLinkDate, item)
        datas = re.findall(findDataEach, item)

        conn = pymysql.connect(
            host='localhost',
            port=3306,
            user='root',
            password='zhoujin@MySQL',
            charset='utf8',
            database="data"
        )
        '''
        conn = pymysql.connect(
            host='60.205.232.122',
            port=3306,
            user='root',
            password='QAZ123wsx',
            charset='utf8',
            database="data"
        )'''
        cursor = conn.cursor()
        sql = "INSERT INTO climate(max_temperature, min_temperature,city,year,month,day,weather,wind_direction,wind_strength) VALUES (%s, %s,\'%s\',%s,%s,%s,\'%s\',\'%s\',\'%s\')"
        dats = []
        for i in range(len(link)):
            datum = []
            datum.append(link[i])
            for j in range(4):
                datum.append(datas[i * 4 + j])
            eachDatum = (int(datum[1][:len(datum[1]) - 1]), int(datum[2][:len(datum[2]) - 1]), city, year, month,
                         toint(datum[0][8:10]), datum[3], datum[4].split()[0], datum[4].split()[1])
            dats.append(eachDatum)
            request = sql % eachDatum
            cursor.execute(request)
            conn.commit()
        cursor.close()
        conn.close()
    return True


# 提供接口：update()，输入：城市，起始日期，终止日期。

def update(ct, startYear, endYear, startMonth, endMonth):
    for year in range(startYear, endYear + 1):
        if year == startYear:
            for month in range(startMonth, 13):
                get_data(getURL(ct, year, month), ct, year, month)
        elif year == endYear:
            for month in range(1, endMonth + 1):
                get_data(getURL(ct, year, month), ct, year, month)
        else:
            for month in range(1, 13):
                get_data(getURL(ct, year, month), ct, year, month)


def whetherhas(ct, year, month):

    conn = pymysql.connect(
        host='localhost',
        port=3306,
        user='root',
        password='zhoujin@MySQL',
        charset='utf8',
        database="data"
    )
    '''
    conn = pymysql.connect(
        host='60.205.232.122',
        port=3306,
        user='root',
        password='QAZ123wsx',
        charset='utf8',
        database="data"
    )'''
    cursor = conn.cursor()
    ans = False
    SQLSentence = "select * from climate where city='%s' and year= %d and month=%d"
    dev = (ct, year, month)
    SQLSentence %= dev
    cursor.execute(SQLSentence)
    myResult = cursor.fetchall()
    if myResult.__len__() > 0:
        ans = True
    cursor.close()
    conn.close()
    return ans


def fetchData(ct, startYear, endYear, startMonth, endMonth):
    if startYear == endYear:
        year = startYear
        for month in range(startMonth, endMonth + 1):
            a = True
            if not whetherhas(ct, year, month):
                a = get_data(getURL(ct, year, month), ct, year, month)
            if a:
                print(ct, ' ', year, month, "ok")
                time.sleep(1)
        return 0
    for year in range(startYear, endYear + 1):
        if year == startYear:
            for month in range(startMonth, 13):
                a = True
                if whetherhas(ct, year, month) == False:
                    a = get_data(getURL(ct, year, month), ct, year, month)
                if a:
                    print(ct, ' ', year, month, "ok")
                    time.sleep(1)
        elif year == endYear:
            for month in range(1, endMonth + 1):
                a = True
                if whetherhas(ct, year, month) == False:
                    a = get_data(getURL(ct, year, month), ct, year, month)
                if a:
                    print(ct, ' ', year, month, "ok")
                    time.sleep(1)
        else:
            for month in range(1, 13):
                a = True
                if whetherhas(ct, year, month) == False:
                    a = get_data(getURL(ct, year, month), ct, year, month)
                if a:
                    print(ct, ' ', year, month, "ok")
                    time.sleep(1)


def topower(s):
    a = 0
    for i in range(len(s)):
        if s[i] == '.':
            a += int(s[i + 1]) * 0.1
            break
        elif s[i] == 'm':
            break
        else:
            a = a * 10 + int(s[i])
    if a <= 0.2:
        return 0
    elif a <= 1.5:
        return 1
    elif a <= 3.3:
        return 2
    elif a <= 5.4:
        return 3
    elif a <= 7.9:
        return 4
    elif a <= 10.7:
        return 5
    elif a <= 13.8:
        return 6
    elif a <= 17.1:
        return 7
    elif a <= 20.7:
        return 8
    elif a <= 24.4:
        return 9
    elif a <= 28.4:
        return 10
    elif a <= 32.6:
        return 11
    else:
        return 12

def toweather(url):
    pic=requests.get(url)
    file_name='tmp.png'
    with open(file_name, 'wb') as f:
        f.write(pic.content)
    address='D:\\pythonProject'
    target=''
    for target in os.listdir(address):
        if target[-3:]!="png" or target=="tmp.png":
            continue
        file1=target
        file2=file_name
        image1=cv2.imread(file1)
        image2=cv2.imread(file2)
        difference=cv2.subtract(image1, image2)
        result = not np.any(difference)
        if result:
            break

    file_name="tmp.png"
    os.remove(file_name)
    diction={"yintian":"阴天",'cloudy':"多云",'midrain':'中雨','heavy_rain':"暴雨",'smallrain':'小雨','sometimesrain':'阵雨','summer':"晴天","thunder_rain":'雷阵雨'}
    return diction[target[:target.__len__()-4]]
# GetDataByHours:获得过去24小时温度数据
# 思路：先获得逐小时天气预报数据，然后插值法扩充数据，最后再用线性拟合/多项式拟合
def GetDataByHours(ct):
    '''
    conn = pymysql.connect(
        host='localhost',
        port=3306,
        user='root',
        password='zhoujin@MySQL',
        charset='utf8',
        database="data"
    )
    '''
    conn = pymysql.connect(
        host='60.205.232.122',
        port=3306,
        user='root',
        password='QAZ123wsx',
        charset='utf8',
        database="data"
    )
    '共用数据表：hours_data'
    '数据项：(城市，月，日，小时，温度，湿度，天气，风向，风力)'
    'past 12 hours data'

    url = "https://datashareclub.com/area/"
    province = {"武汉": "湖北", "杭州": "浙江", "长沙": "湖南", "北京": "北京", "上海": "上海", "南京": "江苏"}
    url += province[ct] + '/' + ct + '.html'
    url = quote(url, safe=string.printable)
    html = askURL(url)
    cursor = conn.cursor()
    soup = BeautifulSoup(html, "html.parser")
    sql = "truncate table hours_data"
    cursor.execute(sql)
    conn.commit()

    for item in soup.find('tbody'):
        item = str(item)
        lst = re.findall(findHoursData, item)
        for itemSec in lst:
            lstSec = re.findall(findHoursDataSec, str(itemSec))
            sql = "INSERT INTO hours_data(month,day,hour,temperature,humidity,weather,wind_direction,wind_power) VALUES (%d,%d,%d,%f,\'%s\',\'%s\',\'%s\',\'%s\')"
            month = toint(lstSec[1][0:2])
            day = toint(lstSec[1][3:5])
            hour = toint(lstSec[1][14:16])
            temperature = getdouble(lstSec[3][:(lstSec[3].__len__() - 1)])
            humidity = lstSec[9]
            weather = lstSec[2]
            wind_direction = lstSec[5]
            wind_power = lstSec[6]
            dats = (month, day, hour, temperature, humidity, weather, wind_direction, wind_power)
            sql %= dats
            cursor.execute(sql)
            conn.commit()
    'future'

    url = "https://weather.cma.cn/web/weather/"
    url += cityToProvince[ct] + '.html'
    html = askURL(url)
    soup = BeautifulSoup(html, "html.parser")
    url = "https://weather.cma.cn"
    datum = []
    weatherlst = soup.find_all('td', class_='wicon')
    weatherData = []
    for item in soup.find('table', class_="hour-table"):
        item = str(item)
        lst = re.findall(findHoursDataThird, item)
        id=0
        if len(lst):
            for i in range(8):

                if datum.__len__():
                    if toint(lst[i][:2])<datum[-1][1]:
                        id=1
                datum.append([id, toint(lst[i][:2]),getdouble(lst[i+8*1]), lst[i + 8 * 6][:lst[i + 8 * 6].__len__()],
                                  lst[i + 8 * 4][:lst[i + 8 * 4].__len__()],
                                  str(topower(lst[i + 8 * 3][:lst[i + 8 * 3].__len__()])) + '级'])
    for i in range(8):
        url_add = str(weatherlst[i])[28:len(str(weatherlst[i]))-8]
        url_add=url+url_add
        weatherData.append(toweather(url_add))
    for i in range(8):
        day=datetime.datetime.today()+datetime.timedelta(datum[i][0])
        month=day.month
        day=day.day
        sql = "INSERT INTO hours_data(month,day,hour,temperature,humidity,weather,wind_direction,wind_power) VALUES (%d,%d,%d,%f,\'%s\',\'%s\',\'%s\',\'%s\')"
        sql%=(month,day,datum[i][1],datum[i][2],datum[i][3],weatherData[i],datum[i][4],datum[i][5])
        cursor.execute(sql)
        conn.commit()

    conn.commit()
    cursor.close()
    conn.close()


if __name__ == '__main__':
    '''
    n=int(input('times:'))
    for i in range(n):
        city=input("city:")
        syear=int(input('syear:'))
        smonth=int(input('smonth:'))
        eyear = int(input('eyear:'))
        emonth = int(input('emonth:'))
        fetchData(city,syear,eyear,smonth,emonth)
        print("City ",city,"'s data from",syear,".",smonth,"to",eyear,".",emonth,end='')
        print()
    '''

    city="武汉"
    syear=2023
    smonth=1
    eyear=2023
    emonth=12
    fetchData(city,syear,eyear,smonth,emonth)
    '''
    GetDataByHours("杭州")'''
