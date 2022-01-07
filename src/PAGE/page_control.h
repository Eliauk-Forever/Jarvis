#ifndef PAGE_CONTROL_H
#define PAGE_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

//屏幕变量
extern lv_obj_t* scr_setup, * scr_home, * scr_setting, * scr_weather, * scr_infrared, * scr_music, * scr_calendar, * scr_time;
extern lv_obj_t* bg_setup, * bg_desktop, * symbol_wifi, * home_time;

void page_init();
void page_home();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  
