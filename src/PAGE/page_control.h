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
extern int results_wendu, results_shidu, img_code, results_air;
extern String results_chengshi, results_wea, results_fengxiang, results_fengli, retdata_city;                                      //天气数据变量
extern int Local_curConfirm, Local_confirm, Local_asymptomatic, Local_nativeRelative, Local_heal, Local_died;                      //地区疫情变量
extern int National_curConfirm, National_confirm, National_asymptomatic, National_nativeRelative, National_heal, National_died;    //全国疫情变量

void Gui_Init(void);
void Text_Format(lv_obj_t* obj, int pos_x, int pos_y, bool hans, bool recolor);
bool Data_Update();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  
