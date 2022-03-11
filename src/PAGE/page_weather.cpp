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
String results_chengshi = "", results_xianxiang = "", results_fengxiang = "";
int results_daima, results_wendu, results_tigan, results_qiya, results_shidu, results_nengjiandu, results_dengji;

WiFiClient client;

lv_obj_t * bg_null;

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
  	results_daima = results_0_now["code"].as<int>();                        //天气现象代码
    results_wendu = results_0_now["temperature"].as<int>();                 //温度
    results_tigan = results_0_now["feels_like"].as<int>();                  //体感温度
    results_qiya = results_0_now["pressure"].as<int>();                     //气压
    results_shidu = results_0_now["humidity"].as<int>();                    //相对湿度
    results_nengjiandu = results_0_now["visibility"].as<int>();             //能见度
    results_fengxiang = results_0_now["wind_direction"].as<String>();    //风向文字
    results_dengji = results_0_now["wind_scale"].as<int>();                 //风力等级

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
    Serial.print("气压: ");
  	Serial.println(results_qiya);
    Serial.print("相对湿度: ");
  	Serial.println(results_shidu);
    Serial.print("能见度: ");
  	Serial.println(results_nengjiandu);
    Serial.print("风向: ");
  	Serial.print(results_fengxiang);
  	Serial.print(results_dengji);
    Serial.println("级");
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
        // Serial.println("JsonAnswer: ");
        // Serial.println(JsonAnswer);
    	ParseInfo(JsonAnswer);      // 利用ArduinoJson库解析响应信息
  	} 
	else 
	{
    	Serial.println(" connection failed!");
  	}   
  	client.stop();  //断开连接
}

void page_weather()
{
    if(Wifi_status == 2)
    {
        HttpRequest(reqRes, "api.seniverse.com");

        lv_obj_t* img_code = lv_img_create(scr_weather);
        lv_obj_add_style(img_code, &img_bg, 0);
        lv_img_set_src(img_code, &code0);
        lv_obj_align(img_code, LV_ALIGN_TOP_LEFT, 25, 15);

        //第一栏部件
        lv_obj_t* city = lv_label_create(scr_weather);
        lv_obj_t* tianqi = lv_label_create(scr_weather);
        lv_obj_t* wendu = lv_label_create(scr_weather);
        lv_obj_set_style_text_font(city, &myfont, 0);
        lv_obj_set_style_text_font(tianqi, &myfont, 0);
        lv_label_set_recolor(city, true);
        lv_label_set_text_fmt(city, "#CC0000 %s#", results_chengshi);
        lv_label_set_text_fmt(tianqi, "%s", results_xianxiang);
        lv_label_set_text_fmt(wendu, "%d°C", results_wendu);
        lv_obj_align(city, LV_ALIGN_TOP_RIGHT, -75, 25);
        lv_obj_align_to(tianqi, city, LV_ALIGN_CENTER, 0, 30);
        lv_obj_align_to(wendu, tianqi, LV_ALIGN_CENTER, 0, 30);
    }
    else
    {
        bg_null = lv_img_create(scr_weather);
        lv_img_set_src(bg_null, &null);
    }
}