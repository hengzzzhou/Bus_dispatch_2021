# 概要设计书-----公交车调度系统

## 组员

丘晨晖 邢可原 周恒



## 输入输出

### 输入：

输入数据包括初始化数据和操作指令。
 初始化数据：在本情况中，初始化数据包括站点总数，每站之间的距离，和调度策略，可从文件中读取，如果文件中不存在该类型数据，则取全局变量，取默认值。
  操作指令：由用户通过键盘输入的数据。包括五种操作指令，分别为end、clock、counterclockwise、clockwise 和target。
  以下为五种指令的说明：
  End：结束指令，只在最后一行出现一次；
  Clock：时钟指令，每出现一次代表过了一秒钟；
  counterclockwise、clockwise、target为请求指令，如果它们出现，同一行内后边一定有一个整数。
  counterclockwise和clockwise：代表站台上的请求，后边的整数代表请求发生的站点号，counterclockwise表示逆时针方向，clockwise代表顺时针方向。
  Target：代表车厢内请求，后边的整数代表要去的站点号。

### 输出：

a：公交车运行动画的显示：环形公路，以及汽车运行过程和最终状态。	公交车在站点接送客户应在用户中明显体现出。
  b：输出每次clock后公交车的状态和站点状态，需要记录公交车的位置和目的地以及站点上的请求，包括正反方向的路线。
  概要设计：
  概要设计主要任务有两个，一是对用户界面进行设计，二是对系统进行模块划分，对全局数据结构和重要的算法进行设计。环形路口系统概要设计书如下：
  用户界面设计

  ![image-20231209131548946](http://hengg.oss-cn-beijing.aliyuncs.com/img/image-20231209131548946.png)

  

如图所示。
在此道路中，公交车可以向正负两个方向行驶，蓝色表示汽车，红色表示target（目的地），正常时站点为绿色，乘客为白色，上方为此时的运行方向。
如图为一种情况，为八个站点，此时汽车在一站点，目的地为七站点，而乘客在四站点。其中在此间过程中，每秒公交车会位移，图像也会相应的发生变化。

## 算法设计

自动机模型（状态转换图） ：
在本题中，我们要分析公交车和站点的状态以及其行为，在此我们用状态图来进行描述。
A：公交车状态图： 
由于我们有三种调度方案所以在此公交车状态有三种：

### a：FCFS（先来先服务）：

![image-20231209131632609](http://hengg.oss-cn-beijing.aliyuncs.com/img/image-20231209131632609.png)

公交车状态图

![image-20231209133443084](http://hengg.oss-cn-beijing.aliyuncs.com/img/image-20231209133443084.png)

行驶状态的细化
当收到乘客的要求时，公交车进入行驶状态，如果在此期间有乘客再发出请求后，则按顺序排列路线。如果到达目的站点后没有目的则在目的站点停留。本系统中不考 虑汽车熄火的问题，所以从暂停状态不会迁移到静止状态。
对于行驶状态可以进一步的细化成顺时针和逆时针。在其他三个状态下接 收到各命令时的状态转换类似，为保持图的简洁，此处就不再给出。

### b：SSTF（最短寻找时间优先）：

![image-20231209133500263](http://hengg.oss-cn-beijing.aliyuncs.com/img/image-20231209133500263.png)

在该情况中每次到达一次目的站点判断现有的最短时间优先策略，即使中途产生更优的请求也不可以更改。

### c：SCAN（顺便服务）：

![image-20231209133507905](http://hengg.oss-cn-beijing.aliyuncs.com/img/image-20231209133507905.png)

由于顺便服务的策略可看为对情况的额外补充，可以在其中判断新请求是否可以顺便服务（同方向的站台请求或车内请求），可以为新的请求停站。
B：站点状态图：

![image-20231209133516720](http://hengg.oss-cn-beijing.aliyuncs.com/img/image-20231209133516720.png)

站点只有有无乘客两种状态，当随着请求的出现而有乘客，无请求则无乘客，乘客上车后请求消除，站点转化为无乘客。

## 高层数据结构设计

常量定义：

```c
 //定义接近 0 的常量 
#define EXP 0.000001//
//公交车运行常量设置
#define clockwise 1 
#define static 0
#define anticlockwise -1 
```

全局变量定义：

```c
int TOTAL_STATION ；//总站点数目
char STRATEGY[5] ； //调度策略
int DISTANCE ；//每站之间的距离
int mind； //乘客对正反方向的选择
```

数据结构的定义：

```c
struct Bus {
int pos;
	int tar;
	int clock;
	int cclock;
	int apply[10][2];
};
```

其中包含公交车的位置（pos），与公交车的目的（tar），公交车顺时针去往的目的地（clock），公交车逆时针去往的目的地（cclock）。

## 系统模块划分

### 软件结构图

本系统程序部分划分为main.c ,input.c,streetDraw.c,busDraw.c,busControl.c
五个模块。各模块功能如下：
1．模块名称：main.c
模块功能简要描述：主函数，主要是运行各个界面绘画程序，运行计算机控制的汽
车的运行情况和用户控制的汽车运行情况（接收人工输入）。
2．模块名称：streetDraw.c
模块功能简要描述：绘制公交站台口界面。
3．模块名称busDraw.c
模块功能简要描述：在公交站台界面上绘制公交车，并控制公交车顺逆两方向移动。

4. 模块名称busControl.c 模块功能简要描述：
   （1）	根据顺逆两个方向检测用户的请求。
   （2）	根据用户请求用计算机控制的汽车,
   （3）	完成对计算机控制公交车的线程和用户请求的实现。

5. 模块名称input.c
   模块功能简要描述：完成对用户输入的线程的实现,根据输入改变汽车的状态。
   软件结构图如图2－34所示。

   ![image-20231209133615691](http://hengg.oss-cn-beijing.aliyuncs.com/img/image-20231209133615691.png)

文件及函数组成表2－2 文件及函数功能描述.

### 函数说明表

| 序号 | 函数原型                                   | 功能                                         | 参数                                          | 返回值                     |
| ---- | ------------------------------------------ | -------------------------------------------- | --------------------------------------------- | -------------------------- |
| 1    | int arrival()                              | 检测公交车是否到站。                         | void                                          | 如果到站返回1；否则返回0； |
| 2    | void  Printf(struct Bus bus);              | 打印出公交车目前的信息                       | struct Bus bus                                | 无                         |
| 3    | void checkAutoclock()                      | 检测顺时针运行的公交车下一个时刻是否变向     | void                                          | void                       |
| 4    | Void checkAutowiseclock()                  | 检测逆时针运行的公交车下一个时刻是否变向     | void                                          | void                       |
| 5    | void  checkAutoSTOP()                      | 检测停靠状态的公交车下一个时刻向哪个方向运行 | void                                          | void                       |
| 6    | void MoveNorth(struct  Car *ptr,double  l) | 控制顺时针移动汽车                           | ptr 是指向汽车的一个指针，l是车每次的移动距离 | void                       |
| 7    | void MoveSouth(struct  Car *ptr,double  l) | 控制逆时针移动汽车                           | ptr 是指向汽车的一个指针，l是车每次的移动距离 | void                       |
| 8    | void DrawSys()                             | 绘制公交车站                                 | void                                          | void                       |

### 代码示例

 高层算法设计下面给出该工程中几个高层算法的代码示例： 

```c
void ChooseFirstDir(Linklist *L) //按最新请求变向
{ 
    struct Request *temp = *L;
    temp = temp->link;
    if (temp == NULL) {
        bus.state = 0;
        return;
    }
        int juli = abs((temp->station - 1) * DISTANCE - bus.pos);

        if (juli >= TOTAL_STATION * DISTANCE / 2) {
            juli = TOTAL_STATION * DISTANCE - juli;
        }

        int z = (temp->station - 1) * DISTANCE - bus.pos;
            if ((temp->station - 1) * DISTANCE > bus.pos) {
                if ((temp->station - 1) * DISTANCE - bus.pos <= TOTAL_STATION * DISTANCE  / 2) {
                    bus.state = 1;
                } else {
                    bus.state = -1;
                }
            }

            if ((temp->station - 1) * DISTANCE < bus.pos) {
                if (bus.pos - (temp->station - 1) * DISTANCE >= TOTAL_STATION * DISTANCE / 2) {
                    bus.state = 1;
                } else {
                    bus.state = -1;
                }
            }
    return;
}   
```

