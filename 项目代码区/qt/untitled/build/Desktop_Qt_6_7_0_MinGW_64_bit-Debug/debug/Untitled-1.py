# This Python file uses the following encoding: utf-8

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense

def predict_temperature(time_window=24, epochs=30, batch_size=32, validation_split=0.2):
    # 1. 读取和准备数据
    # 数据存储在 'temperature_data.csv' 文件中
    data = pd.read_csv('C://Users//Luaming//Desktop//temperature_data.csv', parse_dates=['timestamp'], index_col='timestamp')

    # 只使用温度列
    temperature_data = data['temperature'].values.reshape(-1, 1)

    # 标准化数据
    scaler = MinMaxScaler(feature_range=(0, 1))
    temperature_data_scaled = scaler.fit_transform(temperature_data)


    # 准备训练数据和标签
    X, y = [], []
    for i in range(len(temperature_data_scaled) - time_window):
        X.append(temperature_data_scaled[i:(i + time_window), 0])
        y.append(temperature_data_scaled[i + time_window, 0])

    X = np.array(X)
    y = np.array(y)

    # 将数据reshape成LSTM所需的3D格式
    X = X.reshape((X.shape[0], X.shape[1], 1))

    # 2. 构建 LSTM 模型
    model = Sequential()
    model.add(LSTM(units=50, return_sequences=True, input_shape=(X.shape[1], 1)))
    model.add(LSTM(units=50))
    model.add(Dense(1))  # 输出单一预测值

    model.compile(optimizer='adam', loss='mean_squared_error')

    # 3. 训练模型
    model.fit(X, y, epochs=epochs, batch_size=batch_size, validation_split=validation_split)

    # 4. 预测未来time_window小时的气温
    # 使用最后的24小时的数据来进行预测
    last_24_hours_data = temperature_data_scaled[-24:]
    last_24_hours_data = last_24_hours_data.reshape((1, 24, 1))

    # 预测未来24小时的气温
    predicted_temperature_scaled = []

    for _ in range(time_window):
        # 使用模型预测下一小时的气温
        next_temp_scaled = model.predict(last_24_hours_data)[0][0]
        predicted_temperature_scaled.append(next_temp_scaled)

        # 更新输入数据，去掉最早的一个小时并加入最新预测的一个小时
        last_24_hours_data = np.append(last_24_hours_data[:, 1:, :], [[[next_temp_scaled]]], axis=1)

    # 将预测结果逆标准化为实际温度
    predicted_temperature = scaler.inverse_transform(np.array(predicted_temperature_scaled).reshape(-1, 1))

    # 可视化结果
    plt.figure(figsize=(10, 6))
    plt.plot(data.index[-100:], temperature_data[-100:], label='history temperature')
    plt.plot([data.index[-1] + pd.Timedelta(hours=i) for i in range(1, 1+time_window)], predicted_temperature, label='predicted temperature',
             color='red')
    plt.xlabel('time')
    plt.ylabel('temperature (°C)')
    plt.legend()
    plt.show()
    return predicted_temperature
predict_temperature()