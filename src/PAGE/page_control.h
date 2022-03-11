#ifndef PAGE_CONTROL_H
#define PAGE_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

//屏幕变量
extern lv_obj_t* scr_home, * scr_page;
extern lv_obj_t* symbol_wifi;
extern lv_style_t img_bg;

//时间&日期变量
extern uint16_t currentHour, currentMinute, currentSecond, weekDay, monthDay, currentMonth, currentYear;

void Gui_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  
