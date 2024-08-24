/**
 * @file    Referee_System.h
 * @brief   大疆裁判系统驱动
 *
 * @author  Tang-yucheng
 * @date    2024-4-6 （创建）
 *
 */

#ifndef __HDL_REFEREE_SYSTEM_H
#define __HDL_REFEREE_SYSTEM_H

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include "User_Usart.h"
#include "Crc.h"

/* 宏定义 -------------------------------------------------------------------------------------------------------------*/
/*机器人ID*/
#define ROBOT_HERO_RED			1	    //红方英雄
#define ROBOT_ENGINEER_RED 		2	    //红方工程
#define ROBOT_INFANTRY_RED_1 	3	    //红方步兵1
#define	ROBOT_INFANTRY_RED_2	4	    //红方步兵2
#define	ROBOT_INFANTRY_RED_3	5	    //红方步兵3
#define ROBOT_FLY_RED			6	    //红方空中支援
#define ROBOT_SENTRY_RED		7	    //红方哨兵
#define ROBOT_DART_RED          8	    //红方飞镖
#define ROBOT_RADAR_RED			9	    //红方雷达站
#define ROBOT_OUTPOST_RED		10	    //红方前哨站
#define ROBOT_BASE_RED			11	    //红方基地

#define ROBOT_HERO_BLUE			101		//蓝方英雄
#define ROBOT_ENGINEER_BLUE 	102		//蓝方工程
#define ROBOT_INFANTRY_BLUE_1 	103		//蓝方步兵1
#define	ROBOT_INFANTRY_BLUE_2	104		//蓝方步兵2
#define	ROBOT_INFANTRY_BLUE_3	105		//蓝方步兵3
#define ROBOT_FLY_BLUE			106		//蓝方空中支援
#define ROBOT_SENTRY_BLUE		107		//红方哨兵
#define ROBOT_DART_BLUE         108		//蓝方飞镖
#define ROBOT_RADAR_BLUE		109		//红方雷达站
#define ROBOT_OUTPOST_BLUE		110		//蓝方前哨站
#define ROBOT_BASE_BLUE			111		//蓝方基地

/*客户端ID*/
#define CLIENT_HERO_RED			0X0101	//红方英雄操作手客户端
#define CLIENT_ENGINEER_RED 	0X0102	//红方工程操作手客户端
#define CLIENT_INFANTRY_RED_1 	0X0103	//红方步兵1操作手客户端
#define	CLIENT_INFANTRY_RED_2	0X0104	//红方步兵2操作手客户端
#define	CLIENT_INFANTRY_RED_3	0X0105	//红方步兵3操作手客户端
#define CLIENT_FLY_RED			0X0106	//红方空中操作手客户端

#define CLIENT_HERO_BLUE		0X0165	//蓝方英雄操作手客户端
#define CLIENT_ENGINEER_BLUE 	0X0166	//蓝方工程操作手客户端
#define CLIENT_INFANTRY_BLUE_1 	0X0167	//蓝方步兵1操作手客户端
#define	CLIENT_INFANTRY_BLUE_2	0X0168	//蓝方步兵2操作手客户端
#define	CLIENT_INFANTRY_BLUE_3	0X0169	//蓝方步兵3操作手客户端
#define CLIENT_FLY_BLUE			0X016A	//蓝方空中操作手客户端

#define REFEREE_SYSTEM_SERVER   0X8080  //裁判系统服务器

/*图形颜色*/
#define UI_COLOR_MAIN		0		//红蓝主色
#define UI_COLOR_YELLOW		1		//黄
#define UI_COLOR_GREEN		2		//绿
#define UI_COLOR_ORANGE		3		//橙
#define UI_COLOR_PURPLE		4		//紫
#define UI_COLOR_PINK		5		//粉
#define UI_COLOR_CYAN		6		//青
#define UI_COLOR_BLACK		7		//黑
#define UI_COLOR_WHITE		8		//白

