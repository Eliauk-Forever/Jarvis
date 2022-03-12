#include "HAL/HAL.h"
#include "page_control.h"
#include "page_setting.h"

lv_obj_t* status, * label_SSID, * label_IP, * SSID, * IP, * label_MAC, * MAC;

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

void page_setting()
{
	lv_obj_t* tabview = lv_tabview_create(scr_page, LV_DIR_TOP, 50);
	lv_obj_add_event_cb(tabview, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
	
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
	lv_obj_align(text3, LV_ALIGN_TOP_LEFT, 20, 0);
	
	//创建一个WIFI控制开关
    lv_obj_t* sw_wifi = lv_switch_create(tab3);
    if(Wifi_status == 2)
    {
        lv_obj_add_state(sw_wifi, LV_STATE_CHECKED);
    }   
	lv_obj_set_size(sw_wifi, 40, 20);
	lv_obj_set_pos(sw_wifi, 115, 0);
	lv_obj_add_event_cb(sw_wifi, wifi_event_handler, LV_EVENT_ALL, NULL);
}
