# Huawei Competition

# Second Round

## TODO
+ 多线程？？？来得及学习嘛
+ 探测数据，猜一波最终数据变化不大？
+ 改进算法吧，算法太慢了！！！
  + AC 3+4
  + AC 存答案时直接存字符串，这样就省了输出时将int型答案转成char[]的麻烦 
+ AC 拓扑排序删掉入度为0的点
+ AC 直接在初赛方案上面加上金额约束试试，先找环，找完环后再根据金额约束去掉一些环
## Score
### 10号前
1. 91 [vector+ans_t](./version/vector+ans_t.cpp)
2. 其余方案TLE
### 10-15
+ dfs: 4+3（正走四步倒走三步）比3+4快
1. 699 [vector+ans_t](./version/vector+ans_t.cpp)
2. result incorrect [vector+ans34567](./version/vector+ans34567.cpp)
3. 785 [vector+ans_t+拓扑排序+除法改乘法](./version/vector+ans_t.cpp)
4. 296 [vector+ans34567](./version/vector+ans34567.cpp)
5. 138 [search_first bag c5336c6ba76b47f884c6f191344f0aca965abe4b bag](./version/search_first.cpp)
6. 157 [search_first bag+char[]ans ed203e72206b3d3f6a8cb0560c63edbb7f69170c ](./version/search_first.cpp)
7. 138 [search_first_c5336c6ba76b47f884c6f191344f0aca965abe4b](./version/search_first.cpp)
8. 206 [baseline 3670070ac4cf4db14ec8bf3aca1057d4cd8a06b6](./SecondRound/baseline.cpp)
9. 152 [multithread+bag2+vector34567 433ec0375c797c394c1a389099070c0b47f0ad6c](./version/multithread+bag2+vector34567.cpp)
## data
### 10-15
+ node_size<=29W
+ 出入度应该很大，菊花图
+ 答案环数在1900W左右
## analysis
+ 先把离散化加回来，因为不确定数据范围，ID是否<=INF
+ 最大环数2000万，线上数据有待测试
+ 转账金额约束
## diff with first round
+ 会保证满足条件的循环转账个数小于2000万。
+ 转账记录最多为200万条
+ 转账金额浮动区间约束为[0.2, 3] ：循环转账的前后路径的转账金额浮动，不能小于0.2，不能大于3。如账户A转给账户B 转X元，账户B转给账户C转Y元， 那么进行循环检测时，要满足0.2 <= Y/X <= 3的条件。
+ ID为32位的无符号整数，转账金额为32位的正整数，且都小于2的31次方，选手可以不用考虑范围之外的输入。（转账金额不为0的意思，但所有数据依然为int32范围）
+ 去除了平均转账记录数< 10的说明
+ 每天3次，AB榜。最后一天下午更换数据集，且对题目进行调整（喵喵喵？？？），可提交5次
+ 禁止直接根据ID和权重的取值排除部分数据，禁止跳过文件中的部分数据

## Final data
+ 路径长度[3,8]
+ 转账金额为浮点数
## resources
+ https://zhuanlan.zhihu.com/p/125764650
+ https://blog.csdn.net/qq_34914551/article/details/105788200
+ https://zhuanlan.zhihu.com/p/136785097
