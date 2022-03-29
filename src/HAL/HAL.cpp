#include "HAL/HAL.h"
#include <lvgl.h>

uint32_t CHIP_ID;

void HAL::Init()
{
	Wire.begin();
	Serial.begin(115200);

	for(int i=0; i<17; i=i+8) 		//获取芯片ID
	{
	  CHIP_ID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}

	Serial.println("");
    Serial.print(VERSION_FIRMWARE_NAME);
    Serial.print("System: " VERSION_SYSTEM_NAME);
	Serial.print("Hardware Version: " VERSION_HARDWARE);
    Serial.print("Software Version: " VERSION_SOFTWARE);
	Serial.print("CHIP_ID: ");
	Serial.println(CHIP_ID);
    Serial.print("Author: " VERSION_AUTHOR_NAME);
	Serial.println("\n");

	HAL::SD_Init();
	HAL::Buzz_init();
    HAL::Audio_Init();

}

void HAL::Wifi_Connect()
{
	HAL::Wifi_Config();
}

void HAL::Update()
{
	HAL::Audio_Update();
	lv_task_handler();
	HAL::HandleClient();
	delay(20);
}