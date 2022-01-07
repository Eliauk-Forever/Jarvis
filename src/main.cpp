#include <Arduino.h>
#include "HAL/HAL.h"
#include "PORT/Display.h"
#include "PAGE/page_control.h"
#include "PAGE/page_infrared.h"
#include "PAGE/page_calendar.h"
#include "PAGE/page_setting.h"

lv_timer_t * t_wifi;

void wifi_timer(lv_timer_t * t_wifi)                    //检测是否连接WIFI
{
    if(Wifi_status)
    {
        lv_label_set_text(symbol_wifi, LV_SYMBOL_WIFI);
        lv_timer_pause(t_wifi);                         //WIFI连接成功后在主页显示WIFI图标并暂停定时器
    }
    else
    {
        lv_label_set_text(symbol_wifi, LV_SYMBOL_WARNING);
    }
}   

void setup() 
{
	HAL::Init();
	screen_init();
	page_init(); 	
    page_home();
	HAL::Wifi_Connect();
    t_wifi = lv_timer_create(wifi_timer, 2000, NULL);   //创建一个定时器用于检测WIFI连接状态,每2s执行一次
}

void loop() 
{
	HAL::Update();
}