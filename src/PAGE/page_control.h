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

//存放服务器的返回信息
extern int results_daima, results_wendu, results_tigan, results_shidu, results_nengjiandu, results_fengsu;
extern String results_chengshi, results_xianxiang;

void Gui_Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  
