#include "HAL/HAL.h"
#include "page_control.h"
#include "page_weather.h"

//天气代码图标
LV_IMG_DECLARE(null)
LV_IMG_DECLARE(code0)
LV_IMG_DECLARE(code1)
LV_IMG_DECLARE(code4)
LV_IMG_DECLARE(code5)
LV_IMG_DECLARE(code6)
LV_IMG_DECLARE(code7)
LV_IMG_DECLARE(code8)
LV_IMG_DECLARE(code9)
LV_IMG_DECLARE(code10)
LV_IMG_DECLARE(code11)
LV_IMG_DECLARE(code12)
LV_IMG_DECLARE(code13)
LV_IMG_DECLARE(code14)
LV_IMG_DECLARE(code15)
LV_IMG_DECLARE(code16)
LV_IMG_DECLARE(code17)
LV_IMG_DECLARE(code18)
LV_IMG_DECLARE(code30)
LV_IMG_DECLARE(code31)
LV_IMG_DECLARE(code32)
LV_IMG_DECLARE(code33)
LV_IMG_DECLARE(code37)
LV_IMG_DECLARE(code38)
LV_IMG_DECLARE(code99)

// 心知天气HTTP请求所需信息
String reqUserKey = "SAtkG9P2EzpXVUE-_";   // 私钥
String reqLocation = "ShenZhen";            // 城市
String reqLanguage = "zh-Hans";            // 语言
String reqUnit = "c";                      // 摄氏/华氏
String reqRes = "/v3/weather/now.json?key=" + reqUserKey +
                + "&location=" + reqLocation + "&language=" + reqLanguage +
                "&unit=" + reqUnit;

//存放服务器的返回信息
String results_chengshi = "", results_xianxiang = "";
int results_daima, results_wendu, results_tigan, results_shidu, results_nengjiandu, results_fengsu;

WiFiClient client;

lv_obj_t * bg_null, * shijian;
lv_timer_t* timer_update;

static void back(lv_event_t* event)
{
    lv_timer_del(timer_update);
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);   //退出后删除页面
}

void ParseInfo(String& json)
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
		String JsonAnswer;
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
        Serial.println("JsonAnswer: ");
        Serial.println(JsonAnswer);
    	ParseInfo(JsonAnswer);      // 利用ArduinoJson库解析响应信息
  	} 
	else 
	{
    	Serial.println(" connection failed!");
  	}   
  	client.stop();  //断开连接
}

void update(lv_timer_t * timer_update)
{
    lv_label_set_text_fmt(shijian, "%d:%d:%d", currentHour, currentMinute, currentSecond);
}

