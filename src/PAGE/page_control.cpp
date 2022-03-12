#include "HAL/HAL.h"
#include "page_control.h"
#include "page_infrared.h"
#include "page_weather.h"
#include "page_calendar.h"
#include "page_news.h"
#include "page_sensor.h"
#include "page_setting.h"

LV_FONT_DECLARE(myfont)
LV_IMG_DECLARE(ironman)
LV_IMG_DECLARE(desktop)
LV_IMG_DECLARE(kongzhi)
LV_IMG_DECLARE(tianqi)
LV_IMG_DECLARE(rili)
LV_IMG_DECLARE(xinwen)
LV_IMG_DECLARE(ganzhi)
LV_IMG_DECLARE(shezhi)

// HTTP请求所需信息
String reqUserKey1 = "SAtkG9P2EzpXVUE-_";   				        // 心知天气私钥
String reqLocation = "ShenZhen";            				        // 城市
String reqLanguage = "zh-Hans";            					        // 语言
String reqUnit = "c";                       				        // 摄氏/华氏
String reqRes1 = "/v3/weather/now.json?key=" + reqUserKey1 +
                + "&location=" + reqLocation + "&language=" + reqLanguage +
                "&unit=" + reqUnit;
String JsonAnswer;

uint16_t currentHour, currentMinute, currentSecond, weekDay, monthDay, currentMonth, currentYear;

int results_daima, results_wendu, results_tigan, results_shidu, results_nengjiandu, results_fengsu;
String results_chengshi = "", results_xianxiang = "";

lv_obj_t* scr_setup, * scr_home, * scr_page;
lv_obj_t* symbol_wifi, * symbol_sd, * home_time, * btn_back;
lv_style_t img_bg;
lv_timer_t* timer1, * timer2, * timer3;

ESP32Time rtc;
WiFiUDP ntpUDP;
WiFiClient client;
NTPClient timeClient(ntpUDP, "ntp.aliyun.com");  //NTP服务器地址

void ParseInfo_xinzhi(String& json)
{
    StaticJsonDocument<768> doc;
  	deserializeJson(doc, json);
    JsonObject results_0 = doc["results"][0];
    JsonObject results_0_location = results_0["location"];
    JsonObject results_0_now = results_0["now"];

    // 通过串口监视器显示以上信息
    results_chengshi = results_0_location["name"].as<String>();          //城市名称
  	results_xianxiang = results_0_now["text"].as<String>();              //天气现象文字
  	results_daima = results_0_now["code"].as<int>();                     //天气现象代码
    results_wendu = results_0_now["temperature"].as<int>();              //温度
    results_tigan = results_0_now["feels_like"].as<int>();               //体感温度
    results_shidu = results_0_now["humidity"].as<int>();                 //相对湿度
    results_nengjiandu = results_0_now["visibility"].as<int>();          //能见度
    results_fengsu = results_0_now["wind_speed"].as<int>();              //风速

    Serial.println("======Weahter Now=======");
    Serial.print("城市: ");
  	Serial.println(results_chengshi);
  	Serial.print("天气现象: ");
  	Serial.println(results_xianxiang);
  	Serial.print("天气代码: ");
  	Serial.println(results_daima);
    Serial.print("温度: ");
  	Serial.println(results_wendu);
    Serial.print("体感温度: ");
  	Serial.println(results_tigan);
    Serial.print("相对湿度: ");
  	Serial.println(results_shidu);
    Serial.print("能见度: ");
  	Serial.println(results_nengjiandu);
    Serial.print("风速: ");
  	Serial.println(results_fengsu);
  	Serial.println("========================"); 
}

void HttpRequest(String reqRes, const char* host)
{	
  	// 建立http请求信息
  	String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
  	                            "Host: " + host + "\r\n" + 
  	                            "Connection: close\r\n\r\n";
 	if (client.connect(host, 80))
	{
    	// 向服务器发送http请求信息
    	client.print(httpRequest);
    	// 获取并显示服务器响应状态行 
    	String status_response = client.readStringUntil('\n');
		String Answer;
		while(client.available())
    	{
      		String line = client.readStringUntil('\r');
      		Answer += line;
    	}
    	// 使用find跳过HTTP响应头
    	if (client.find("\r\n\r\n")) 
		{
    	  	//Serial.println("Found Header End. Start Parsing.");
    	}
  		int JsonIndex;
  		//找到有用的返回数据位置i 返回头不要
  		for (int i = 0; i < Answer.length(); i++) 
		{
  		  	if (Answer[i] == '{') 
			{
  		    	JsonIndex = i;
  		    	break;
  		  	}
  		}
  		JsonAnswer = Answer.substring(JsonIndex);
      	//Serial.println("JsonAnswer: ");
      	//Serial.println(JsonAnswer);
  	} 
	else 
	{
    	Serial.println(" connection failed!");
  	}   
  	client.stop();  //断开连接
}

static void back_delete_cb(lv_event_t* event)
{
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);   //退出后删除页面
}

