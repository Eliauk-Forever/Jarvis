/**
 * @ Author: LAN
 * @ Modified time: 2022-01-11 23:50:07
 * @ Description: 优化逻辑框架 & 修复wifi以及日历页面错误
 */

#include "HAL/HAL.h"
#include "PORT/Display.h"
#include "PAGE/page_control.h"

void setup() 
{
	HAL::Init();
	Screen_Init();
	Gui_Init();
	HAL::Wifi_Connect();
}

void loop() 
{
	HAL::Update();
}