import numpy as np
import pymysql
import random
from scipy.interpolate import make_interp_spline

def predict_fake(max_temp,min_temp,length):
    conn = pymysql.connect(
        host='60.205.232.122',
        port=3306,
        user='root',
        password='123456',
        charset='utf8',
        database="data"
    )
    cursor = conn.cursor()
    make_data(max_temp,min_temp,conn,cursor,length)


    cursor.close()
    conn.close()

def make_data(max_temp,min_tem,conn,cursor,length):
    dat=[]
    for i in range(length):
        if len(dat)>0:
            dat.append(random.randint(max(min_tem,dat[len(dat)-1]-5),min(max_temp,dat[len(dat)-1]+5)))
        else:
            dat.append(random.randint(min_tem,max_temp))
    sql="truncate fake_data"
    cursor.execute(sql)
    sql="INSERT INTO fake_data (day,temperature,is_predict) values (%d,%d,0)"
    dat=np.array(dat)
    times=[i for i in range(len(dat))]
    times=np.array(times)
    for i in range(len(dat)):
        cursor.execute(sql%(i,dat[i]))
        conn.commit()

if __name__ == '__main__':
    predict_fake(20,10,20)