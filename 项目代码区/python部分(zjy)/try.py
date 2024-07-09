import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import pymysql
import matplotlib.pyplot as plt
from torch.utils.data import Dataset, DataLoader, random_split
import pandas as pd
from sklearn.preprocessing import StandardScaler
# input_size输入特征的维度
# hidden_size隐藏层的维度，即每个LSTM单元的隐藏状态向量的维度。
# output_size：输出的维度。
# num_layers：LSTM层的数量，默认为1。


class LSTMModel(nn.Module):
    def __init__(self, input_size, hidden_size, output_size, num_layers=1):
        super(LSTMModel, self).__init__()
        self.hidden_size = hidden_size
        self.num_layers = num_layers

        # 定义lsmt层
        # batch_first=True表示输入数据的形状是(batch_size, sequence_length, input_size)
        # 而不是默认的(sequence_length, batch_size, input_size)。
        # batch_size是指每个训练批次中包含的样本数量
        # sequence_length是指输入序列的长度
        self.lstm = nn.LSTM(input_size, hidden_size, num_layers, batch_first=True)

        # 定义全连接层，将LSTM层的输出映射到最终的输出空间。
        self.fc = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        # 初始化了隐藏状态h0和细胞状态c0，并将其设为零向量。
        x = x.view(len(x), 1, -1)
        h0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)
        c0 = torch.zeros(self.num_layers, x.size(0), self.hidden_size).to(x.device)

        # LSTM层前向传播
        # 将输入数据x以及初始化的隐藏状态和细胞状态传入LSTM层
        # 得到输出out和更新后的状态。
        # out的形状为(batch_size, sequence_length, hidden_size)。
        out, _ = self.lstm(x, (h0, c0))

        # 全连接层前向传播
        # 使用LSTM层的最后一个时间步的输出out[:, -1, :]（形状为(batch_size, hidden_size)）作为全连接层的输入，得到最终的输出。
        out = self.fc(out[:, -1, :])

        return out


class CustomDataset(Dataset):
    def __init__(self, year, transform=None):
        conn = pymysql.connect(
            host='localhost',
            port=3306,
            user='root',
            password='zhoujin@MySQL',
            charset='utf8',
            database="data"
        )
        cursor = conn.cursor()
        sql = "SELECT max_temperature,year,month,day FROM climate where year=%d order by year asc,month asc,day asc"%year
        cursor.execute(sql)

        cursor.close()
        conn.close()
        self.data = cursor.fetchall()

        self.transform = transform
        self.X=[]
        self.Y=[]
        # 三次指数平滑
        #self.X = self.X.apply(self.triple_exponential_smoothing, axis=0)

        # 提取特征和目标变量
        for i in self.data:
            self.X.append((i[1],i[2],i[3]))
            self.Y.append(i[0])

        self.scaler = StandardScaler()

        # 使用StandardScaler对数据进行标准化处理，以确保训练过程中的数值稳定性（可选）
        # pd.DataFrame()函数，可以将数据从不同的数据源（如列表、字典、NumPy数组等）转换成数据帧
        #self.scaler = StandardScaler()
        #self.X = pd.DataFrame(self.scaler.fit_transform(self.X), columns=['年','月','日'])

        self.X = pd.DataFrame(self.X, columns=['年', '月', '日'])
        self.Y = pd.DataFrame(np.array(self.Y).reshape(-1,1), columns=['最高气温'])
        self.length=len(self.X)
    def __len__(self):
        return len(self.X)

    def __getitem__(self, idx):
        # 从self.X中选择索引为idx的行，并将其赋值给X_sample
        X_sample = self.X.iloc[idx]
        Y_sample = self.Y.iloc[idx]

        # 特征数据和标签数据都被转换为float32类型的PyTorch张量。
        sample = {'X': torch.tensor(X_sample.values, dtype=torch.float32),
                  'Y': torch.tensor(Y_sample, dtype=torch.float32)}

        # 在这里进行数据预处理，如果需要的话
        if self.transform:
            sample = self.transform(sample)

        return sample




def load_data():
    # 读取CSV文件

    # 创建数据集实例
    custom_dataset = CustomDataset(2023)

    # 划分训练集和测试集
    train_size = int(0.9 * custom_dataset.length)  # 训练集占比90%
    test_size = custom_dataset.length - train_size  # 测试集占比10%
    train_dataset, test_dataset = random_split(custom_dataset, [train_size, test_size])  # 按照比例随机划分

    # 创建数据加载器
    # batch_size参数用于指定每个批次（batch）中包含的样本数量。
    # 通常情况下，较大的batch_size可以加快训练速度，但可能会占用更多的内存资源。
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)

    # 检查数据加载器
    for batch in train_loader:
        print(batch['X'].shape, batch['Y'].shape)
        break

    return train_loader, test_loader, custom_dataset




def evaluate_model(model, data_loader, criterion):
    # 评估模块
    model.eval()
    total_loss = 0
    with torch.no_grad():
        for batch in data_loader:
            X_batch = batch['X'].unsqueeze(1)
            Y_batch = batch['Y'].unsqueeze(1)

            outputs = model(X_batch)
            loss = criterion(outputs, Y_batch)
            total_loss += loss.item()
    return total_loss / len(data_loader)


