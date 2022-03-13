/**
 * @ Author: LAN
 * @ Modified time: 2022-03-13 17:27:05
 * @ Description: 解决了上电雪花屏的问题
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