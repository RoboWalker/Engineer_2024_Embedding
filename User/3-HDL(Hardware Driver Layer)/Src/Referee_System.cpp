/**
 * @file    Referee_System.cpp
 * @brief   大疆裁判系统驱动
 *
 * @author  Tang-yucheng
 * @date    2024-4-6 （创建）
 *
 */

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "Referee_System.h"

Class_Referee_System Referee_System;

/* 函数定义 -----------------------------------------------------------------------------------------------------------*/
/**
 * @brief 初始化函数
 * @note
 *
 * @param
 * @return
 */
void Class_Referee_System::Init(Struct_UART_Manage_Object * __UART_Manage_Obj)
{
    this->UART_Mangae_Obj = __UART_Manage_Obj;
    UART_Init(this->UART_Mangae_Obj, 512);
}

/**
 * @brief 图层操作
 * @note
 *
 * @param interaction_layer_delete
 * @param layer 图层号 0-9
 * @param operation 操作 0：空操作，1：删除图层，2：删除所有
 * @return
 */
void Class_Referee_System::Layer_operations(interaction_layer_delete_t *interaction_layer_delete,uint8_t layer, uint8_t operation)
{
    interaction_layer_delete->layer = layer;
    interaction_layer_delete->delete_type = operation;
}

/**
 * @brief 绘制直线
 * @note
 *
 * @param name[] 在图形删除，修改等操作中，作为索引
 * @param operate_type 操作类型 0：空操作，1： 增加图形，2：修改图形，3：删除图形
 * @param layer_ID 图层号 0-9
 * @param color_ID 颜色
 * @param line_width 线宽
 * @param start_x 起点x坐标
 * @param start_y 起点y坐标
 * @param end_x 终点x坐标
 * @param end_y 终点y坐标
 * @return
 */
void Class_Referee_System::Draw_line(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y)
{
    memcpy(interaction_figure->figure_name,name,3);
    interaction_figure->operate_tpye = operate_type;
    interaction_figure->figure_tpye = 0;
    interaction_figure->layer = layer_ID;
    interaction_figure->color = color_ID;
    interaction_figure->width = line_width;
    interaction_figure->start_x = start_x;
    interaction_figure->start_y = start_y;
    interaction_figure->details_d = end_x;
    interaction_figure->details_e = end_y;
}

/**
 * @brief 绘制矩形
 * @note
 *
 * @param name[] 在图形删除，修改等操作中，作为索引
 * @param operate_type 操作类型 0：空操作，1： 增加图形，2：修改图形，3：删除图形
 * @param layer_ID 图层号 0-9
 * @param color_ID 颜色
 * @param line_width 线宽
 * @param start_x 起点x坐标
 * @param start_y 起点y坐标
 * @param end_x 终点x坐标
 * @param end_y 终点y坐标
 * @return
 */
void Class_Referee_System::Draw_rectangle(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y)
{
    memcpy(interaction_figure->figure_name,name,3);
    interaction_figure->operate_tpye = operate_type;
    interaction_figure->figure_tpye = 1;
    interaction_figure->layer = layer_ID;
    interaction_figure->color = color_ID;
    interaction_figure->width = line_width;
    interaction_figure->start_x = start_x;
    interaction_figure->start_y = start_y;
    interaction_figure->details_d = end_x;
    interaction_figure->details_e = end_y;
}

/**
 * @brief 绘制圆形
 * @note
 *
 * @param
 * @return
 */
void Class_Referee_System::Draw_circle(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t radius)
{
    memcpy(interaction_figure->figure_name,name,3);
    interaction_figure->operate_tpye = operate_type;
    interaction_figure->figure_tpye = 2;
    interaction_figure->layer = layer_ID;
    interaction_figure->color = color_ID;
    interaction_figure->width = line_width;
    interaction_figure->start_x = start_x;
    interaction_figure->start_y = start_y;
    interaction_figure->details_c = radius;
}

/**
 * @brief 绘制椭圆
 * @note
 *
 * @param name[] 在图形删除，修改等操作中，作为索引
 * @param operate_type 操作类型 0：空操作，1： 增加图形，2：修改图形，3：删除图形
 * @param layer_ID 图层号 0-9
 * @param color_ID 颜色
 * @param line_width 线宽
 * @param start_x 起点x坐标
 * @param start_y 起点y坐标
 * @param length_xaxis x半轴长度
 * @param length_yaxis y半轴长度
 * @return
 */