def main():
    # 加载训练集和测试集和对应的数据加载器
    train_loader, test_loader, custom_dataset = load_data()

    # 设置超参数
    input_size = len(custom_dataset.X.columns)  # 特征数量
    hidden_size = 20  # 隐藏层大小
    # 隐藏层大小是指每个LSTM单元中的隐藏状态向量的维度，它在很大程度上影响了模型的表示能力和性能
    # 如果隐藏层太大，模型可能会过拟合训练数据，对测试数据表现不佳。
    # 如果隐藏层太小，模型可能无法捕捉到足够的信息，导致欠拟合。

    output_size = 1  # 输出大小（预测当日票房）

    num_layers = 20 # LSTM层数
    # 单层LSTM：适用于简单的序列建模任务，结构简单，计算效率高。
    # 多层LSTM：适用于复杂的序列建模任务，能够捕捉更复杂的模式和长距离依赖，但需要更多的计算资源。
    # 层数选择：需要通过实验来确定，考虑任务复杂度、数据量和计算资源。

    learning_rate = 0.01  # 学习率
    # 对于较小的网络或简单任务，较大的学习率（如 0.01)可能是合适的。
    # 对于较深的网络或复杂任务，较小的学习率（如 0.0001)可能是必要的。

    # 实例化模型、损失函数和优化器
    model = LSTMModel(input_size, hidden_size, output_size, num_layers)  # 在GPU上训练
    criterion = nn.SmoothL1Loss()  # 均方误差损失函数
    # 回归损失函数
    # torch.nn.MSELoss 用于回归任务，计算预测值与目标值之间的均方误差。
    # torch.nn.L1Loss 用于回归任务，计算预测值与目标值之间的平均绝对误差。
    # torch.nn.SmoothL1Loss 结合了 L1Loss 和 MSELoss 的优点，对于回归任务更为稳健。
    optimizer = optim.RMSprop(model.parameters(), lr=learning_rate)
    # model.parameters()，获取模型中所有需要训练的参数（权重和偏置）
    # SGD：适合大规模数据和需要较好泛化性能的任务，可以通过调节学习率和添加动量（Momentum）来改进。
    # RMSprop：适合处理非平稳目标，可以自动调整学习率。
    # Adam：适用于大多数情况，特别是有噪声的梯度和稀疏梯度的情形。

    # 训练模型
    ''''''
    try:
        epoch = 0
        model.train()
        trainloss=[]
        while True:
            for batch in train_loader:
                X_batch = batch['X'].unsqueeze(1) # 增加序列维度
                Y_batch = batch['Y'].unsqueeze(1)

                # 前向传播
                outputs = model(X_batch)  # 经过LSTM层，全连接层，生成输出
                loss = criterion(outputs, Y_batch)  # 计算模型输出与目标值之间的损失

                # 反向传播及优化
                optimizer.zero_grad()  # 梯度清零，防止堆积

                # 反向传播PyTorch会自动计算损失相对于模型参数的梯度
                # 在这一步中，PyTorch会执行以下操作：
                # 从损失开始，沿计算图的反向方向计算梯度。
                # 对于LSTM层，这意味着计算损失相对于LSTM权重和偏置的梯度。LSTM层的反向传播包括计算输入门、遗忘门、输出门和候选记忆细胞的梯度。
                # 对于全连接层（Linear layer），计算损失相对于线性层权重和偏置的梯度。
                loss.backward()

                torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)  # 添加梯度裁剪，防止梯度爆炸

                # 调用优化器的step函数，使用计算得到的梯度来更新模型参数
                # 优化器会使用学习率来缩放梯度。
                # 对于每个参数，优化器会减去相应的梯度乘以学习率，从而更新参数。
                optimizer.step()

            if (epoch + 1) % 50 == 0:
                # 每20epoch使用测试集计算一次损失,并保存模型
                test_loss = evaluate_model(model, test_loader, criterion)
                print(f'Epoch [{epoch + 1}], Train Loss: {loss.item():.4f}, Test Loss: {test_loss:.4f}')
                trainloss.append(loss.item())
                # 保存模型
                torch.save(model.state_dict(), f'D:/model/lstm_model_{epoch + 1}.pth')
                print(f"训练完成并保存模型(lstm_model_{epoch + 1}.pth)。")
                model.train()
            else:
                print(f'Epoch [{epoch + 1}], Train Loss: {loss.item():.4f}')
                trainloss.append(loss.item())

            epoch += 1
            if epoch == 360:
                trainloss=np.array(trainloss)
                plt.plot(trainloss,label="train loss")
                plt.legend()
                plt.show()
                break
    except KeyboardInterrupt:
        # ctrl+c中止训练
        print("训练已中止。")

    model_predict = LSTMModel(input_size, hidden_size, output_size, num_layers)
    model_predict.load_state_dict(torch.load('D:/model/lstm_model_100.pth'))
    input_data=np.array([2025,1,1])
    prediction=predict(model_predict,input_data,custom_dataset.scaler)
    print(prediction)

def predict(model, input_data, scaler):
    model.eval()  # 将模型设置为评估模式
    with torch.no_grad():  # 使用torch.no_grad()上下文管理器来关闭梯度计算，以便在推断过程中不计算梯度。
        input_data = input_data.reshape(-1,1)
        # 将输入数据转换为torch张量（tensor），并进行一些维度调整，最后将其移动到GPU上
        input_tensor = torch.tensor(input_data, dtype=torch.float32).unsqueeze(0)
        output = model(input_tensor)  # 将输入数据传递给模型，获取输出结果
        return output.item()


if __name__ == "__main__":
    main()