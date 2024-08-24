//
// Created by xuejl on 2024/4/23.
//

#ifndef ENGINEER_LCY_REFEREE_UI_FUNCTION_H
#define ENGINEER_LCY_REFEREE_UI_FUNCTION_H


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "RobotArm.h"
/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern uint8_t UI_reset_flag;
/* Exported function declarations --------------------------------------------*/
void UI_draw_layer_9(void);
void UI_draw_layer_8(void);
void UI_draw_arm_state(void);
void UI_draw_sucker_state(void);
void UI_draw_layer_5(void);

#endif //ENGINEER_LCY_REFEREE_UI_FUNCTION_H