void Class_Referee_System::Draw_ellipse(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t length_xaxis,uint16_t length_yaxis)
{
    memcpy(interaction_figure->figure_name,name,3);
    interaction_figure->operate_tpye = operate_type;
    interaction_figure->figure_tpye = 3;
    interaction_figure->layer = layer_ID;
    interaction_figure->color = color_ID;
    interaction_figure->width = line_width;
    interaction_figure->start_x = start_x;
    interaction_figure->start_y = start_y;
    interaction_figure->details_d = length_xaxis;
    interaction_figure->details_e = length_yaxis;
}

/**
 * @brief 绘制圆弧
 * @note
 *
 * @param name[] 在图形删除，修改等操作中，作为索引
 * @param operate_type 操作类型 0：空操作，1： 增加图形，2：修改图形，3：删除图形
 * @param layer_ID 图层号 0-9
 * @param color_ID 颜色
 * @param line_width 线宽
 * @param start_x 起点x坐标
 * @param start_y 起点y坐标
 * @param start_angle 起始角度
 * @param end_angle 终止角度
 * @param length_xaxis x半轴长度
 * @param length_yaxis y半轴长度
 * @return
 */
void Class_Referee_System::Draw_arc(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t start_angle,uint16_t end_angle,uint16_t length_xaxis,uint16_t length_yaxis)
{
    memcpy(interaction_figure->figure_name,name,3);
    interaction_figure->operate_tpye = operate_type;
    interaction_figure->figure_tpye = 4;
    interaction_figure->layer = layer_ID;
    interaction_figure->color = color_ID;
    interaction_figure->details_a = start_angle;
    interaction_figure->details_b = end_angle;
    interaction_figure->width = line_width;
    interaction_figure->start_x = start_x;
    interaction_figure->start_y = start_y;
    interaction_figure->details_d = length_xaxis;
    interaction_figure->details_e = length_yaxis;
}

/**
 * @brief 绘制浮点数
 * @note
 *
 * @param name[] 在图形删除，修改等操作中，作为索引
 * @param operate_type 操作类型 0：空操作，1： 增加图形，2：修改图形，3：删除图形
 * @param layer_ID 图层号 0-9
 * @param color_ID 颜色
 * @param line_width 线宽
 * @param start_x 起点x坐标
 * @param start_y 起点y坐标
 * @param font_size 字体大小
 * @param data_draw 要绘制的浮点数
 * @return
 */
void Class_Referee_System::Draw_float(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t font_size,float data_draw)
{
    int32_t data_draw_int;
    data_draw_int = (int32_t)(data_draw*1000);
    uint8_t* Float_ptr = (uint8_t* )interaction_figure;
    memcpy(interaction_figure->figure_name,name,3);
    interaction_figure->operate_tpye = operate_type;
    interaction_figure->figure_tpye = 5;
    interaction_figure->layer = layer_ID;
    interaction_figure->color = color_ID;
    interaction_figure->width = line_width;
    interaction_figure->start_x = start_x;
    interaction_figure->start_y = start_y;
    interaction_figure->details_a = font_size;
    memcpy(Float_ptr+11,&data_draw_int,4);
}

/**
 * @brief 绘制整型数
 * @note
 *
 * @param name[] 在图形删除，修改等操作中，作为索引
 * @param operate_type 操作类型 0：空操作，1： 增加图形，2：修改图形，3：删除图形
 * @param layer_ID 图层号 0-9
 * @param color_ID 颜色
 * @param line_width 线宽
 * @param start_x 起点x坐标
 * @param start_y 起点y坐标
 * @param font_size 字体大小
 * @param data_draw 要绘制的整型数
 * @return
 */
void Class_Referee_System::Draw_int(interaction_figure_t *interaction_figure,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t font_size,int32_t data_draw)
{
    uint8_t* Int_ptr = (uint8_t* )interaction_figure;
    memcpy(interaction_figure->figure_name,name,3);
    interaction_figure->operate_tpye = operate_type;
    interaction_figure->figure_tpye = 6;
    interaction_figure->layer = layer_ID;
    interaction_figure->color = color_ID;
    interaction_figure->width = line_width;
    interaction_figure->start_x = start_x;
    interaction_figure->start_y = start_y;
    interaction_figure->details_a = font_size;
    memcpy(Int_ptr+11,&data_draw,4);
}

