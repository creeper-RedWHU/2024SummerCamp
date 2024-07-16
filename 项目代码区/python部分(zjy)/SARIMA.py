import datetime
import numpy as np
import pandas as pd
import statsmodels.api as sm
import matplotlib.pyplot as plt
from statsmodels.stats.diagnostic import acorr_ljungbox
from statsmodels.graphics.tsaplots import plot_acf, plot_pacf
import itertools
import warnings
import pymysql
import torch
from config import Config
import GetData
from statsmodels.tsa.stattools import adfuller as ADF

warnings.filterwarnings("ignore")  # 忽略输出警告
plt.rcParams["font.sans-serif"] = ["SimHei"]  # 用来正常显示中文标签
plt.rcParams["axes.unicode_minus"] = False  # 用来显示负号
'''数据的平稳性检验'''


def test_stationarity(timeseries, alpha):
    adf = ADF(timeseries)
    p = adf[1]
    critical_value = adf[4]["5%"]  # 在95%置信区间下的临界的ADF检验值
    test_statistic = adf[0]  # ADF统计量
    if p < alpha and test_statistic < critical_value:
        print("ADF平稳性检验结果：在显著性水平%s下，数据经检验平稳" % alpha)
        return True
    else:
        print("ADF平稳性检验结果：在显著性水平%s下，数据经检验不平稳" % alpha)
        return False


def STLJudge(NGE):
    decompostion = sm.tsa.STL(NGE, period=12).fit()  # statsmodels.tsa.api:时间序列模型和方法
    decompostion.plot()
    # 趋势效益
    trend = decompostion.trend
    # 季节效应
    seasonal = decompostion.seasonal
    # 随机效应
    residual = decompostion.resid
    plt.show()


def test_white_noise(data, alpha):
    sc = acorr_ljungbox(data, lags=1)
    p = sc["lb_pvalue"][1]
    if p < alpha:
        print('LB白噪声检验结果：在显著性水平%s下，数据经检验为非白噪声序列' % alpha)
    else:
        print('LB白噪声检验结果：在显著性水平%s下，数据经检验为白噪声序列' % alpha)


def SARIMA_search(data):
    p = q = range(0, 3)
    s = [12]  # 周期为12
    d = [1]  # 做了一次季节性差分
    PDQs = list(itertools.product(p, d, q, s))  # itertools.product()得到的是可迭代对象的笛卡儿积
    pdq = list(itertools.product(p, d, q))  # list是python中是序列数据结构，序列中的每个元素都分配一个数字定位位置
    params = []
    seasonal_params = []
    results = []
    grid = pd.DataFrame()
    for param in pdq:
        for seasonal_param in PDQs:
            # 建立模型
            mod = sm.tsa.SARIMAX(data, order=param, seasonal_order=seasonal_param,
                                 enforce_stationarity=False, enforce_invertibility=False)
            # 实现数据在模型中训练
            result = mod.fit()
            print("ARIMA{}x{}-AIC:{}".format(param, seasonal_param, result.aic))
            # format表示python格式化输出，使用{}代替%
            params.append(param)
            seasonal_params.append(seasonal_param)
            results.append(result.aic)
    grid["pdq"] = params
    grid["PDQs"] = seasonal_params
    grid["aic"] = results
    print(grid[grid["aic"] == grid["aic"].min()])