/*图形操作*/
#define UI_GRAPHIC_NONE		0		//图形空操作
#define UI_GRAPHIC_ADD		1		//增加
#define UI_GRAPHIC_MODIFY	2		//修改
#define UI_GRAPHIC_DELETE	3		//删除

/*内容ID*/
#define UI_DATA_DELETE		0X100	//客户端删除图形
#define UI_DATA_DRAW_1		0X101	//客户端绘制一个图形
#define UI_DATA_DRAW_2		0X102	//客户端绘制二个图形
#define UI_DATA_DRAW_5		0X103	//客户端绘制五个图形
#define UI_DATA_DRAW_7		0X104	//客户端绘制七个图形
#define UI_DATA_DRAW_CHAR	0X110	//客户端绘制字符串

/* 结构体定义 ----------------------------------------------------------------------------------------------------------*/
/* 帧头 */
struct frame_header
{
    uint8_t SOF;
    uint16_t data_length;
    uint8_t seq;
    uint8_t CRC8;
} __attribute__((packed));

/* 比赛状态数据，固定 1Hz 频率发送 0x0001*/
struct game_status_t
{
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
    uint64_t SyncTimeStamp;
} __attribute__((packed));

/* 比赛结果数据，比赛结束触发发送 0x0002*/
struct game_result_t
{
    uint8_t winner;
} __attribute__((packed));

/* 机器人血量数据，固定 3Hz 频率发送 0x0003 */
struct game_robot_HP_t
{
    uint16_t red_1_robot_HP;
    uint16_t red_2_robot_HP;
    uint16_t red_3_robot_HP;
    uint16_t red_4_robot_HP;
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;
    uint16_t red_outpost_HP;
    uint16_t red_base_HP;
    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;
    uint16_t blue_3_robot_HP;
    uint16_t blue_4_robot_HP;
    uint16_t blue_5_robot_HP;
    uint16_t blue_7_robot_HP;
    uint16_t blue_outpost_HP;
    uint16_t blue_base_HP;
} __attribute__((packed));

/* 场地事件数据，固定 1Hz 频率发送 0x0101 */
struct event_data_t
{
    uint32_t event_data;
} __attribute__((packed));

/* 补给站动作标识数据，补给站弹丸释放时触发发送 0x0102 */
struct ext_supply_projectile_action_t
{
    uint8_t reserved;
    uint8_t supply_robot_id;
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
} __attribute__((packed));

/* 裁判警告数据，己方判罚/判负时触发发送，其余时间以 1Hz 频率发送 0x0104 */
struct referee_warning_t
{
    uint8_t level;
    uint8_t offending_robot_id;
    uint8_t count;
} __attribute__((packed));

/* 飞镖发射相关数据，固定 1Hz 频率发送 0x0105 */
struct dart_info_t
{
    uint8_t dart_remaining_time;
    uint8_t dart_aim_state;
} __attribute__((packed));

/* 机器人性能体系数据，固定10Hz 频率发送 0x0201*/
struct robot_status_t
{
    uint8_t robot_id;
    uint8_t robot_level;
    uint16_t current_HP;
    uint16_t maximum_HP;
    uint16_t shooter_barrel_cooling_value;
    uint16_t shooter_barrel_heat_limit;
    uint16_t chassis_power_limit;
    uint8_t power_management_gimbal_output : 1;
    uint8_t power_management_chassis_output : 1;
    uint8_t power_management_shooter_output : 1;
} __attribute__((packed));

/* 实时功率热量数据，固定 50Hz频率发送 0x0202*/
struct power_heat_data_t
{
    uint16_t chassis_voltage;
    uint16_t chassis_current;
    float chassis_power;
    uint16_t buffer_energy;
    uint16_t shooter_17mm_1_barrel_heat;
    uint16_t shooter_17mm_2_barrel_heat;
    uint16_t shooter_42mm_barrel_heat;
} __attribute__((packed));

