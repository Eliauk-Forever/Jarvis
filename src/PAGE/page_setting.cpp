#include "HAL/HAL.h"
#include "page_control.h"
#include "page_setting.h"

lv_obj_t* table, * sw, * btn_exit, * status, * label_SSID, * label_IP, * SSID, * IP;
lv_timer_t* timer1, * timer2, * timer3, * timer4;

//温湿度变量
float temp, humi;
uint8_t t, h;

ESP32Time rtc;
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP,"pool.ntp.org");  //NTP服务器地址

String ip2Str(IPAddress ip)		//IP地址转字符串
{ 
    String s = "";
    for (int i = 0; i < 4; i++)
	{
        s += i ? "." + String(ip[i]) : String(ip[i]);
    }
    return s;
}

void sensor_measure(lv_timer_t * timer1)    //每2s执行一次
{
	if (HAL::AHT_measure(&temp, &humi)) 
    {
        Serial.print(F("Temperature is "));
        Serial.print(temp);
        Serial.print(F(" °C, humidity is "));
        Serial.print(humi);
        Serial.println(" %");

		t = char(temp);
		h = char(humi);

		lv_table_set_cell_value_fmt(table, 0, 1, "%2d °C", t);
		lv_table_set_cell_value_fmt(table, 1, 1, "%2d RH", h);
		if(temp >= 24 && humi <= 50)
		{
			lv_table_set_cell_value(table, 2, 1, "^.^");
		}
		else
		{
			lv_table_set_cell_value(table, 2, 1, "-_-");
		}
    }
}

void wifi_detect(lv_timer_t * timer2)		    //检测当前WIFI状态，连接成功后显示SSID和IP地址并打开时间和日期更新的定时器
{
	if(Wifi_status == 1)
	{
		lv_label_set_text(status, "#BC1717 WEB配网#");
		lv_label_set_text(SSID, "ESP32_Config");
		lv_label_set_text(IP, ip2Str(WiFi.softAPIP()).c_str());
	}

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

		lv_label_set_text(status, "#FF6EC7 已连接WIFI#");
		lv_label_set_text(SSID, WiFi.SSID().c_str());
		lv_label_set_text(IP, ip2Str(WiFi.localIP()).c_str());

        lv_timer_resume(timer3);
        lv_timer_resume(timer4);
	}

    if(Wifi_status == 3)
    {
        lv_label_set_text(status, "#BC1717 连接中...#");
    }
}

void time_update(lv_timer_t * timer3)       //通过RTC获取时间,每1s执行一次
{
    currentHour = rtc.getHour(true);
    currentMinute = rtc.getMinute();
    currentSecond = rtc.getSecond();
    lv_label_set_text_fmt(home_time, "%02d:%02d:%02d", currentHour, currentMinute, currentSecond);
    //Serial.println(rtc.getTime("%Y %d %d  %H:%M:%S  %A"));
}

void day_update(lv_timer_t * timer4)        //日期更新,每1h执行一次
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
    lv_timer_set_period(timer4, 3600000);
}

static void back_keep_cb(lv_event_t* LV_EVENT_PRESSED)
{
	lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 50, 100, false);		//退出后保留页面
}

//温湿度传感器开关事件
static void aht_event_handler(lv_event_t* a)
{
	lv_event_code_t code = lv_event_get_code(a);
	lv_obj_t* obj = lv_event_get_target(a);
	if (code == LV_EVENT_VALUE_CHANGED)
	{
		if (lv_obj_has_state(obj, LV_STATE_CHECKED))
		{   
			lv_timer_resume(timer1);
		}
		else
		{
			lv_timer_pause(timer1);
			lv_table_set_cell_value(table, 0, 1, "°C");
			lv_table_set_cell_value(table, 1, 1, " RH");
			lv_table_set_cell_value(table, 2, 1, ""); 
		}
	}
}

static void wifi_event_handler(lv_event_t* b)
{
	lv_event_code_t code = lv_event_get_code(b);
	lv_obj_t* obj = lv_event_get_target(b);
	if (code == LV_EVENT_VALUE_CHANGED)
	{
		if (lv_obj_has_state(obj, LV_STATE_CHECKED))
		{
            if(HAL::AutoConfig())
            {
                lv_timer_resume(timer2);
                Wifi_status = 2;
                lv_label_set_text(status, "#FF6EC7 已连接WIFI#");
		        lv_label_set_text(SSID, WiFi.SSID().c_str());
		        lv_label_set_text(IP, ip2Str(WiFi.localIP()).c_str());  
            }
		}
		else
		{
            lv_timer_pause(timer2);
			HAL::Wifi_Close();
            Wifi_status = 0;
            lv_label_set_text(symbol_wifi, LV_SYMBOL_WARNING);
		    lv_label_set_text(status, "#3299CC WIFI已关闭#");
		    lv_label_set_text(SSID, "");
		    lv_label_set_text(IP, "");
		}
	}
}