/**
 * @brief 绘制字符串
 * @note
 *
 * @param name[] 在图形删除，修改等操作中，作为索引
 * @param operate_type 操作类型 0：空操作，1： 增加图形，2：修改图形，3：删除图形
 * @param layer_ID 图层号 0-9
 * @param color_ID 颜色
 * @param line_width 线宽
 * @param start_x 起点x坐标
 * @param start_y 起点y坐标
 * @param font_size 字体大小
 * @param data_draw 要绘制的字符串
 * @return
 */
void Class_Referee_System::Draw_string(ext_client_custom_character_t *client_custom_character,uint8_t name[],uint8_t operate_type,uint8_t layer_ID,uint8_t color_ID,uint16_t line_width,uint16_t start_x,uint16_t start_y,uint16_t font_size,uint8_t* data_draw,uint16_t CHAR_length)
{
    memcpy(client_custom_character->grapic_data_struct.figure_name,name,3);
    client_custom_character->grapic_data_struct.operate_tpye = operate_type;
    client_custom_character->grapic_data_struct.figure_tpye = 7;
    client_custom_character->grapic_data_struct.layer = layer_ID;
    client_custom_character->grapic_data_struct.color = color_ID;
    client_custom_character->grapic_data_struct.width = line_width;
    client_custom_character->grapic_data_struct.start_x = start_x;
    client_custom_character->grapic_data_struct.start_y = start_y;
    client_custom_character->grapic_data_struct.details_a = font_size;
    client_custom_character->grapic_data_struct.details_b = CHAR_length;
    memcpy(client_custom_character->data,data_draw,CHAR_length);
}

/**
 * @brief 裁判系统数据处理
 * @note
 *
 * @param
 * @return
 */
void Class_Referee_System::Referee_data_processing(uint8_t data[], uint16_t length)
{
    auto data_origin = data;

    while((data - data_origin) < 500)//截取500个字节
    {
        if(*data == 0xA5)//寻找帧头
        {
            memcpy(&Frame_header, data, 5);
            memcpy(&cmd_id, data + 5, 2);
            crc8_check = Verify_CRC8_Check_Sum(data, 5);
            crc16_check = Verify_CRC16_Check_Sum(data, Frame_header.data_length + 9);
            data += 7;
            if((Frame_header.SOF == 0xA5) && (crc8_check == 1) && (crc16_check == 1))
            {
                switch (cmd_id)
                {
                    case 0x0001://比赛状态数据
                        memcpy(&game_status, data, Frame_header.data_length);
                        break;
                    case 0x0002://场地事件数据
                        memcpy(&game_result, data, Frame_header.data_length);
                        break;
                    case 0x0003://机器人血量数据
                        memcpy(&game_robot_HP, data, Frame_header.data_length);
                        break;
                    case 0x0101://场地事件数据
                        memcpy(&event_data, data, Frame_header.data_length);
                        break;
                    case 0x0102://补给站动作标识数据
                        memcpy(&ext_supply_projectile_action, data, Frame_header.data_length);
                        break;
                    case 0x0104://裁判警告数据
                        memcpy(&referee_warning, data, Frame_header.data_length);
                        break;
                    case 0x0105://飞镖发射相关数据
                        memcpy(&dart_info, data, Frame_header.data_length);
                        break;
                    case 0x0201://机器人性能体系数据
                        memcpy(&robot_status, data, Frame_header.data_length);
                        break;
                    case 0x0202://实时功率热量数据
                        memcpy(&power_heat_data, data, Frame_header.data_length);
                        break;
                    case 0x0203://机器人位置数据
                        memcpy(&robot_pos, data, Frame_header.data_length);
                        break;
                    case 0x0204://机器人增益数据
                        memcpy(&buff, data, Frame_header.data_length);
                        break;
                    case 0x0205://空中支援时间数据
                        memcpy(&air_support_data, data, Frame_header.data_length);
                        break;
                    case 0x0206://伤害状态数据
                        memcpy(&hurt_data, data, Frame_header.data_length);
                        break;
                    case 0x0207://实时射击信息数据
                        memcpy(&shoot_data, data, Frame_header.data_length);
                        break;
                    case 0x0208://允许发弹量
                        memcpy(&projectile_allowance, data, Frame_header.data_length);
                        break;
                    case 0x0209://机器人 RFID 状态
                        memcpy(&rfid_status, data, Frame_header.data_length);
                        break;
                    case 0x020A://飞镖选手端指令数据
                        memcpy(&dart_client_cmd, data, Frame_header.data_length);
                        break;
                    case 0x020B://地面机器人位置数据
                        memcpy(&ground_robot_position, data, Frame_header.data_length);
                        break;
                    case 0x020C://雷达标记进度数据
                        memcpy(&radar_mark_data, data, Frame_header.data_length);
                        break;
                    case 0x020D://哨兵自主决策相关信息同步
                        memcpy(&sentry_info, data, Frame_header.data_length);
                        break;
                    case 0x020E://雷达自主决策信息同步
                        memcpy(&radar_info, data, Frame_header.data_length);
                        break;
                    case 0x0301://机器人交互数据
                        memcpy(&robot_interaction_data, data, Frame_header.data_length);
                        break;
                    default:
                        break;
                }
            }
            data += Frame_header.data_length + 2;
        }
        else data++;
    }
}

