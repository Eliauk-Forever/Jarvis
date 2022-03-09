/**
 * @ Author: LAN
 * @ Modified time: 2022-03-10 00:24:35
 * @ Description: 增加天气界面和红外功能
 */

#include "HAL/HAL.h"
#include "PORT/Display.h"
#include "PAGE/page_control.h"

void setup()
{
	HAL::Init();
	Screen_Init();
	Gui_Init();
}

void loop()
{
	HAL::Update();
}