static void btn1_event_cb(lv_event_t * event)       //控制
{
    scr_page = lv_obj_create(NULL);
    lv_scr_load_anim(scr_page, LV_SCR_LOAD_ANIM_NONE, 50, 0, false);
    lv_obj_add_event_cb(scr_page, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
    page_infrared();
}

static void btn2_event_cb(lv_event_t * event)       //天气
{
    scr_page = lv_obj_create(NULL);
    lv_scr_load_anim(scr_page, LV_SCR_LOAD_ANIM_NONE, 50, 0, false);
    page_weather();
}

static void btn3_event_cb(lv_event_t * event)       //日历
{
    scr_page = lv_obj_create(NULL);
    lv_scr_load_anim(scr_page, LV_SCR_LOAD_ANIM_NONE, 50, 0, false);
    lv_obj_add_event_cb(scr_page, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);

    btn_back = lv_btn_create(scr_page);
    lv_obj_set_size(btn_back, 150, 30);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 10, 10);

    lv_obj_t* lunar = lv_label_create(btn_back);
    lv_obj_set_style_text_font(lunar, &myfont, 0);
    lv_label_set_text(lunar, "壬寅虎年");
    lv_obj_center(lunar);

    lv_obj_add_event_cb(btn_back, back_delete_cb, LV_EVENT_PRESSED, NULL);
    page_calendar();
}

static void btn4_event_cb(lv_event_t * event)       //信息
{
    scr_page = lv_obj_create(NULL);
    lv_scr_load_anim(scr_page, LV_SCR_LOAD_ANIM_NONE, 50, 0, false);
    page_news();
}

static void btn5_event_cb(lv_event_t * event)       //感知
{
    scr_page = lv_obj_create(NULL);
    lv_scr_load_anim(scr_page, LV_SCR_LOAD_ANIM_NONE, 50, 0, false);
    page_sensor();
}

static void btn6_event_cb(lv_event_t * event)       //设置
{
    scr_page = lv_obj_create(NULL);
    lv_scr_load_anim(scr_page, LV_SCR_LOAD_ANIM_NONE, 50, 0, false);
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

        //获取天气信息
        HttpRequest(reqRes1, "api.seniverse.com");
		ParseInfo_xinzhi(JsonAnswer);      // 利用ArduinoJson库解析响应信息

        lv_timer_resume(timer2);
        lv_timer_ready(timer3);
        lv_timer_pause(timer1);
	}
}

void time_update(lv_timer_t * timer2)       //通过RTC获取时间,每1s执行一次
{
    currentHour = rtc.getHour(true);
    currentMinute = rtc.getMinute();
    currentSecond = rtc.getSecond();
    lv_label_set_text_fmt(home_time, "%02d:%02d:%02d", currentHour, currentMinute, currentSecond);
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
    lv_timer_pause(timer3);
}

void page_init()
{
    scr_setup = lv_obj_create(NULL);
    lv_scr_load_anim(scr_setup, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);
    lv_obj_t* bg_setup = lv_img_create(scr_setup);
    lv_img_set_src(bg_setup, &ironman);
    
    lv_obj_t* spinner = lv_spinner_create(bg_setup, 2000, 40);
    lv_obj_set_size(spinner, 50, 50);
    lv_obj_align(spinner, LV_ALIGN_BOTTOM_RIGHT, -15, -15);
}

