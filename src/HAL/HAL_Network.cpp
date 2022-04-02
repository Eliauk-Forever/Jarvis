#include "HAL.h"

uint8_t Wifi_status = 0;	//0 -> 未连接, 1 -> AP模式, 2 -> 已连接， 3 -> 连接后断开

const char* AP_SSID  = "ESP32_Config"; //热点名称
String wifi_ssid = "";
String wifi_pass = "";

#define ROOT_HTML  "<!DOCTYPE html><html><head><title>WIFI Config</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><style type=\"text/css\">.input{display: block; margin-top: 50px;}.input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.input input{height: 30px;width: 200px;}.btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}</style><body><form method=\"GET\" action=\"connect\"><label class=\"input\"><span>WiFi SSID</span><input type=\"text\" name=\"ssid\"></label><label class=\"input\"><span>WiFi PASS</span><input type=\"text\"  name=\"pass\"></label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"Connect\"></P></form>"
WebServer server(80);

//用于配置WiFi
void HAL::Wifi_Config()
{
  	WiFi.mode(WIFI_AP);//配置为AP模式
  	boolean result = WiFi.softAP(AP_SSID, ""); //开启WIFI热点
  	if (result)
  	{
    	IPAddress myIP = WiFi.softAPIP();
    	//打印相关信息
    	Serial.println("");
    	Serial.print("Soft-AP IP address = ");
    	Serial.println(myIP);
    	Serial.println(String("MAC address = ")  + WiFi.softAPmacAddress().c_str());
    	Serial.println("waiting ...");
  	} 
	else 
	{  //开启热点失败
    	Serial.println("WiFiAP Failed");
    	delay(3000);
    	ESP.restart();  //复位esp32
  	}

    Wifi_status = 1;
    
  	if (MDNS.begin("esp32")) 
  	{
  	  	Serial.println("MDNS responder started");
  	}

  	//首页
  	server.on("/", []() 
	{
  	  	server.send(200, "text/html", ROOT_HTML);
  	});

  	//连接
  	server.on("/connect", []() {

    server.send(200, "text/html", "<html><body><font size=\"10\">successd,wifi connecting...<br />Please close this page!</font></body></html>");

    Wifi_status = 3;

    WiFi.softAPdisconnect(true);
    //获取输入的WIFI账户和密码
    wifi_ssid = server.arg("ssid");
    wifi_pass = server.arg("pass");
    server.close();

    WiFi.softAPdisconnect();
    Serial.println("WiFi Connect SSID:" + wifi_ssid + "  PASS:" + wifi_pass);

    //设置为STA模式并连接WIFI
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());
    uint8_t Connect_time = 0; //用于连接计时，如果长时间连接不成功，复位设备
    while (WiFi.status() != WL_CONNECTED) 
	{  //等待WIFI连接成功
      	delay(500);
      	Serial.print(".");
      	Connect_time ++;
      	if (Connect_time > 80) 
		{  //长时间连接不上，复位设备
      	  	Serial.println("Connection timeout, check input is correct or try again later!");
      	  	delay(3000);
      	  	ESP.restart();
      	}
    }
	Serial.println("");
    Serial.println("WIFI Config Success");
    Serial.printf("SSID:%s", WiFi.SSID().c_str());
    Serial.print("  LocalIP:");
    Serial.print(WiFi.localIP());
    Serial.println("");
    Wifi_status = 2;
  });
  server.begin();
}

void HAL::Wifi_Close()
{
	WiFi.mode(WIFI_OFF);
	Wifi_status = 0;
}

//用于上电自动连接WiFi
bool HAL::AutoConfig()
{
  	WiFi.begin();
  	for (int i = 0; i < 10; i++)
  	{
  	  	int wstatus = WiFi.status();
  	  	if (wstatus == WL_CONNECTED)
  	  	{
  	  	  	Serial.println("WIFI SmartConfig Success");
  	  	  	Serial.printf("SSID:%s", WiFi.SSID().c_str());
  	  	  	Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
  	  	  	Serial.print("LocalIP:");
  	  	  	Serial.print(WiFi.localIP());
  	  	  	Serial.print(" ,GateIP:");
  	  	  	Serial.println(WiFi.gatewayIP());
  	  	  	return true;
  	  	}
  	  	else
  	  	{
  	  	  	Serial.println("WIFI AutoConfig Waiting...");
  	  	  	//Serial.println(wstatus);
  	  	  	delay(1000);
  	  	}
  	}
  	Serial.println("WIFI AutoConfig Faild!" );
  	return false;
}

void HAL::HandleClient()
{
	server.handleClient();
}