def SARIMA_Predict(city):
    conn = pymysql.connect(
        host=Config.MYSQL_HOST,
        user=Config.MYSQL_USER,
        password=Config.MYSQL_PASSWORD,
        database=Config.MYSQL_DB,
    )
    '思路：获取历史几年的当月数据'
    month_day = {1: 31, 2: 28, 3: 31, 4: 30, 5: 31, 6: 30, 7: 31, 8: 31, 9: 30, 10: 31, 11: 30, 12: 31}
    cursor = conn.cursor()
    ''''''
    sql = "truncate table future_climate"
    cursor.execute(sql)
    for i in range(0, 2):
        month_now = datetime.datetime.today().month + i
        year_now = datetime.datetime.today().year
        if month_now > 12:
            month_now = 1
            year_now += 1
        for year in range(year_now - 5, year_now+1):
            if i==1:
                if year==year_now:
                    break
            GetData.fetchData(city, year, year, month_now, month_now)
        sql = "select * from climate where month=%d and year>=%d and year<%d and city=\'%s\' order by year asc,month asc,day asc" % (
            month_now, year_now - 5, year_now, city)
        cursor.execute(sql)
        lst = cursor.fetchall()
        temperature_max_list = [lst[i][0] for i in range(len(lst))]
        temperature_min_list = [lst[i][1] for i in range(len(lst))]
        date_list = [str(lst[i][3]) + str(lst[i][4]) + str(lst[i][5]) for i in range(len(lst))]
        dict_max = {"max_temp": temperature_max_list, "date": date_list}
        dict_min = {"min_temp": temperature_min_list, "date": date_list}
        data_max = pd.DataFrame(dict_max)
        data_min = pd.DataFrame(dict_min)
        NGE_max_tmp = data_max["max_temp"]
        NGE_min_tmp = data_min["min_temp"]
        model = sm.tsa.SARIMAX(NGE_max_tmp, order=(0, 1, 2), seasonal_order=(2, 1, 2, month_day[month_now]))
        SARIMA_m = model.fit()
        model_min = sm.tsa.SARIMAX(NGE_min_tmp, order=(0, 1, 2), seasonal_order=(2, 1, 2, month_day[month_now]))
        SARIMA_m_min = model_min.fit()

        # 预测未来
        forecast = SARIMA_m.get_forecast(steps=month_day[month_now])
        forecast_min = SARIMA_m_min.get_forecast(steps=month_day[month_now])

        for j in range(len(forecast.predicted_mean)):
            if i == 0:
                if j < datetime.datetime.today().day - 1:
                    continue
            sql = "INSERT INTO future_climate VALUES (%d,%d,%d,%f,\'%s\',%f)"
            sql %= (year_now, month_now, j + 1, forecast.predicted_mean[j + 5 * month_day[month_now]], city,
                    forecast_min.predicted_mean[j + 5 * month_day[month_now]])
            cursor.execute(sql)
            conn.commit()

    '思路：读取history数据，横坐标为日期（年-月-日），纵坐标为气温，红色是最高温，蓝色是最低温'
    '读取±10天的数据'

    max_temp_list=[]
    min_temp_list=[]
    date_list=[]
    '读历史'
    delta= 10
    while delta>0:
        history_ = datetime.datetime.today() - datetime.timedelta(delta)
        history_year = history_.year
        history_month = history_.month
        history_day = history_.day
        sql = "select max_temperature , min_temperature from climate where year =%d and month =%d and day=%d and city=\'%s\'"
        sql%=(history_year,history_month,history_day,city)
        cursor.execute(sql)
        lx=cursor.fetchall()
        max_temp_list.append(lx[0][0])
        min_temp_list.append(lx[0][1])
        date_list.append(str(history_year)+'-'+str(history_month)+'-'+str(history_day))
        delta=delta-1
    print(max_temp_list)
    print(min_temp_list)
    delta=0
    while delta<=10:
        history_ = datetime.datetime.today() + datetime.timedelta(delta)
        history_year = history_.year
        history_month = history_.month
        history_day = history_.day
        sql = "select max_temperature , min_temperature from future_climate where year =%d and month =%d and day=%d and city=\'%s\'"
        sql %= (history_year, history_month, history_day, city)
        cursor.execute(sql)
        lx = cursor.fetchall()
        max_temp_list.append(lx[0][0])
        min_temp_list.append(lx[0][1])
        date_list.append(str(history_year) + '-' + str(history_month) + '-' + str(history_day))
        delta=delta+1
    for i in range(len(max_temp_list)):
        max_t=max(max_temp_list[i],min_temp_list[i])
        min_t=min(max_temp_list[i],min_temp_list[i])
        max_temp_list[i]=max_t
        min_temp_list[i]=min_t
    plt.plot(date_list,max_temp_list,color='r',label='最高温',marker='*')
    plt.plot(date_list, min_temp_list,color='b',label='最低温',marker='.')
    plt.xlabel('时间')  # 设置x坐标轴的名称
    plt.ylabel('温度/℃')  # 设置y坐标轴的名称
    x_major_locator = plt.MultipleLocator(1)
    # 把x轴的刻度间隔设置为1，并存在变量里
    y_major_locator = plt.MultipleLocator(1)
    plt.xticks(rotation=30)
    # 把y轴的刻度间隔设置为10，并存在变量里
    ax = plt.gca()
    # ax为两条坐标轴的实例
    ax.xaxis.set_major_locator(x_major_locator)
    # 把x轴的主刻度设置为1的倍数
    ax.yaxis.set_major_locator(y_major_locator)
    #plt.legend()
    #plt.show()
    plt.savefig('sample.png', dpi=300,bbox_inches='tight', pad_inches=0)
    cursor.close()
    conn.close()


if __name__ == '__main__':
    SARIMA_Predict("武汉")
