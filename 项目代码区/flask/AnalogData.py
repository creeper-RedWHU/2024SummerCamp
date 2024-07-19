import numpy as np
import pandas as pd
import random
from keras.models import Sequential
from keras.layers import Dense
import keras
import pymysql
from sklearn.model_selection import train_test_split
from config import Config
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt
from statsmodels.tsa.holtwinters import ExponentialSmoothing
from statsmodels.tsa.ar_model import AutoReg
from statsmodels.tsa.statespace.sarimax import SARIMAX
from sklearn.neighbors import KNeighborsRegressor
"""
模拟算法

"""
def LinerRegression_1(lst,x_new,cursor,conn):
    '''
    线性回归算法
    '''
    lr=LinearRegression()
    x=[]
    y=[]
    for i in lst:
        x.append(i[1])
        y.append(i[0])
    x=np.array(x).reshape(-1,1)
    y=np.array(y).reshape(-1,1)
    lr.fit(x,y)
    x_new=x_new.reshape(-1,1)

    y_predict = lr.predict(x_new)
    sql="insert into analog_data VALUES(%f,%d,1)"
    for i in range(len(y_predict)):
        cursor.execute(sql%(y_predict[i],x_new[i]))
        conn.commit()

def Holt_Winters_method_2(lst,cursor,conn):
    '''
    Holt-Winters 方法(三次指数平滑)
    '''
    y = []
    for i in lst:
        y.append(i[0])
    data_sr=pd.Series(y)
    fit = ExponentialSmoothing(data_sr, seasonal_periods=20, trend='add', seasonal='add', damped_trend=True).fit(smoothing_level=0.5)
    sr = fit.forecast(10)
    sql = "insert into analog_data VALUES(%f,%d,2)"
    for i in sr.index:
        cursor.execute(sql%(sr[i],i))
        conn.commit()

def AutoReg_model_3(lst,cursor,conn):
    '''
    自回归模型
    '''
    y = []
    for i in lst:
        y.append(i[0])
    model=AutoReg(y,lags=5)
    model_fit=model.fit()
    forecast=model_fit.predict(start=len(y),end=len(y)+10)
    sql = "insert into analog_data VALUES(%f,%d,3)"
    for i in range(forecast.__len__()):
        cursor.execute(sql%(forecast[i],i+40))
        conn.commit()

def deeplearning(x_train, y_train, x_val, y_val, x_tar):
    # 确保输入数据是二维的
    x_train = np.expand_dims(x_train, axis=1)
    x_val = np.expand_dims(x_val, axis=1)
    x_tar = np.expand_dims(x_tar, axis=1)

    model = Sequential()
    model.add(Dense(80,input_dim=1, activation='relu'))
    model.add(Dense(80, input_dim=1, activation='relu'))
    model.add(Dense(80, input_dim=1, activation='relu'))
    model.add(Dense(80, input_dim=1, activation='relu'))
    model.add(Dense(1, activation='linear'))

    model.compile(loss='mean_squared_error', optimizer='adam', metrics=['mean_absolute_error'])
    tensorboard_callback = keras.callbacks.TensorBoard(log_dir="./logs")
    model_new=model.fit(x_train, y_train, epochs=500, batch_size=64, validation_data=(x_val, y_val),
              callbacks=[tensorboard_callback])
    predictions = model.predict(x_tar)
    '''
    history = model_new
    
    loss = history.history['loss']
    val_loss = history.history['val_loss']
    # Plot the training and validation loss
    plt.plot(loss, label='Training loss')
    plt.plot(val_loss, label='Validation loss')
    plt.legend()
    plt.show()
    '''
    return predictions


def predictByMachine_4(lst,cursor,conn,x_new):
    x=np.array([i for i in range(40)])
    lst_new=np.array([lst[i][0] for i in range(len(lst))])
    x_train,x_val,y_train,y_val=train_test_split(x, lst_new, test_size=0.15,random_state=42)
    y_want_tmp=deeplearning(x_train,y_train,x_val,y_val,x_new)
    sql = "insert into analog_data VALUES(%f,%d,4)"
    for i in range(len(y_want_tmp)):
        cursor.execute(sql%(y_want_tmp[i],i+40))
        conn.commit()

def predictBySARIMA_5(lst,cursor,conn):
    '''SARIMA模型，时间序列，季节性自回归积分滑动平均模型'''
    lst_new = np.array([lst[i][0] for i in range(len(lst))])
    model=SARIMAX(lst_new,order=(1,1,1),seasonal_order=(1,1,1,12))
    y_want_tmp=model.fit().forecast(steps=10)
    sql = "insert into analog_data VALUES(%f,%d,5)"
    for i in range(len(y_want_tmp)):
        cursor.execute(sql % (y_want_tmp[i], i + 40))
        conn.commit()




def predict_fake(max_temp,min_temp):
    length=40
    
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
    '''
    cursor = conn.cursor()
    make_data(max_temp,min_temp,conn,cursor,length)
    sql = "SELECT * FROM  analog_data where id = 0 order by time asc"
    cursor.execute(sql)
    lst = cursor.fetchall()
    x_new=np.array([i for i in range(40,50)])
    LinerRegression_1(lst,x_new,cursor,conn)
    Holt_Winters_method_2(lst,cursor,conn)
    AutoReg_model_3(lst,cursor,conn)
    predictByMachine_4(lst,cursor,conn,x_new)
    predictBySARIMA_5(lst,cursor,conn)
    cursor.close()
    conn.close()

def make_data(max_temp,min_tem,conn,cursor,length):
    dat=[]
    for i in range(length):
        if len(dat)>0:
            dat.append(random.uniform(max(min_tem,dat[len(dat)-1]-5),min(max_temp,dat[len(dat)-1]+5)))
        else:
            dat.append(random.uniform(min_tem,max_temp))
    sql="truncate analog_data"
    cursor.execute(sql)
    sql="INSERT INTO analog_data (time,temperature,id) values (%d,%f,0)"
    dat=np.array(dat)
    times=[i for i in range(len(dat))]
    times=np.array(times)
    for i in range(len(dat)):
        cursor.execute(sql%(i,dat[i]))
        conn.commit()

if __name__ == '__main__':
    predict_fake(20,10)