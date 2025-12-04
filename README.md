# 一个简单的函数绘图语言解释器

## 语法

- --表示注释，解释器会忽略注释内容到行尾
- 每条语句以分号;结尾
- 不区分大小写，大小写字母可以混用
- 左上角为坐标原点，坐标轴的方向为x轴向右，y轴向下
- ORIGIN IS (200, 200)：设置坐标原点为(200, 200)，默认值为(0, 0)，即坐标原点
- SCALE IS (100, 100)：设置坐标轴的长度为(100, 100)，默认值为(1, 1)
- ROT IS 0：设置旋转角度，默认为0
- for T from 0 to 2*Pi step pi/100 draw (cos(T), sin(T))：T从0到2*Pi，步长pi/100，绘制(cos(T),sin(T))
- origin, scale, rot按照origin->scale->rot的顺序生效
- 不设置origin, scale, rot时，使用默认值
- 重复设置origin, scale, rot会覆盖之前的值
- origin, scale, rot只对下一个for语句生效，for语句执行后，origin, scale, rot恢复默认值
- 支持指数运算**，和Python用法一样
- 支持sin,cos,tan,ln,exp,sqrt
- pi(3.14159265358979323846)和e(2.71828182845904523536)作为常数，可在表达式中使用

例子：

```
-- Draw a circle with radius 100 and center at (200, 200)
ORIGIN IS (200, 200);
SCALE IS (100, 100);
ROT is 0;
For T from 0 to 2*Pi step pi/100 draw (cos(T), sin(T));

-- Draw coordinate axes
ORIGIN IS (100, 300);
For T from 0 to 400 step 1 draw (0, -t); 
ORIGIN IS (100, 300);
For T from 0 to 400 step 1 draw (T, 0);

-- Draw sin function
ORIGIN IS (100, 300);
SCALE IS (2*pI, 50);
For T from -15 to 75 step pi/100 draw (t, sin(t));

-- Draw exponential function
ORIGIN IS (100, 300);
SCALE IS (100, -1);
For T from -0.9 to 5.9 step 0.02 draw (t, exp(t)+10);

-- Draw logarithmic function
ORIGIN IS (100, 300);
SCALE IS (1, -50);
For T from 0.000001 to 333 step 0.03 draw (t+10, ln(t));

-- Draw sqrt function
ORIGIN IS (100, 300);
SCALE IS (10, -20);
for T from 0 to 60 step 0.1 draw (T, sqrt(T));

-- Draw tangent function
ORIGIN IS (100, 300);
SCALE IS (50, -40);
For T from -1.2 to 11 step pi/300 draw (T, tan(T));

-- Draw a parabola that opens upward
ORIGIN IS (100, 300);
SCALE IS (100, 100);
ROT is pi;
For T from -10 to 20 step 0.01 draw (T,T**2);
```

## 编译

- 指定C++版本-std=c++17
- 可以加入-w关闭警告
- - 链接-lgdi32和-luser32
- 加入-mwindows关闭控制台窗口
- 加入debug信息-DDEBUG=n，数字越大信息越多，2达到最大值
- 其他优化选项比如-O2 -ffunction-sections -fdata-sections -Wl,--gc-sections -flto

e.g.

```powershell
g++ -std=c++17 FDLI.cpp FDLI/*.cpp -o FDLI.exe -lgdi32 -luser32
g++ -std=c++17 FDLI.cpp FDLI/*.cpp -o FDLI.exe -lgdi32 -luser32 -w -mwindows -DDEBUG=2 -O2 -ffunction-sections -fdata-sections "-Wl,--gc-sections" -flto
```

## 运行

- 双击运行，默认读取当前目录下的FDLI.txt文件作为输入，FDLI_log.txt作为输出
- 命令行运行./FDLI.exe 输入文件名 输出文件名指定输入输出文件