/* 机器人位置数据，固定以 1Hz 频率发送 0x0203 */
struct robot_pos_t
{
    float x;
    float y;
    float angle;
} __attribute__((packed));

/* 机器人增益数据，固定 3Hz 频率发送 0x0204 */
struct buff_t
{
    uint8_t recovery_buff;
    uint8_t cooling_buff;
    uint8_t defence_buff;
    uint16_t attack_buff;
} __attribute__((packed));

/* 空中支援时间数据，固定 1Hz 频率发送 0x0205*/
struct air_support_data_t
{
    uint8_t airforce_status;
    uint8_t time_remain;
} __attribute__((packed));

/* 伤害状态数据，伤害发生后发送 0x0206 */
struct hurt_data_t
{
    uint8_t armor_id : 4;
    uint8_t HP_deduction_reason : 4;
} __attribute__((packed));

/* 实时射击数据，弹丸发射后发送 0x0207 */
struct shoot_data_t
{
    uint8_t bullet_type;
    uint8_t shooter_number;
    uint8_t launching_frequency;
    float initial_speed;
} __attribute__((packed));

/* 允许发弹量，固定 10Hz 频率发送 0x0208 */
struct projectile_allowance_t
{
    uint16_t projectile_allowance_17mm;
    uint16_t projectile_allowance_42mm;
    uint16_t remaining_gold_coin;
} __attribute__((packed));

/* 机器人 RFID 状态，固定 3Hz 频率发送 0x0209 */
struct rfid_status_t
{
    uint32_t rfid_status;
} __attribute__((packed));

/* 飞镖选手端指令数据，飞镖闸门上线后固定 3Hz 频率发送 0x020A */
struct dart_client_cmd_t
{
    uint8_t dart_launch_opening_status;
    uint8_t  reserved;
    uint16_t target_change_time;
    uint16_t latest_launch_cmd_time;
} __attribute__((packed));

/* 地面机器人位置数据，固定 1Hz频率发送 0x020B */
struct ground_robot_position_t
{
    float hero_x;
    float hero_y;
    float engineer_x;
    float engineer_y;
    float standard_3_x;
    float standard_3_y;
    float standard_4_x;
    float standard_4_y;
    float standard_5_x;
    float standard_5_y;
} __attribute__((packed));

/* 雷达标记进度数据，固定 1Hz 频率发送 0x020C */
struct radar_mark_data_t
{
    uint8_t mark_hero_progress;
    uint8_t mark_engineer_progress;
    uint8_t mark_standard_3_progress;
    uint8_t mark_standard_4_progress;
    uint8_t mark_standard_5_progress;
    uint8_t mark_sentry_progress;
} __attribute__((packed));

/* 哨兵自主决策相关信息同步，固定 1Hz 频率发送 0x020D */
struct sentry_info_t
{
    uint32_t sentry_info;
} __attribute__((packed));

/* 雷达自主决策信息同步，固定1Hz 频率发送 0x020E */
struct radar_info_t
{
    uint8_t radar_info;
} __attribute__((packed));

/* 机器人交互数据，发送方触发发送，频率上限为 10Hz 0x0301 */
struct robot_interaction_data_t
{
    uint16_t data_cmd_id;
    uint16_t sender_id;
    uint16_t receiver_id;
    uint8_t user_data[113];
} __attribute__((packed));

//0x0100 x=2 选手端删除图层
struct interaction_layer_delete_t
{
    uint8_t delete_type;
    uint8_t layer;
} __attribute__((packed));

//0x0101 x=15 选手端绘制一个图形
struct interaction_figure_t
{
    uint8_t figure_name[3];
    uint32_t operate_tpye:3;
    uint32_t figure_tpye:3;
    uint32_t layer:4;
    uint32_t color:4;
    uint32_t details_a:9;
    uint32_t details_b:9;
    uint32_t width:10;
    uint32_t start_x:11;
    uint32_t start_y:11;
    uint32_t details_c:10;
    uint32_t details_d:11;
    uint32_t details_e:11;
} __attribute__((packed));

