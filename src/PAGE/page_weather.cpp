#include "HAL/HAL.h"
#include "page_control.h"
#include "page_weather.h"

LV_FONT_DECLARE(myfont)
LV_FONT_DECLARE(weatherfont)

LV_IMG_DECLARE(qingtian)
LV_IMG_DECLARE(yintian)
LV_IMG_DECLARE(yutian)
LV_IMG_DECLARE(yewan)
LV_IMG_DECLARE(null)

// 心知天气HTTP请求所需信息
String reqUserKey1 = "S-VfiR3_DeGwQpmZT";   // 私钥
String reqLocation = "ShenZhen";            // 城市
String reqLanguage = "zh-Hans";            // 语言
String reqUnit = "c";                      // 摄氏/华氏

String reqRes = "/v3/weather/now.json?key=" + reqUserKey1 +
                + "&location=" + reqLocation + "&language=" + reqLanguage +
                "&language=en&unit=" +reqUnit;

String temperature = "", weather = "";    //用来解析Json得到的字符串
int weather_code;

WiFiClient client;

void ParseInfo(String& json)
{
  	const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  	DynamicJsonDocument doc(capacity);	
  	deserializeJson(doc, json);

  	JsonObject results_0 = doc["results"][0];
  	JsonObject results_0_now = results_0["now"];
  	const char* results_0_now_text = results_0_now["text"];
  	const char* results_0_now_code = results_0_now["code"];
  	const char* results_0_now_temperature = results_0_now["temperature"];
	
  	// 通过串口监视器显示以上信息
  	String results_0_now_text_str = results_0_now["text"].as<String>(); 
  	int results_0_now_code_int = results_0_now["code"].as<int>(); 
  	int results_0_now_temperature_int = results_0_now["temperature"].as<int>(); 

    weather = results_0_now_text_str;
    temperature = results_0_now_temperature_int;
    weather_code = results_0_now_code_int;

  	Serial.println("======Weahter Now=======");
  	Serial.print("Weather Now: ");
  	Serial.println(weather);
  	Serial.print("Weather Code: ");
  	Serial.println(weather_code);
  	Serial.print("Temperature: ");
  	Serial.println(temperature);
  	Serial.println("========================");
}

void HttpRequest(String reqRes, const char* host)
{	
  	// 建立http请求信息
  	String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
  	                            "Host: " + host + "\r\n" + 
  	                            "Connection: close\r\n\r\n";
 	// 尝试连接服务器
 	if (client.connect(host, 80))
	{
    	//Serial.println("Success!");
    	// 向服务器发送http请求信息
    	client.print(httpRequest);
    	//Serial.println("Sending request: ");
    	//Serial.println(httpRequest);
    	// 获取并显示服务器响应状态行 
    	String status_response = client.readStringUntil('\n');
    	//Serial.print("status_response: ");
    	//Serial.println(status_response);
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
  		//Serial.println();
  		//Serial.println("JsonAnswer: ");
  		//Serial.println(JsonAnswer);
    	// 利用ArduinoJson库解析响应信息
    	ParseInfo(JsonAnswer);
  	} 
	else 
	{
    	Serial.println(" connection failed!");
  	}   
  	//断开客户端与服务器连接工作
  	client.stop(); 
}

void page_weather()
{
    lv_obj_t* bg = lv_img_create(scr_page);

    lv_obj_t* current_tianqi = lv_label_create(bg);
    lv_obj_t* current_wendu = lv_label_create(bg);
    lv_obj_t* chengshi = lv_label_create(bg);
    lv_obj_t* tags = lv_label_create(bg);

    lv_obj_set_style_text_font(chengshi, &weatherfont, 0);
    lv_obj_set_style_text_font(current_tianqi, &weatherfont, 0);
    lv_obj_set_style_text_font(current_wendu, &weatherfont, 0);
    lv_obj_set_style_text_font(tags, &myfont, 0);

    lv_label_set_recolor(chengshi, true);
    lv_label_set_recolor(tags, true);

    lv_obj_align(chengshi, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_align(current_wendu, LV_ALIGN_CENTER, 15, -30);
    lv_obj_align(current_tianqi, LV_ALIGN_CENTER, 0, 30);
    lv_obj_align(tags, LV_ALIGN_BOTTOM_MID, 0, -20);

    if(Wifi_status == 2)
    {
        HttpRequest(reqRes, "api.seniverse.com");
        lv_label_set_text(chengshi, "#FF7F00 深圳市#");
        if(weather_code == 0 || weather_code == 2)
        {
            lv_img_set_src(bg, &qingtian);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 天气晴朗,出门玩耍吧!#");
        }
        else if(weather_code == 1 || weather_code == 3 || weather_code == 6 || weather_code == 8)
        {
            lv_img_set_src(bg, &yewan);
            lv_obj_set_style_text_color(current_wendu, lv_color_white(), 0);
            lv_obj_set_style_text_color(current_tianqi, lv_color_white(), 0);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 时间不早了,记得早点回家!#");
        }
        else if(weather_code == 4 || weather_code == 5 || weather_code == 7)
        {
            lv_img_set_src(bg, &yintian);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 室外多云,非常适合运动!#");
        }
        else if(weather_code == 9)
        {
            lv_img_set_src(bg, &yintian);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 可能会下雨,出门记得带伞!#");
        }
        else if(weather_code >= 10 && weather_code <= 20)
        {
            lv_img_set_src(bg, &yutian);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 外面在下雨,出门注意安全!#");
        }
        else if(weather_code >= 26  && weather_code <= 36)
        {
            lv_img_set_src(bg, &yintian);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 恶劣天气,尽量不要出门!#");
        }
        else if(weather_code == 37)
        {
            lv_img_set_src(bg, &yintian);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 天气寒冷,注意保暖!#");
        }
        else if(weather_code == 38)
        {
            lv_img_set_src(bg, &qingtian);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 天气炎热,注意避暑!#");
        }
        else
        {
            lv_img_set_src(bg, &qingtian);
            lv_label_set_text_fmt(current_wendu, "%s °", temperature.c_str());
            lv_label_set_text_fmt(current_tianqi, "%s", weather.c_str());
            lv_label_set_text(tags, "#FF0000 未知天气!#");
        }
    }
    else
    {
        lv_img_set_src(bg, &null);
        lv_obj_clean(bg);
    }
}