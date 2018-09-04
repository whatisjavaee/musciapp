import tensorflow as tf
import time as time
import numpy as np
import math as m
# 钢琴88 键盘 每个样本 1024个输入
# 10000个随机样本
y = np.random.rand(10000, 1).astype(np.float32)  # 随机输入
for i in range(0, 10000):
    # 随机琴键 0 -87
    y[i][0] = i % 88 + 1.0;
x = np.random.rand(10000, 1024).astype(np.float32)
for i in range (0, 10000):
    # 时序 1024个点 模拟采样频率 44100 每个样点间隔 1/44100 随机起始时间 r
    r = np.random.rand(1)
    for j in range (0, 1024):
        # 频率= w/2PI
        x[i][j] = np.sin((j * (1 / 44100) + r) * (2 * m.pi) * (y[i] * 20))
# 钢琴88 键盘 每个样本 1024个输入
y_test = np.random.rand(88, 1).astype(np.float32)  # 随机输入
for i in range(0, 88):
    y_test[i][0] = i + 1;
x_test = np.random.rand(88, 1024).astype(np.float32)
for i in range (0, 88):
    # 时序 1024个点 模拟采样频率 44100 每个样点间隔 1/44100 随机起始时间
    r = np.random.rand(1)
    for j in range (0, 1024):
        # 频率= w/2PI
        x_test[i][j] = np.sin((j * (1 / 44100) + r) * (2 * m.pi) * (y_test[i] * 20))


def add_layer(inputs, in_size, out_size, activation_function=None):
    Weights = tf.Variable(tf.random_normal([in_size, out_size]))
    biases = tf.Variable(tf.zeros([1, out_size]) + 0.1)
    Wx_plus_b = tf.matmul(inputs, Weights) + biases
    if activation_function is None:
        outputs = Wx_plus_b
    else:
        outputs = activation_function(Wx_plus_b)
    return outputs


xs = tf.placeholder(tf.float32, [None, 1024])
ys = tf.placeholder(tf.float32, [None, 1])
l1 = add_layer(xs, 1024, 512, activation_function=tf.nn.sigmoid)

prediction = add_layer(l1, 512, 1, activation_function=None)
loss = tf.reduce_sum(tf.abs(prediction - ys))
optimizer = tf.train.GradientDescentOptimizer(0.000001)
train = optimizer.minimize(loss)

# 初始化变量
init = tf.global_variables_initializer()

# 启动图 (graph)
sess = tf.Session()
sess.run(init)
start = time.time();
# 拟合平面
# for step in range(0, 1000000000):
step = 0;
while True:    
    r = step % 100
    sess.run(train, feed_dict={xs: x[r:(r + 1) * 100], ys: y[r:(r + 1) * 100]})
    if step % 100 == 0:
        print(
            "step:",
            step,
            "time:",
            time.time() - start,
            # "prediction:", sess.run(prediction, feed_dict={xs: x[r:(r + 1)*10], ys: y[r:(r + 1)*10]}),
            # "y:", y[r:r + 1],
            "loss", sess.run(loss, feed_dict={xs: x[r:(r + 1) * 100], ys: y[r:(r + 1) * 100]}) / 100,
            # "prediction:", sess.run(prediction, feed_dict={xs: x_test, ys:y_test}),
            # "y:", y_test,
            "loss", sess.run(loss / 88, feed_dict={xs: x_test, ys:y_test}))

    step = step + 1;