void page_setting()
{
	lv_obj_t* tabview = lv_tabview_create(scr_setting, LV_DIR_TOP, 50);
	
	lv_obj_t* tab1 = lv_tabview_add_tab(tabview, LV_SYMBOL_LIST);
	lv_obj_t* tab2 = lv_tabview_add_tab(tabview, LV_SYMBOL_HOME);
	lv_obj_t* tab3 = lv_tabview_add_tab(tabview, LV_SYMBOL_SETTINGS);

    lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_scrollbar_mode(tab2, LV_SCROLLBAR_MODE_ACTIVE);

	lv_obj_t* container_Wifi = lv_obj_create(tab1);
	lv_obj_t* container_About = lv_obj_create(tab1);
	lv_obj_clear_flag(container_About, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_t* text1 = lv_label_create(container_About);
	lv_obj_t* text2 = lv_label_create(container_About);
	
	lv_obj_set_style_text_font(text1, &myfont, 0);
	lv_obj_set_style_text_font(text2, &myfont, 0);

	//tab1显示的内容（WIFI信息 & 关于本机）
	lv_obj_set_size(container_Wifi, 280, 120);
	lv_obj_align(container_Wifi, LV_ALIGN_TOP_MID, 0, 0);

	status = lv_label_create(container_Wifi);
	lv_label_set_recolor(status, true);
	lv_obj_set_style_text_font(status, &myfont, 0);
	lv_obj_align(status, LV_ALIGN_TOP_MID, 0, 0);

	label_SSID = lv_label_create(container_Wifi);
	lv_obj_align(label_SSID, LV_ALIGN_LEFT_MID, 5, 0);
	lv_label_set_text(label_SSID, "SSID: ");

	SSID = lv_label_create(container_Wifi);
	lv_obj_align_to(SSID, label_SSID, LV_ALIGN_LEFT_MID, 80, 0);
	
	label_IP = lv_label_create(container_Wifi);
	lv_obj_align(label_IP, LV_ALIGN_LEFT_MID, 5, 30);
	lv_label_set_text(label_IP, "IP: ");

	IP = lv_label_create(container_Wifi);
	lv_obj_align_to(IP, label_IP, LV_ALIGN_LEFT_MID, 80, 0);

	lv_obj_set_size(container_About, 280, 190);
	lv_obj_align(container_About, LV_ALIGN_TOP_MID, 0, 130);

	lv_label_set_text(text1, "名称\n"                             
							 "系统\n"
							 "硬件版本\n"
							 "软件版本\n"
							 "芯片\n"
							 "作者");

	lv_label_set_text(text2, VERSION_FIRMWARE_NAME
							 VERSION_SYSTEM_NAME
							 VERSION_HARDWARE
							 VERSION_SOFTWARE
							 VERSION_MCU
							 VERSION_AUTHOR_NAME);

	lv_obj_align_to(text2, text1, LV_ALIGN_TOP_LEFT, 135, 1);

	//tab2显示的内容（环境温湿度）
    lv_obj_t* text3 = lv_label_create(tab2);

	lv_obj_set_style_text_font(text3, &myfont, 0);

	if(HAL::AHT_begin())
	{
		timer1 = lv_timer_create(sensor_measure, 3000, NULL);		//创建一个定时器用于获取温湿度传感器测量值
		lv_timer_pause(timer1);
	}

	lv_label_set_text(text3, "传感器开关");                      
	lv_obj_align(text3, LV_ALIGN_TOP_LEFT, 20, 0);

	lv_obj_t* sw = lv_switch_create(tab2);
	lv_obj_align_to(sw, text3, LV_ALIGN_TOP_RIGHT, 100, 0);
	lv_obj_add_event_cb(sw, aht_event_handler, LV_EVENT_ALL, NULL);

	table = lv_table_create(tab2);
	lv_obj_align_to(table, text3, LV_ALIGN_CENTER, 0, 50);

	lv_table_set_cell_value(table, 0, 0, "TEMP");
	lv_table_set_cell_value(table, 1, 0, "HDMI");
	lv_table_set_cell_value(table, 2, 0, "FEEL");

	lv_table_set_cell_value(table, 0, 1, "°C");
	lv_table_set_cell_value(table, 1, 1, "%");

	//tab3显示的内容（WIFI开关 & 退出按钮）
    lv_obj_t* text4 = lv_label_create(tab3);
	lv_obj_set_style_text_font(text4, &myfont, 0);

	lv_label_set_text(text4, "WIFI开关");                      
	lv_obj_align(text4, LV_ALIGN_TOP_LEFT, 20, 0);

    lv_obj_t* sw_wifi = lv_switch_create(tab3);                       //创建一个开关用于控制Wifi
    lv_obj_add_state(sw_wifi, LV_STATE_CHECKED);
	lv_obj_align_to(sw_wifi, text4, LV_ALIGN_TOP_RIGHT, 100, 0);
	lv_obj_add_event_cb(sw_wifi, wifi_event_handler, LV_EVENT_ALL, NULL);

	btn_exit = lv_btn_create(tab3);
	lv_obj_set_size(btn_exit, 100, 30);
	lv_obj_align(btn_exit, LV_ALIGN_CENTER, 0, 50);

	lv_obj_t* label_exit = lv_label_create(btn_exit);
	lv_obj_center(label_exit);
	lv_label_set_text(label_exit, "exit");
	lv_obj_add_event_cb(btn_exit, back_keep_cb, LV_EVENT_PRESSED, NULL);

    timer2 = lv_timer_create(wifi_detect, 2000, NULL);		//创建一个定时器来检测WIFI是否连接
	lv_timer_ready(timer2);     //使定时器立即开启
	
    timer3 = lv_timer_create(time_update, 1000, NULL);      //创建一个定时器来更新时间
    lv_timer_pause(timer3);     //暂停定时器,连接wifi后开启

    timer4 = lv_timer_create(day_update, 1000, NULL);      //创建一个定时器来更新日期
    lv_timer_pause(timer4);     //暂停定时器,连接wifi后开启
}
