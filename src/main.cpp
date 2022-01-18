/**
 * @ Author: LAN
 * @ Modified time: 2022-01-18 18:00:09
 * @ Description: 优化逻辑框架
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