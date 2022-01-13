#include "HAL/HAL.h"
#include "page_control.h"
#include "page_infrared.h"
#include "page_weather.h"
#include "page_calendar.h"
#include "page_time.h"
#include "page_setting.h"

LV_FONT_DECLARE(myfont);
LV_IMG_DECLARE(ironman);
LV_IMG_DECLARE(desktop);

lv_obj_t* scr_setup, * scr_home, * scr_setting, * scr_weather, * scr_infrared, * scr_music, * scr_calendar, * scr_time;
lv_obj_t* bg_setup, * bg_desktop;
lv_obj_t* symbol_wifi, * btn_back, * home_time;

lv_timer_t* timer1, * timer2, * timer3;

uint16_t currentHour, currentMinute, currentSecond, weekDay, monthDay, currentMonth, currentYear;

ESP32Time rtc;
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP,"pool.ntp.org");  //NTP服务器地址

static void back_delete_cb(lv_event_t* LV_EVENT_LONG_PRESSED)
{
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 10, 10, true);        //退出后删除页面
}

static void btn1_event_cb(lv_event_t * LV_EVENT_RELEASED)       //红外
{
    scr_infrared = lv_obj_create(NULL);
    lv_scr_load_anim(scr_infrared, LV_SCR_LOAD_ANIM_FADE_ON, 50, 10, false);
    lv_obj_add_event_cb(scr_infrared, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
    page_infrared();
}

static void btn2_event_cb(lv_event_t * LV_EVENT_RELEASED)       //天气
{
    scr_weather = lv_obj_create(NULL);
    lv_scr_load_anim(scr_weather, LV_SCR_LOAD_ANIM_FADE_ON, 50, 10, false);
    lv_obj_add_event_cb(scr_weather, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
    page_weather();
}

static void btn3_event_cb(lv_event_t * LV_EVENT_RELEASED)       //日历
{
    scr_calendar = lv_obj_create(NULL);
    lv_scr_load_anim(scr_calendar, LV_SCR_LOAD_ANIM_FADE_ON, 10, 100, false);

    btn_back = lv_btn_create(scr_calendar);
    lv_obj_set_size(btn_back, 150, 30);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 10, 10);

    lv_obj_t* lunar = lv_label_create(btn_back);
    lv_obj_set_style_text_font(lunar, &myfont, 0);
    lv_label_set_text(lunar, "壬寅虎年");
    lv_obj_center(lunar);

    lv_obj_add_event_cb(btn_back, back_delete_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(scr_calendar, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
    page_calendar();
}

static void btn4_event_cb(lv_event_t * LV_EVENT_RELEASED)       //时钟
{
    scr_time = lv_obj_create(NULL);
    lv_scr_load_anim(scr_time, LV_SCR_LOAD_ANIM_FADE_ON, 50, 10, false);
    page_time();
}

static void btn5_event_cb(lv_event_t * LV_EVENT_RELEASED)       //音乐
{
    scr_music = lv_obj_create(NULL);
    lv_scr_load_anim(scr_music, LV_SCR_LOAD_ANIM_FADE_ON, 50, 10, false);
    lv_obj_add_event_cb(scr_music, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
}

static void btn6_event_cb(lv_event_t * LV_EVENT_RELEASED)       //设置
{
    scr_setting = lv_obj_create(NULL); 
    lv_scr_load_anim(scr_setting, LV_SCR_LOAD_ANIM_FADE_ON, 50, 10, false);
    page_setting(); 
}

void wifi_detect(lv_timer_t * timer1)		    //检测当前WIFI状态，连接成功后显示SSID和IP地址并打开时间和日期更新的定时器
{
	if(Wifi_status == 2)
	{
        timeClient.begin();
        timeClient.setTimeOffset(28800);        //设置偏移时间（以秒为单位）以调整时区
        timeClient.update();
        unsigned long epochTime = timeClient.getEpochTime();
        currentHour = timeClient.getHours();    //获取时间
        currentMinute = timeClient.getMinutes();
        currentSecond = timeClient.getSeconds();
        weekDay = timeClient.getDay();
        //将epochTime换算成年月日
        struct tm *ptm = gmtime ((time_t *)&epochTime);
        monthDay = ptm->tm_mday;
        currentMonth = ptm->tm_mon+1;
        currentYear = ptm->tm_year+1900;
        timeClient.end();
        rtc.setTime(currentSecond, currentMinute, currentHour, monthDay, currentMonth, currentYear);
        lv_label_set_text(symbol_wifi, LV_SYMBOL_WIFI);

        lv_timer_resume(timer2);
        lv_timer_resume(timer3);

        lv_timer_pause(timer1);
	}
}

void time_update(lv_timer_t * timer2)       //通过RTC获取时间,每1s执行一次
{
    currentHour = rtc.getHour(true);
    currentMinute = rtc.getMinute();
    currentSecond = rtc.getSecond();
    lv_label_set_text_fmt(home_time, "%02d:%02d:%02d", currentHour, currentMinute, currentSecond);
    //Serial.println(rtc.getTime("%Y %d %d  %H:%M:%S  %A"));
}

void day_update(lv_timer_t * timer3)        //日期更新,每1h执行一次
{
    currentYear = rtc.getYear();
    currentMonth = rtc.getMonth() + 1;
    monthDay = rtc.getDay();
	Serial.print("CurrentDay : ");
	Serial.print(currentYear);
	Serial.print("/");
	Serial.print(currentMonth);
	Serial.print("/");
	Serial.println(monthDay);
    lv_timer_set_period(timer3, 3600000);
}

void page_init()
{
    //背景主题初始化
    scr_setup = lv_obj_create(NULL);
    lv_scr_load(scr_setup);

    bg_setup = lv_img_create(scr_setup);
    lv_img_set_src(bg_setup, &ironman);

    lv_obj_t* spinner_start = lv_spinner_create(bg_setup, 3000, 15);
    lv_obj_set_size(spinner_start, 45, 45);
    lv_obj_align(spinner_start, LV_ALIGN_BOTTOM_RIGHT, -15, -15);
}

void page_home()
{
    scr_home = lv_obj_create(NULL);
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 300, 2000, true);

    bg_desktop = lv_img_create(scr_home);
    lv_img_set_src(bg_desktop, &desktop);

    lv_obj_t* symbol1 = lv_label_create(bg_desktop);
    lv_obj_t* symbol2 = lv_label_create(bg_desktop);
    symbol_wifi = lv_label_create(bg_desktop);
    home_time = lv_label_create(bg_desktop);

    lv_label_set_text(symbol1, LV_SYMBOL_HOME);
    lv_label_set_text(home_time, "--:--:--");
    lv_label_set_text(symbol_wifi, LV_SYMBOL_WARNING);
    lv_label_set_text(symbol2, LV_SYMBOL_BATTERY_2);

    lv_obj_set_pos(symbol1, 10, 10);
    lv_obj_set_pos(home_time, 80, 10);
    lv_obj_set_pos(symbol_wifi, 230, 10);
    lv_obj_set_pos(symbol2, 275, 10);
    
    lv_obj_t* btn1 = lv_btn_create(bg_desktop);
    lv_obj_set_size(btn1, 60, 60);
    lv_obj_align(btn1, LV_ALIGN_CENTER, -100, -30);

    lv_obj_t* label_btn1 = lv_label_create(btn1);
    lv_obj_set_style_text_font(label_btn1, &myfont, 0);
    lv_label_set_text(label_btn1, "红外");
    lv_obj_center(label_btn1);

    lv_obj_t* btn2 = lv_btn_create(bg_desktop);
    lv_obj_set_size(btn2, 60, 60);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, -30);

    lv_obj_t* label_btn2 = lv_label_create(btn2);
    lv_obj_set_style_text_font(label_btn2, &myfont, 0);
    lv_label_set_text(label_btn2, "天气");
    lv_obj_center(label_btn2);

    lv_obj_t* btn3 = lv_btn_create(bg_desktop);
    lv_obj_set_size(btn3, 60, 60);
    lv_obj_align(btn3, LV_ALIGN_CENTER, 100, -30);

    lv_obj_t* label_btn3 = lv_label_create(btn3);
    lv_obj_set_style_text_font(label_btn3, &myfont, 0);
    lv_label_set_text(label_btn3, "日历");
    lv_obj_center(label_btn3);

    lv_obj_t* btn4 = lv_btn_create(bg_desktop);
    lv_obj_set_size(btn4, 60, 60);
    lv_obj_align(btn4, LV_ALIGN_CENTER, -100, 65);

    lv_obj_t* label_btn4 = lv_label_create(btn4);
    lv_obj_set_style_text_font(label_btn4, &myfont, 0);
    lv_label_set_text(label_btn4, "时钟");
    lv_obj_center(label_btn4);

    lv_obj_t* btn5 = lv_btn_create(bg_desktop);
    lv_obj_set_size(btn5, 60, 60);
    lv_obj_align(btn5, LV_ALIGN_CENTER, 0, 65);

    lv_obj_t* label_btn5 = lv_label_create(btn5);
    lv_obj_set_style_text_font(label_btn5, &myfont, 0);
    lv_label_set_text(label_btn5, "音乐");
    lv_obj_center(label_btn5);

    lv_obj_t* btn6 = lv_btn_create(bg_desktop);
    lv_obj_set_size(btn6, 60, 60);
    lv_obj_align(btn6, LV_ALIGN_CENTER, 100, 65);

    lv_obj_t* label_btn6 = lv_label_create(btn6);
    lv_obj_set_style_text_font(label_btn6, &myfont, 0);
    lv_label_set_text(label_btn6, "设置");
    lv_obj_center(label_btn6);
    
    //创建按钮点击事件,跳转页面
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(btn2, btn2_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(btn3, btn3_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(btn4, btn4_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(btn5, btn5_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(btn6, btn6_event_cb, LV_EVENT_RELEASED, NULL);

    timer1 = lv_timer_create(wifi_detect, 2000, NULL);		//创建一个定时器来检测WIFI是否连接
	
    timer2 = lv_timer_create(time_update, 1000, NULL);      //创建一个定时器来更新时间
    lv_timer_pause(timer2);     //暂停定时器,连接wifi后开启

    timer3 = lv_timer_create(day_update, 1000, NULL);      //创建一个定时器来更新日期
    lv_timer_pause(timer3);     //暂停定时器,连接wifi后开启
}

void Gui_Init(void)
{
    page_init();
    page_home();
    HAL::Wifi_Connect();
}