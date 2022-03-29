/**
 * @ Author: LAN
 * @ Modified time: 2022-03-29 23:52:08
 * @ Description: 增加了蜂鸣器播放音乐功能
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