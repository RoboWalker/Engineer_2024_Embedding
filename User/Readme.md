# RM2024-工程

- <font size=4>**代码架构**</font>
    - **MWL 中间件层 (Middleware Layer)**

            一些高层次（脱离硬件）可复用组件。
            e.g. 算法实现，数据结构实现...

    - **APL 应用层 (Application Layer)**

            实现程序执行逻辑。  
            e.g. 主函数实现（初始化、循环），回调函数实现。

    - **FML 功能模块层 (Function Module Layer)**

            通过调用 HDL、HAL 层封装的外设接口，组合成对应功能模块，实现相应功能。   
            e.g. 底盘功能模块实现，云台功能模块实现。

    - **HDL 硬件驱动层 (Hardware Driver Layer)**

            通过调用 HAL 层封装片上外设接口，实现对片外外设驱动。  
            e.g. 板级支持包实现，电机驱动实现...
        - Motor_DJI (大疆电机驱动)
        - Motor_DM (达妙电机驱动)

    - **HAL 硬件抽象层 (Hardware Abstract Layer)**

            实现对于片上外设再次封装。  
            e.g. CAN外设封装，USART外设封装...