//0x0102 x=30 选手端绘制两个图形
struct interaction_figure_2_t
{
    interaction_figure_t interaction_figure[2];
} __attribute__((packed));

//0x0103 x=75 选手端绘制五个图形
struct interaction_figure_3_t
{
    interaction_figure_t interaction_figure[5];
} __attribute__((packed));

//0x0104 x=105 选手端绘制七个图形
struct interaction_figure_4_t
{
    interaction_figure_t interaction_figure[7];
} __attribute__((packed));

//0x0110 x=45 选手端绘制字符图形
struct ext_client_custom_character_t
{
    interaction_figure_t grapic_data_struct;
    uint8_t data[30];
} __attribute__((packed));

//0x0120 x=4 哨兵自主决策指令
struct sentry_cmd_t
{
    uint32_t sentry_cmd;
} __attribute__((packed));

//0x0121 x=1 雷达自主决策指令
struct radar_cmd_t
{
    uint8_t radar_cmd;
} __attribute__((packed));

/* 自定义控制器与机器人交互数据，发送方触发发送，频率上限为 30Hz 0x0302 */
struct custom_robot_data_t
{
    uint8_t data[30];
} __attribute__((packed));

/* 选手端小地图交互数据，选手端触发发送 0x0303 */
struct map_command_t
{
    float target_position_x;
    float target_position_y;
    uint8_t cmd_keyboard;
    uint8_t target_robot_id;
    uint8_t cmd_source;
} __attribute__((packed));

/* 键鼠遥控数据，固定 30Hz 频率发送 0x0304 */
struct remote_control_t
{
    int16_t mouse_x;
    int16_t mouse_y;
    int16_t mouse_z;
    int8_t left_button_down;
    int8_t right_button_down;
    uint16_t keyboard_value;
    uint16_t reserved;
} __attribute__((packed));

/* 选手端小地图接收雷达数据，频率上限为 10Hz 0x0305 */
struct map_robot_data_t
{
    uint16_t target_robot_id;
    float target_position_x;
    float target_position_y;
} __attribute__((packed));

/* 自定义控制器与选手端交互数据，发送方触发发送，频率上限为 30Hz 0x0306 */
struct custom_client_data_t
{
    uint16_t key_value;
    uint16_t x_position:12;
    uint16_t mouse_left:4;
    uint16_t y_position:12;
    uint16_t mouse_right:4;
    uint16_t reserved;
} __attribute__((packed));

/* 选手端小地图接收哨兵数据，频率上限为 1Hz 0x0307 */
struct map_data_t
{
    uint8_t intention;
    uint16_t start_position_x;
    uint16_t start_position_y;
    int8_t delta_x[49];
    int8_t delta_y[49];
    uint16_t sender_id;
} __attribute__((packed));

/* 选手端小地图接收机器人消息，频率上限为 3Hz 0x0308 */
struct custom_info_t
{
    uint16_t sender_id;
    uint16_t receiver_id;
    uint16_t user_data[30];
} __attribute__((packed));

/* 类定义 -------------------------------------------------------------------------------------------------------------*/
/**
 * @brief 常规链路类
 */
class Class_Referee_System
{
public:
    /* 函数 */
    void Init(Struct_UART_Manage_Object * __UART_Manage_Obj);

    void Layer_operations(interaction_layer_delete_t * interaction_layer_delete,uint8_t layer_id,uint8_t operate_type);//图层操作

    void Draw_line(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y);//绘制直线
    void Draw_rectangle(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y);//绘制矩形
    void Draw_circle(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t radius);//绘制圆形
    void Draw_ellipse(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t length_xaxis,uint16_t length_yaxis);//绘制椭圆
    void Draw_arc(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t start_angle,uint16_t end_angle,uint16_t length_xaxis,uint16_t length_yaxis);//绘制圆弧
    void Draw_float(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t font_size,float data_draw);//绘制浮点数
    void Draw_int(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t font_size,int32_t data_draw);//绘制整数
    void Draw_string(ext_client_custom_character_t *client_custom_character,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t font_size,uint8_t* data_draw,uint16_t CHAR_length);//绘制字符串

