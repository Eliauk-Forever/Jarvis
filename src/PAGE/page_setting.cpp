#include "HAL/HAL.h"
#include "page_control.h"
#include "page_setting.h"

lv_obj_t* status, * label_SSID, * label_IP, * SSID, * IP, * label_MAC, * MAC, * sw_backlight, * sw_voice;
int newBacklight;

String ip2Str(IPAddress ip)		//IP地址转字符串
{ 
    String s = "";
    for (int i = 0; i < 4; i++)
	{
        s += i ? "." + String(ip[i]) : String(ip[i]);
    }
    return s;
}

static void back_delete_cb(lv_event_t* event)
{
    if(Wifi_status == 2)
    {
        lv_label_set_text(symbol_wifi, LV_SYMBOL_WIFI);
    }
    if(Wifi_status == 0 || Wifi_status == 1)
    {
        lv_label_set_text(symbol_wifi, LV_SYMBOL_WARNING);
    }
	lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);
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
                Wifi_status = 2;
                lv_label_set_text(status, "#FF6EC7 已连接WIFI#");
		        lv_label_set_text(SSID, WiFi.SSID().c_str());
		        lv_label_set_text(IP, ip2Str(WiFi.localIP()).c_str());
            }
		}
		else
		{
			HAL::Wifi_Close();
            Wifi_status = 0;
		    lv_label_set_text(status, "#3299CC WIFI已关闭#");
		    lv_label_set_text(SSID, "");
		    lv_label_set_text(IP, "");
		}
	}
}

static void set_backlight(lv_event_t* c)
{
    sw_backlight = lv_event_get_target(c);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(sw_backlight));

	currentBacklight = (int)lv_slider_get_value(sw_backlight);
	newBacklight = map(currentBacklight, 5, 100, 50, 1024);
	HAL::Backlight_SetValue(newBacklight);
}

static void set_voice(lv_event_t* d)
{
    sw_voice = lv_event_get_target(d);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(sw_voice));
}

