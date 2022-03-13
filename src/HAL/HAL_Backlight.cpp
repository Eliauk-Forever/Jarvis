#include "HAL/HAL.h"
#include "lvgl.h"
#define CONFIG_SCREEN_BLK_PIN 2

uint32_t currentBacklight = 0;

//背光初始化
void HAL::Backlight_Init()
{
	//PWM初始化,1024级,5kHzƵ频率
	ledcSetup(0, 5000, 10);
	ledcAttachPin(CONFIG_SCREEN_BLK_PIN, 0);
	ledcWrite(0, 1024);  //关闭背光
}

//获取当前背光亮度
uint32_t HAL::Backlight_GetValue()
{
	return currentBacklight;
}

//背光亮度设置(0-1000 -> 0-100%)
void HAL::Backlight_SetValue(int32_t val)
{
	val = constrain(val, 0, 1024);
	currentBacklight = map(val, 0, 1024, 0, 100);		//映射函数
	val =  1024 - val;
	ledcWrite(0, val);
}

//背光渐亮
static void Backlight_AnimCallback(void *obj, int32_t brightness)
{
	HAL::Backlight_SetValue(brightness);
}

void HAL::Backlight_SetGradual(uint32_t target, uint16_t time)
{
	lv_anim_t a;
	lv_anim_init(&a);
	lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) Backlight_AnimCallback);
	lv_anim_set_values(&a, Backlight_GetValue(), target);
	lv_anim_set_time(&a, time);
	lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
	lv_anim_start(&a);
}

//背光强制点亮
void HAL::Backlight_ForceLit(bool en)
{
	pinMode(CONFIG_SCREEN_BLK_PIN, OUTPUT);
	digitalWrite(CONFIG_SCREEN_BLK_PIN, en);
}