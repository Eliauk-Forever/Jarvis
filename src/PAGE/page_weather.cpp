#include "HAL/HAL.h"
#include "page_control.h"
#include "page_weather.h"

// 心知天气HTTP请求所需信息
String reqUserKey1 = "S-VfiR3_DeGwQpmZT";   // 私钥
String reqLocation = "ShenZhen";            // 城市
String reqLanguage = "zh-Hans";            // 语言
String reqUnit = "c";                      // 摄氏/华氏

String reqRes = "/v3/weather/now.json?key=" + reqUserKey1 +
                + "&location=" + reqLocation + "&language=" + reqLanguage +
                "&language=en&unit=" +reqUnit;

String temperature = "", weather = "";    //用来解析Json得到的字符串

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

  	Serial.println(F("======Weahter Now======="));
  	Serial.print(F("Weather Now: "));
  	Serial.println(results_0_now_text_str);
  	Serial.print(F("Weather Code: "));
  	Serial.println(results_0_now_code_int);
  	Serial.print(F("Temperature: "));
  	Serial.println(results_0_now_temperature_int);
  	Serial.println(F("========================"));
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
	lv_obj_t* tianqi = lv_label_create(scr_page);
    lv_obj_t* wendu = lv_label_create(scr_page);
    lv_obj_set_style_text_font(tianqi, &myfont, 0);
    lv_obj_set_style_text_font(wendu, &myfont, 0);

    lv_obj_align(tianqi, LV_ALIGN_CENTER, 0, -20);
    lv_obj_align(wendu, LV_ALIGN_CENTER, 0, 20);

    if(Wifi_status == 2)
    {
        HttpRequest(reqRes, "api.seniverse.com");
        lv_label_set_text_fmt(tianqi, "天气: %s", weather.c_str());
        lv_label_set_text_fmt(wendu, "温度 : %s", temperature.c_str());
    }
    else
    {
        lv_label_set_text(tianqi, "天气: NULL");
        lv_label_set_text(wendu, "温度: NULL" );
    }
}