void page_setting()
{
	lv_obj_t* tabview = lv_tabview_create(scr_page, LV_DIR_TOP, 50);
	lv_obj_add_event_cb(tabview, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
	lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);		//禁止滚动
	
	lv_obj_t* tab1 = lv_tabview_add_tab(tabview, LV_SYMBOL_LIST);
	lv_obj_t* tab2 = lv_tabview_add_tab(tabview, LV_SYMBOL_HOME);
	lv_obj_t* tab3 = lv_tabview_add_tab(tabview, LV_SYMBOL_SETTINGS);

	//tab1显示的内容（WIFI信息）
	lv_obj_t* container_Wifi = lv_obj_create(tab1);
	lv_obj_set_size(container_Wifi, 280, 155);
    lv_obj_center(container_Wifi);
	status = lv_label_create(container_Wifi);
	lv_label_set_recolor(status, true);
	lv_obj_set_style_text_font(status, &myfont, 0);
	lv_obj_align(status, LV_ALIGN_TOP_MID, 0, 0);
	label_SSID = lv_label_create(container_Wifi);
	lv_label_set_text(label_SSID, "SSID: ");
    lv_obj_set_pos(label_SSID, 5, 40);
	SSID = lv_label_create(container_Wifi);
	lv_obj_align_to(SSID, label_SSID, LV_ALIGN_LEFT_MID, 70, 0);
	label_IP = lv_label_create(container_Wifi);
	lv_label_set_text(label_IP, "IP: ");
    lv_obj_set_pos(label_IP, 5, 70);
	IP = lv_label_create(container_Wifi);
	lv_obj_align_to(IP, label_IP, LV_ALIGN_LEFT_MID, 70, 0);
    label_MAC = lv_label_create(container_Wifi);
    lv_label_set_text(label_MAC, "MAC: ");
    lv_obj_set_pos(label_MAC, 5, 100);
    MAC = lv_label_create(container_Wifi);
    lv_obj_align_to(MAC, label_MAC, LV_ALIGN_LEFT_MID, 70, 0);

    static lv_style_t text_style;
    lv_style_init(&text_style);
    lv_style_set_text_font(&text_style, &lv_font_montserrat_18);
    lv_obj_add_style(label_SSID, &text_style, 0);
    lv_obj_add_style(label_IP, &text_style, 0);
    lv_obj_add_style(label_MAC, &text_style, 0);
    lv_obj_add_style(SSID, &text_style, 0);
    lv_obj_add_style(IP, &text_style, 0);
    lv_obj_add_style(MAC, &text_style, 0);

    if(Wifi_status == 0)
    {
		lv_label_set_text(status, "#3299CC WIFI已关闭#");
		lv_label_set_text(SSID, "");
		lv_label_set_text(IP, "");
		lv_label_set_text(MAC, "");
    }
    if(Wifi_status == 1)
    {
        lv_label_set_text(status, "#BC1717 WEB配网#");
		lv_label_set_text(SSID, "ESP32_Config");
		lv_label_set_text(IP, ip2Str(WiFi.softAPIP()).c_str());
		lv_label_set_text(MAC, WiFi.softAPmacAddress().c_str());
    }
    if(Wifi_status == 2)
    {
		lv_label_set_text(status, "#FF6EC7 已连接WIFI#");
		lv_label_set_text(SSID, WiFi.SSID().c_str());
		lv_label_set_text(IP, ip2Str(WiFi.localIP()).c_str());
		lv_label_set_text(MAC, WiFi.softAPmacAddress().c_str());
    }

	//tab2显示的内容(关于本机)
	lv_obj_t* container_About = lv_obj_create(tab2);
	lv_obj_set_size(container_About, 280, 160);
	lv_obj_center(container_About);
	lv_obj_t* text1 = lv_label_create(container_About);
	lv_obj_t* text2 = lv_label_create(container_About);	
	lv_obj_set_style_text_font(text1, &myfont, 0);
	lv_obj_set_style_text_font(text2, &myfont, 0);
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
	lv_obj_align_to(text2, text1, LV_ALIGN_TOP_LEFT, 150, 1);

	//tab3显示的内容（设置界面）
    lv_obj_t* text3 = lv_label_create(tab3);
	lv_obj_set_style_text_font(text3, &myfont, 0);
	lv_label_set_text(text3, "WIFI开关");    
	lv_obj_set_pos(text3, 0, 10);
    lv_obj_t* sw_wifi = lv_switch_create(tab3);
    if(Wifi_status == 2)
    {
        lv_obj_add_state(sw_wifi, LV_STATE_CHECKED);
    }   
	lv_obj_set_size(sw_wifi, 50, 25);
	lv_obj_set_pos(sw_wifi, 120, 10);
	lv_obj_add_event_cb(sw_wifi, wifi_event_handler, LV_EVENT_ALL, NULL);

	//屏幕背光亮度控制
	lv_obj_t* text4 = lv_label_create(tab3);
    lv_obj_set_style_text_font(text4, &myfont, 0);
    lv_label_set_text(text4, "屏幕亮度");
    lv_obj_set_pos(text4, 0, 50);
	sw_backlight = lv_slider_create(tab3);
    lv_obj_set_pos(sw_backlight, 120, 50);
    lv_obj_set_size(sw_backlight, 140, 20);
    lv_slider_set_range(sw_backlight, 5, 100);
    lv_slider_set_value(sw_backlight, currentBacklight, LV_ANIM_OFF);
    lv_obj_add_event_cb(sw_backlight, set_backlight, LV_EVENT_VALUE_CHANGED, NULL);

	lv_obj_t* text5 = lv_label_create(tab3);
    lv_obj_set_style_text_font(text5, &myfont, 0);
    lv_label_set_text(text5, "蜂鸣器开关");
    lv_obj_set_pos(text5, 0, 100);
    lv_obj_t* sw_buzzer = lv_switch_create(tab3);
    lv_obj_set_size(sw_buzzer, 50, 25);
    lv_obj_add_state(sw_buzzer, LV_STATE_CHECKED);
    lv_obj_set_pos(sw_buzzer, 120, 100);

    lv_obj_t* text6 = lv_label_create(tab3);
    lv_obj_set_style_text_font(text6, &myfont, 0);
    lv_label_set_text(text6, "设备音量");
    lv_obj_set_pos(text6, 0, 140);
    sw_voice = lv_slider_create(tab3);
    lv_obj_set_pos(sw_voice, 120, 140);
    lv_obj_set_size(sw_voice, 140, 20);
    lv_slider_set_range(sw_voice, 10, 100);
    lv_slider_set_value(sw_voice, 50, LV_ANIM_OFF);
    lv_obj_add_event_cb(sw_voice, set_voice, LV_EVENT_VALUE_CHANGED, NULL);
}