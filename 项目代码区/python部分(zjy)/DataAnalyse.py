import pymysql
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import make_interp_spline
import GetData
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from keras.models import Sequential
from keras.layers import Dense
import numpy as np
from sklearn.model_selection import train_test_split
'''
接口函数：
1.平均最高气温(ok)
2.平均最低气温(ok)
3.平均气温(ok)
4.预测：过去24小时（预测）
        predictFuture12Hours(ct)
 + 
 
 
 
 根据年际数据预测今日 （显示往年数据）（先爬取历年当月数据：GetData.py中的函数）
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


def predictFuture12Hours(ct):
    GetData.GetDataByHours(ct)
    conn = pymysql.connect(
        host='localhost',
        port=3306,
        user='root',
        password='zhoujin@MySQL',
        charset='utf8',
        database="data"
    )
    cursor = conn.cursor()
    SQL = "SELECT * FROM future_hours_weather"
    cursor.execute(SQL)
    lst = cursor.fetchall()
    times = []
    air_quality = []
    temperature = []
    for item in lst:
        temperature.append(item[3])
        air_quality.append(item[4])
    temperature = temperature[::-1]
    air_quality = air_quality[::-1]
    for i in range(24):
        times.append(i)
    times = np.array(times)
    air_quality = np.array(air_quality)
    temperature = np.array(temperature)
    poly_air = make_interp_spline(times, air_quality)  # 样条插值扩充样本
    poly_tem = make_interp_spline(times, temperature)
    x_new = np.linspace(0, 23, num=150)
    y_new_air = poly_air(x_new)
    y_new_tem = poly_tem(x_new)

    x_train_temp,x_test_temp,y_train_temp,y_test_temp=train_test_split(x_new,y_new_tem,test_size=0.15,random_state=42)
    x_train_temp, x_val_temp, y_train_temp, y_val_temp = train_test_split(x_new, y_new_tem, test_size=0.15,
                                                                            random_state=42)

    model=Sequential()
    model.add(Dense(50, input_dim=1, activation='relu'))
    model.add(Dense(1, activation='linear'))
    model.compile(loss='mean_squared_error', optimizer='adam',
                  metrics=['mean_absolute_error'])
    history = model.fit(x_train_temp, y_train_temp, epochs=500,
                        batch_size=30, validation_data=(x_val_temp, y_val_temp))
    predictions = model.predict(x_test_temp)
    loss = history.history['loss']
    val_loss = history.history['val_loss']
    mae = history.history['mean_absolute_error']
    val_mae = history.history['val_mean_absolute_error']
    # Plot the training and validation loss
    plt.plot(loss, label='Training loss')
    plt.plot(val_loss, label='Validation loss')
    plt.legend()
    plt.show()
    cursor.close()
    conn.close()

'''
思路：日期*24+小时，唯一的问题在于跨日（或者跨月）
    plt.scatter(x_new,y_new_tem,label="拟合")
    plt.scatter(times,temperature,label="原始数据")
    plt.legend()
    plt.show()
'''
if __name__ == '__main__':
    predictFuture12Hours("武汉")