void page_home()
{
    scr_home = lv_obj_create(NULL);
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 2100, true);
    
    //设置桌面壁纸
    lv_obj_t* bg_desktop = lv_img_create(scr_home);
    lv_img_set_src(bg_desktop, &desktop);

    //创建桌面状态栏
    lv_obj_t* symbol1 = lv_label_create(bg_desktop);
    lv_obj_t* symbol2 = lv_label_create(bg_desktop);
    symbol_wifi = lv_label_create(bg_desktop);
    home_time = lv_label_create(bg_desktop);
    symbol_sd = lv_label_create(bg_desktop);
    lv_obj_set_style_text_color(symbol1, lv_color_white(), 0);
    lv_obj_set_style_text_color(symbol2, lv_color_white(), 0);
    lv_obj_set_style_text_color(symbol_wifi, lv_color_white(), 0);
    lv_obj_set_style_text_color(home_time, lv_color_white(), 0);
    lv_obj_set_style_text_color(symbol_sd, lv_color_white(), 0);
    lv_label_set_text(symbol1, LV_SYMBOL_HOME);
    lv_label_set_text(home_time, "--:--:--");
    lv_label_set_text(symbol_wifi, LV_SYMBOL_WARNING);
    lv_label_set_text(symbol_sd, LV_SYMBOL_SD_CARD);
    lv_label_set_text(symbol2, LV_SYMBOL_BATTERY_2);
    lv_obj_align(symbol1, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_align(home_time, LV_ALIGN_TOP_LEFT, 80, 10);
    lv_obj_align(symbol_sd, LV_ALIGN_TOP_LEFT, 195, 10);
    lv_obj_align(symbol_wifi, LV_ALIGN_TOP_LEFT, 230, 10);
    lv_obj_align(symbol2, LV_ALIGN_TOP_LEFT, 275, 10);

    //创建图标背景风格
    lv_style_init(&img_bg);
    lv_style_set_bg_img_opa(&img_bg, LV_OPA_TRANSP);

    lv_obj_t* btn1 = lv_imgbtn_create(scr_home);
    lv_obj_add_style(btn1, &img_bg, 0);
    lv_obj_set_size(btn1, 75, 75);
    lv_obj_align(btn1, LV_ALIGN_TOP_LEFT, 24, 45);
    lv_imgbtn_set_src(btn1, LV_IMGBTN_STATE_RELEASED, NULL, &kongzhi, NULL);
    lv_obj_t* label_btn1 = lv_label_create(scr_home);
    lv_obj_set_style_text_color(label_btn1, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_btn1, &myfont, 0);
    lv_label_set_text(label_btn1, "控制");
    lv_obj_align_to(label_btn1, btn1, LV_ALIGN_BOTTOM_MID, 0, 20);

    lv_obj_t* btn2 = lv_imgbtn_create(scr_home);
    lv_obj_add_style(btn2, &img_bg, 0);
    lv_obj_set_size(btn2, 75, 75);
    lv_obj_align_to(btn2, btn1, LV_ALIGN_OUT_RIGHT_MID, 24, 0);
    lv_imgbtn_set_src(btn2, LV_IMGBTN_STATE_RELEASED, NULL, &tianqi, NULL);
    lv_obj_t* label_btn2 = lv_label_create(scr_home);
    lv_obj_set_style_text_color(label_btn2, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_btn2, &myfont, 0);
    lv_label_set_text(label_btn2, "天气");
    lv_obj_align_to(label_btn2, btn2, LV_ALIGN_BOTTOM_MID, 0, 20);

    lv_obj_t* btn3 = lv_imgbtn_create(scr_home);
    lv_obj_add_style(btn3, &img_bg, 0);
    lv_obj_set_size(btn3, 75, 75);
    lv_obj_align_to(btn3, btn2, LV_ALIGN_OUT_RIGHT_MID, 24, 0);
    lv_imgbtn_set_src(btn3, LV_IMGBTN_STATE_RELEASED, NULL, &rili, NULL);
    lv_obj_t* label_btn3 = lv_label_create(scr_home);
    lv_obj_set_style_text_color(label_btn3, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_btn3, &myfont, 0);
    lv_label_set_text(label_btn3, "日历");
    lv_obj_align_to(label_btn3, btn3, LV_ALIGN_BOTTOM_MID, 0, 20);

    lv_obj_t* btn4 = lv_imgbtn_create(scr_home);
    lv_obj_add_style(btn4, &img_bg, 0);
    lv_obj_set_size(btn4, 75, 75);
    lv_obj_align_to(btn4, label_btn1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_imgbtn_set_src(btn4, LV_IMGBTN_STATE_RELEASED, NULL, &xinwen, NULL);
    lv_obj_t* label_btn4 = lv_label_create(scr_home);
    lv_obj_set_style_text_color(label_btn4, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_btn4, &myfont, 0);
    lv_label_set_text(label_btn4, "信息");
    lv_obj_align_to(label_btn4, btn4, LV_ALIGN_BOTTOM_MID, 0, 20);

    lv_obj_t* btn5 = lv_imgbtn_create(scr_home);
    lv_obj_add_style(btn5, &img_bg, 0);
    lv_obj_set_size(btn5, 75, 75);
    lv_obj_align_to(btn5, label_btn2, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_imgbtn_set_src(btn5, LV_IMGBTN_STATE_RELEASED, NULL, &ganzhi, NULL);
    lv_obj_t* label_btn5 = lv_label_create(scr_home);
    lv_obj_set_style_text_color(label_btn5, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_btn5, &myfont, 0);
    lv_label_set_text(label_btn5, "感知");
    lv_obj_align_to(label_btn5, btn5, LV_ALIGN_BOTTOM_MID, 0, 20);

    lv_obj_t* btn6 = lv_imgbtn_create(scr_home);
    lv_obj_add_style(btn6, &img_bg, 0);
    lv_obj_set_size(btn6, 75, 75);
    lv_obj_align_to(btn6, label_btn3, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_imgbtn_set_src(btn6, LV_IMGBTN_STATE_RELEASED, NULL, &shezhi, NULL);
    lv_obj_t* label_btn6 = lv_label_create(scr_home);
    lv_obj_set_style_text_color(label_btn6, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_btn6, &myfont, 0);
    lv_label_set_text(label_btn6, "设置");
    lv_obj_align_to(label_btn6, btn6, LV_ALIGN_BOTTOM_MID, 0, 20);
    
    //创建按钮点击事件,跳转页面
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(btn2, btn2_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(btn3, btn3_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(btn4, btn4_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(btn5, btn5_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(btn6, btn6_event_cb, LV_EVENT_SHORT_CLICKED, NULL);

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