void page_weather()
{
    timer_update = lv_timer_create(update, 1000, NULL);		//创建一个定时器更新时间
    lv_obj_add_event_cb(scr_page, back, LV_EVENT_LONG_PRESSED, NULL);

    if(Wifi_status == 2)
    {
        HttpRequest(reqRes, "api.seniverse.com");
		lv_timer_ready(timer_update);

        lv_obj_set_scrollbar_mode(scr_page, LV_SCROLLBAR_MODE_ACTIVE);
        static lv_style_t bg_style;     //背景蓝白渐变
        lv_style_init(&bg_style);
        lv_style_set_bg_color(&bg_style, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_bg_grad_color(&bg_style, lv_color_make(0xFF, 0xFF, 0xFF));
        lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER);
        lv_obj_add_style(scr_page, &bg_style, 0);

        //上半部分
        shijian = lv_label_create(scr_page);
        lv_obj_t* date = lv_label_create(scr_page);
        lv_label_set_recolor(date, true);
        static lv_style_t text_style;
        lv_style_init(&text_style);
        lv_style_set_text_color(&text_style, lv_color_make(0x9B, 0x44, 0x00));
        lv_style_set_text_font(&text_style, &lv_font_montserrat_38);
        lv_obj_set_style_text_font(date, &myfont, 0);
        lv_obj_add_style(shijian, &text_style, 0);      
        
        switch(weekDay)
        {
            case 1: lv_label_set_text_fmt(date, "#C93756 星期一%u/%d#", currentMonth, monthDay);break;
            case 2: lv_label_set_text_fmt(date, "#C93756 星期二%u/%d#", currentMonth, monthDay);break; 
            case 3: lv_label_set_text_fmt(date, "#C93756 星期三%u/%d#", currentMonth, monthDay);break;
            case 4: lv_label_set_text_fmt(date, "#C93756 星期四%u/%d#", currentMonth, monthDay);break;
            case 5: lv_label_set_text_fmt(date, "#C93756 星期五%u/%d#", currentMonth, monthDay);break;
            case 6: lv_label_set_text_fmt(date, "#C93756 星期六%u/%d#", currentMonth, monthDay);break;
            case 7: lv_label_set_text_fmt(date, "#C93756 星期天%u/%d#", currentMonth, monthDay);break;
        }

        lv_obj_align(shijian, LV_ALIGN_TOP_LEFT, 15, 30);
        lv_obj_align_to(date, shijian, LV_ALIGN_BOTTOM_MID, 20, 30);

        lv_obj_t* img_code = lv_img_create(scr_page);
        lv_obj_add_style(img_code, &img_bg, 0);
        lv_obj_align(img_code, LV_ALIGN_TOP_RIGHT, -25, 15);

        switch(results_daima)       //根据天气代码选择对应的图标
        {
            case 0: lv_img_set_src(img_code, &code0);break;
            case 1: lv_img_set_src(img_code, &code1);break;
            case 4: lv_img_set_src(img_code, &code4);break;
            case 5: lv_img_set_src(img_code, &code5);break;
            case 6: lv_img_set_src(img_code, &code6);break;
            case 7: lv_img_set_src(img_code, &code7);break;
            case 8: lv_img_set_src(img_code, &code8);break;
            case 9: lv_img_set_src(img_code, &code9);break;
            case 10: lv_img_set_src(img_code, &code10);break;
            case 11: lv_img_set_src(img_code, &code11);break;
            case 13: lv_img_set_src(img_code, &code13);break;
            case 14: lv_img_set_src(img_code, &code14);break;
            case 15: lv_img_set_src(img_code, &code15);break;
            case 16: lv_img_set_src(img_code, &code16);break;
            case 17: lv_img_set_src(img_code, &code17);break;
            case 18: lv_img_set_src(img_code, &code18);break;
            case 30: lv_img_set_src(img_code, &code30);break;
            case 31: lv_img_set_src(img_code, &code31);break;
            case 32: lv_img_set_src(img_code, &code32);break;
            case 33: lv_img_set_src(img_code, &code33);break;
            case 37: lv_img_set_src(img_code, &code37);break;
            case 38: lv_img_set_src(img_code, &code38);break;      
            default: lv_img_set_src(img_code, &code99);break;
        }

        //下半部分
        lv_obj_t* city = lv_label_create(scr_page);
        lv_obj_t* tianqi = lv_label_create(scr_page);
        lv_obj_t* wendu = lv_label_create(scr_page);
        lv_obj_set_style_text_font(city, &myfont, 0);
        lv_obj_set_style_text_font(tianqi, &myfont, 0);
        lv_label_set_recolor(city, true);
        lv_label_set_text_fmt(city, "#CC0000 %s#", results_chengshi);
        lv_label_set_text_fmt(tianqi, "%s", results_xianxiang);
        lv_label_set_text_fmt(wendu, "%d°C", results_wendu);
        lv_obj_align(city, LV_ALIGN_LEFT_MID, 15, 45);
        lv_obj_align_to(tianqi, city, LV_ALIGN_CENTER, 0, 20);
        lv_obj_align_to(wendu, tianqi, LV_ALIGN_CENTER, 0, 20);

        static lv_style_t style_line;   //创建一条分界线
        lv_style_init(&style_line);
        lv_style_set_line_width(&style_line, 8);
        lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_PINK));
        lv_style_set_line_rounded(&style_line, true);

        static lv_point_t line_points[] = { {90, 145}, {90, 225} };
        lv_obj_t* line = lv_line_create(scr_page);
        lv_line_set_points(line, line_points, 2);
        lv_obj_add_style(line, &style_line, 0);
        
        lv_obj_t* text1 = lv_label_create(scr_page);
        lv_obj_t* text2 = lv_label_create(scr_page);
        lv_obj_t* tigan = lv_label_create(scr_page);
        lv_obj_t* shidu = lv_label_create(scr_page);
        lv_obj_set_style_text_font(text1, &myfont, 0);
        lv_obj_set_style_text_font(text2, &myfont, 0);
        lv_label_set_recolor(tigan, true);
        lv_label_set_recolor(shidu, true);
        lv_label_set_text(text1, "体感温度");
        lv_label_set_text(text2, "相对湿度");
        lv_label_set_text_fmt(tigan, "#003371 %d°C#", results_tigan);
        lv_label_set_text_fmt(shidu, "#003371 %d%#", results_shidu);
        lv_obj_set_pos(text1, 120, 130);
        lv_obj_set_pos(text2, 220, 130);
        lv_obj_align_to(tigan, text1, LV_ALIGN_BOTTOM_MID, 0, 25);
        lv_obj_align_to(shidu, text2, LV_ALIGN_BOTTOM_MID, 0, 25);

        lv_obj_t* text3 = lv_label_create(scr_page);
        lv_obj_t* text4 = lv_label_create(scr_page);
        lv_obj_t* nengjiandu = lv_label_create(scr_page);
        lv_obj_t* fengsu = lv_label_create(scr_page);
        lv_obj_set_style_text_font(text3, &myfont, 0);
        lv_obj_set_style_text_font(text4, &myfont, 0);
        lv_label_set_recolor(nengjiandu, true);
        lv_label_set_recolor(fengsu, true);
        lv_label_set_text(text3, "能见度");
        lv_label_set_text(text4, "风速");
        lv_label_set_text_fmt(nengjiandu, "#003371 %dkm#", results_nengjiandu);
        lv_label_set_text_fmt(fengsu, "#003371 %dkm/h#", results_fengsu);
        lv_obj_set_pos(text3, 130, 185);
        lv_obj_set_pos(text4, 240, 185);
        lv_obj_align_to(nengjiandu, text3, LV_ALIGN_BOTTOM_MID, 0, 25);
        lv_obj_align_to(fengsu, text4, LV_ALIGN_BOTTOM_MID, 0, 25);
    }
    else
    {
        bg_null = lv_img_create(scr_page);
        lv_timer_pause(timer_update);
        lv_img_set_src(bg_null, &null);
    }
}