    void Referee_data_processing(uint8_t data[],uint16_t length);//裁判系统数据处理

    frame_header Frame_header;                          //帧头
    uint16_t cmd_id;                                    //命令ID

    game_status_t game_status;                          //比赛状态数据 0x0001
    game_result_t game_result;                          //比赛结果数据 0x0002
    game_robot_HP_t game_robot_HP;                      //机器人血量数据 0x0003
    event_data_t event_data;                            //场地事件数据 0x0101
    ext_supply_projectile_action_t ext_supply_projectile_action;//补给站动作标识数据 0x0102
    referee_warning_t referee_warning;                  //裁判警告数据 0x0104
    dart_info_t dart_info;                              //飞镖发射相关数据    0x0105
    robot_status_t robot_status;                        //机器人性能体系数据 0x0201
    power_heat_data_t power_heat_data;                  //实时功率热量数据 0x0202
    robot_pos_t robot_pos;                              //机器人位置数据 0x0203
    buff_t buff;                                        //机器人增益数据 0x0204
    air_support_data_t air_support_data;                //空中支援时间数据 0x0205
    hurt_data_t hurt_data;                              //伤害状态数据 0x0206
    shoot_data_t shoot_data;                            //实时射击数据 0x0207
    projectile_allowance_t projectile_allowance;        //允许发弹量 0x0208
    rfid_status_t rfid_status;                          //机器人 RFID 状态 0x0209
    dart_client_cmd_t dart_client_cmd;                  //飞镖选手端指令数据 0x020A
    ground_robot_position_t ground_robot_position;      //地面机器人位置数据 0x020B
    radar_mark_data_t radar_mark_data;                  //雷达标记进度数据 0x020C
    sentry_info_t sentry_info;                          //哨兵自主决策相关信息同步 0x020D
    radar_info_t radar_info;                            //雷达自主决策信息同步 0x020E
    robot_interaction_data_t robot_interaction_data;    //机器人交互数据 0x0301

    map_command_t map_command;                          //选手端小地图交互数据 0x0303
    map_robot_data_t map_robot_data;                    //选手端小地图接收雷达数据 0x0305
    custom_client_data_t custom_client_data;            //自定义控制器与选手端交互数据 0x0306
    map_data_t map_data;                                //选手端小地图接收哨兵数据 0x0307
    custom_info_t custom_info;                          //选手端小地图接收机器人消息 0x0308

    uint8_t crc8_check;                                 //CRC8校验,1为成功，0为失败
    uint8_t crc16_check;                                //CRC16校验,1为成功，0为失败
private:
    Struct_UART_Manage_Object * UART_Mangae_Obj;        /*!< 绑定的UART */
};

/**
 * @brief 图传链路类
 */
class Class_video_transmission_link
{
public:
    frame_header Frame_header;                      //帧头
    uint16_t cmd_id;                                //命令ID

    custom_robot_data_t custom_robot_data;          //自定义控制器与机器人交互数据 0x0302
    remote_control_t remote_control;                //键鼠遥控数据 0x0304
private:
    Struct_UART_Manage_Object * UART_Mangae_Obj;    /*!< 绑定的UART */
};

/* 函数声明 ------------------------------------------------------------------------------------------------------------*/
int16_t Referee_data_transmit(uint16_t Child_content_ID, uint16_t Cilent_ID, uint16_t Robot_ID, uint8_t user_tx_data[]);

/* 变量声明 ------------------------------------------------------------------------------------------------------------*/
extern Class_Referee_System Referee_System;

#endif /* HDL_Referee_System */
