import pandas as pd
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
import numpy as np
import matplotlib.pyplot as plt
import mglearn

'画标记数据集的图'
'''
iris_dataframe=pd.DataFrame(X_train,columns=iris_dataset.feature_names)
grr=pd.plotting.scatter_matrix(iris_dataframe,c=y_train,figsize=(15,15),marker='o',hist_kwds={'bins':20},s=60,alpha=0.8,cmap=mglearn.cm3)
plt.show()
'''

X, y = mglearn.datasets.make_wave(n_samples=40)
print(X.__len__())
print(y)