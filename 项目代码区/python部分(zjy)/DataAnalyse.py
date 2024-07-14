import datetime
import GetData
import pymysql
import matplotlib.pyplot as plt
from scipy.interpolate import make_interp_spline
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from keras.models import Sequential
from keras.layers import Dense
import numpy as np
from sklearn.model_selection import train_test_split
import keras
from config import Config
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
        host='60.205.232.122',
        port=3306,
        user='root',
        password='123456',
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
        host='60.205.232.122',
        port=3306,
        user='root',
        password='123456',
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
        host='60.205.232.122',
        port=3306,
        user='root',
        password='123456',
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


def deeplearning(x_train, y_train, x_val, y_val, x_tar, x_test, y_test):
    # 确保输入数据是二维的
    x_train = np.expand_dims(x_train, axis=1)
    x_val = np.expand_dims(x_val, axis=1)
    x_tar = np.expand_dims(x_tar, axis=1)
    x_test = np.expand_dims(x_test, axis=1)

    model = Sequential()
    model.add(Dense(80,input_dim=1, activation='relu'))
    model.add(Dense(80, input_dim=1, activation='relu'))
    model.add(Dense(80, input_dim=1, activation='relu'))
    model.add(Dense(80, input_dim=1, activation='relu'))
    model.add(Dense(80, input_dim=1, activation='relu'))
    model.add(Dense(80, input_dim=1, activation='relu'))
    model.add(Dense(1, activation='linear'))

    model.compile(loss='mean_squared_error', optimizer='adam', metrics=['mean_absolute_error'])
    tensorboard_callback = keras.callbacks.TensorBoard(log_dir="./logs")

    model.fit(x_train, y_train, epochs=100, batch_size=64, validation_data=(x_val, y_val),
              callbacks=[tensorboard_callback])
    predictions = model.predict(x_tar)

    history = model.fit(x_train, y_train, epochs=500,
                        batch_size=64, validation_data=(x_val, y_val),callbacks=[tensorboard_callback])
    loss = history.history['loss']
    val_loss = history.history['val_loss']
    # Plot the training and validation loss
    plt.plot(loss, label='Training loss')
    plt.plot(val_loss, label='Validation loss')
    plt.legend()
    plt.show()
    return predictions




def predictFuturedays(city):#默认有了fetch的数据,需要两年的

    year=datetime.datetime.today().year
    month=datetime.datetime.today().month
    GetData.fetchData(city, year-1,year-1,month,12)
    GetData.fetchData(city, year, year, 1, month)
    '''
    conn = pymysql.connect(
        host=Config.MYSQL_HOST,
        user=Config.MYSQL_USER,
        password=Config.MYSQL_PASSWORD,
        database=Config.MYSQL_DB,
    )
    '''
    conn = pymysql.connect(
        host='localhost',
        port=3306,
        user='root',
        password='zhoujin@MySQL',
        charset='utf8',
        database="data"
    )
    cursor = conn.cursor()
    SQL="SELECT * FROM climate where year>=%d and year<=%d and city=\'%s\' order by year asc,month asc"%(year-1,year,city)
    cursor.execute(SQL)
    lst=cursor.fetchall()
    cursor.close()
    conn.close()
    max_temperaturelst=[]
    times=[]
    for i in range(lst.__len__()):
        max_temperaturelst.append(lst[i][0])
        times.append(i)
    times=np.array(times)
    max_temperaturelst=np.array(max_temperaturelst)
    poly_tem=make_interp_spline(times,max_temperaturelst)
    x_new = np.linspace(0, times.__len__()-1, num=3*365)
    y_new_tem = poly_tem(x_new)
    plt.scatter(x_new, y_new_tem)
    plt.legend()
    plt.show()
    x_train_temp, x_test_temp, y_train_temp, y_test_temp = train_test_split(x_new, y_new_tem, test_size=0.15,
                                                                            random_state=42)
    x_train_temp, x_val_temp, y_train_temp, y_val_temp = train_test_split(x_new, y_new_tem, test_size=0.15,
                                                                          random_state=42)
    x_want = []
    x_want.append(times.__len__())
    x_want = np.array(x_want)
    y_want_temp = deeplearning(x_train_temp, y_train_temp, x_val_temp, y_val_temp, x_want, x_test_temp, y_test_temp)

    plt.scatter(x_train_temp,y_train_temp)
    plt.scatter(x_test_temp, y_test_temp)

    plt.scatter(x_want, y_want_temp)
    plt.legend()
    plt.show()
    ''''''

'''
思路：日期*24+小时，唯一的问题在于跨日（或者跨月）
    plt.scatter(x_new,y_new_tem,label="拟合")
    plt.scatter(times,temperature,label="原始数据")
    plt.legend()
    plt.show()
'''
if __name__ == '__main__':
    predictFuturedays("武汉")
