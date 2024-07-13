# 队伍名称：非常五加一

##  软件名称：好灵气象

##  成员组成和说明：

#####  姓名 角色 联系方式 特点说明 

王皓霖	 项目经理 	18523991519 	较冷静，规划能力较强 

莫韬 	    产品经理 	15773219798 	交际能力强，擅长演讲，善于发现并解决问题

周锦耀 	    开发经理 	15868094322 	技术水平高，有丰富竞赛经验，学习能力强 

余嵘 	    测试工程师      13133963918	 交际能力强，学习能力强，有耐心 

张佳顺 	软件工程师      19272698711 	动手能力强，主观能动性高

团队logo:![团队logo](团队相关介绍/团队logo.png)

用例图示范：![用例图](产品大致介绍/用例图.png)

目前构想的原型图：![原型](产品大致介绍/原型.png)

### 一.功能需求 

1 注册与登录 用户能够注册新账号以便切换数据模拟历史，或是登录管理员账号以便于管理气象数据和更新。

 2 数据查看 用户能够翻阅一定时间和地点的相应气象数据，包括温度、风向、湿度等等。

 3 数据分析 根据用户指定的数据能够进行算法分析，将分析后的图表等信息呈现于前端。 用户可以切换算法，进行多方面的数据勘察，得到更全面、更细节化分析结果来做出判断。 

4 模拟预测数据 用户可以指定城市和时间对未来的气象进行模拟预测，得到天气预报的结果，便于出行。 用户可以编撰数据进行模拟，实现对于各种气象下的预测研究。

 5 数据共享与实时更新 数据链接实时在线数据库，获得最新数据，使得用户能够更加精确的进行预测。 用户可以将更新数据与模拟数据对比分析误差，有助于算法优化和机器学习。

 6 服务器端实现 系统应提供服务器端实现，以实现客户端数据的存储。 服务器端应能够保存用户的模拟数据和账号信息。

###  二. 技术要求 

使用Qt界面开发框架，以实现跨平台的用户界面。

 使用文件I/O操作来保存和读取实时更新的气象数据。 

使用多线程处理来实现多用户在线模拟预测。 使用GDI图形处理库来绘制统计图表。 

使用MYSQL数据库来存储用户数据。 使用网络编程技术来实现客户端与服务器端之间的通信。 

### 三. 其他要求 

用户界面应该友好、直观，以便用户方便地操作和管理任务。 系统应具有良好的性能和稳定性，能够处理大量的任务和数据。 

系统应具备良好的安全性，确保用户的数据不会被未经授权的访问所泄露。

 用户界面应具有排列完好的各种选项，以便于用户对于各种算法进行选择，对于所有数据能够快捷搜索和调用。 

用户界面应具有良好的显示功能，以便于各种图表的显示。
### 四.注解
本项目中基于python开发的代码部分通过云服务器上的API接口实现与Qt项目的对接，因此在源代码库部分python代码与Qt项目略显隔离，实则已成功对接。

