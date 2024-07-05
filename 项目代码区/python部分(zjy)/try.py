from scipy.interpolate import BarycentricInterpolator
import numpy as np
import matplotlib.pyplot as plt
x = np.array([0, 1, 2, 3, 4, 5])
y = np.array([0, 3, 4, 1, 0, 4])

# 创建多项式插值函数
f = BarycentricInterpolator(x, y)
# 计算插值结果
x_new = np.linspace(0, 5, 100)
y_new = f(x_new)

# 绘制x和y的图形
plt.plot(x, y, 'o', label='原始数据')
# 绘制x_new和y_new的图形
plt.plot(x_new, y_new, '-', label='多项式插值结果')
# 添加图例
plt.legend()
# 显示图形
plt.show()