# Engineer_2024_Embedding

## 写在前面

RoboWalker战队的工程机器人在区域赛的时候取得了不错的战绩，从区域赛开始基本上稳定单场黄金矿工，并且获得了区域赛东部赛区场均经济最高、单场经济最高。在复活赛，各个强队的工程机器人水平迎头赶上，而我们的工程机器人也出现了一些机械、电路上的战损情况，导致机器人在赛场上的表现有所下滑，不过每场的表现也还算可以接受。现开源工程机器人下位机代码，希望和各队伍多多交流。

上位机代码仓库：https://github.com/RoboWalker/Engineer2024_ROS2_Controller

自定义控制器代码仓库：https://github.com/RoboWalker/Engineer2024_Custom_Contoller

技术文档（完整形态考核版本）：https://github.com/RoboWalker/Engineer2024_Custom_Contoller/blob/main/%E4%B8%AD%E5%9B%BD%E7%A7%91%E5%AD%A6%E6%8A%80%E6%9C%AF%E5%A4%A7%E5%AD%A6RoboWalker_2024%E5%B7%A5%E7%A8%8B%E6%9C%BA%E5%99%A8%E4%BA%BA%E6%8A%80%E6%9C%AF%E6%96%87%E6%A1%A3.pdf

下位机代码功能并不复杂，主要是转发来自上位机的控制数据，一些简单的模式处理，和上位机挂掉时候的备用解算算法（到比赛结束也没有用到）。复活赛的时候写了一个“末端两电机翻转”的操作模式（`RobotArm_Mode_Reverse`），为了让机械臂能在某一侧的五级矿角度下伸的更远，但那个模式的bug可能没有找全，大家可以忽略掉相关的代码。

## 工具链

开发板为RoboMaster A板，使用`CubeMX`+`cmake`+`armgcc`+`openocd`工具链，使用`Clion`作为IDE开发。

Clion作为一个成熟的IDE，相比于Keil开发环境，具有代码补全功能丰富、编译超级快等显著优点，同时也可以在Linux系统上配置环境。

## 代码架构

代码主要构建在User目录下。代码层次为如下。

### 0-MWL(Middleware Layer)

主要是算法中间件，包括CRC校验、PID、自定义的一些数学库。

### 1-APL(Application Layer)

代码最顶层，包含轮询执行的函数，和中断回调函数。

### 2-FML(Function Module Layer)

包括底盘、云台、机械臂三个功能模块的类。相关控制函数在类中封装。

### 3-HDL(Hardware Driver Layer)

底层传感器、执行器等驱动代码，包括控制电机的代码、解析电机数据的代码、读取陀螺仪的代码等。每一个硬件驱动用类来封装。

### 4-HAL(Hardware Abstract Layer)

自己对stm32 HAL库中的CAN、UART等接口进行再封装。