/**
 * @brief 数据发送
 * @note 不支持机器人间通信
 *
 * @param
 * @return
 */
uint8_t data_seq;//数据包序号
uint8_t data_send_buf[130];//发送数据缓存
int16_t Referee_data_transmit(uint16_t Child_content_ID,uint16_t Cilent_ID,uint16_t Robot_ID,uint8_t user_tx_data[])
{
    uint16_t CMD_ID = 0x0301;
    uint8_t *frame_point;//读写指针
    uint16_t frametail = 0xFFFF;
    uint16_t data_length;
    robot_interaction_data_t robot_interaction_data_send;//交互数据接收信息

    switch (Child_content_ID)
    {
        case 0x0100://选手端删除图层
            data_length = 2+6;
            break;
        case 0x0101://选手端绘制一个图形
            data_length = 15+6;
            break;
        case 0x0102://选手端绘制两个图形
            data_length = 30+6;
            break;
        case 0x0103://选手端绘制五个图形
            data_length = 75+6;
            break;
        case 0x0104://选手端绘制七个图形
            data_length = 105+6;
            break;
        case 0x0110://选手端绘制字符图形
            data_length = 45+6;
            break;
        case 0x0120://哨兵自主决策指令
            data_length = 4+6;
            break;
        case 0x0121://雷达自主决策指令
            data_length = 1+6;
            break;

        default:
            return -1;
    }
    /* 完成帧头打包 */
    frame_header Frame_head_send;//帧头
    frame_point = (uint8_t *)&Frame_head_send;
    Frame_head_send.SOF = 0xA5;
    Frame_head_send.data_length = data_length;
    Frame_head_send.seq = data_seq++;
    Frame_head_send.CRC8 = Get_CRC8_Check_Sum(frame_point,4,0xFF);

    memcpy(data_send_buf,&Frame_head_send,5);

    /*
    memcpy(data_send_buf,&Frame_head_send.SOF,1);
    memcpy(data_send_buf+1,&Frame_head_send.data_length,2);
    memcpy(data_send_buf+3,&Frame_head_send.seq,1);
    memcpy(data_send_buf+4,&Frame_head_send.CRC8,1);

    */

    memcpy(data_send_buf+5,&CMD_ID,2);

    robot_interaction_data_send.data_cmd_id = Child_content_ID;
    robot_interaction_data_send.receiver_id = Cilent_ID;
    robot_interaction_data_send.sender_id = Robot_ID;

    memcpy(data_send_buf+7,&robot_interaction_data_send,6);
    memcpy(data_send_buf+13,user_tx_data,data_length-6);

    /* 整包校验 */
    frametail = Get_CRC16_Check_Sum(data_send_buf,Frame_head_send.data_length+7,0xFFFF);
    memcpy(data_send_buf+Frame_head_send.data_length+7,&frametail,2);

    /* 数据发送 */
    HAL_UART_Transmit_IT(&huart6,data_send_buf,Frame_head_send.data_length+9);
    return 0;
}
