#include "HAL/HAL.h"

#define CONFIG_SCREEN_BLK_PIN 2

/**
  * @brief  背光初始化
  * @param  无
  * @retval 无
  */
void HAL::Backlight_Init()
{
    /*PWM初始化，1024级，5KHz频率*/
    ledcSetup(0, 5000, 10);
    ledcAttachPin(CONFIG_SCREEN_BLK_PIN, 0);
}

/**
  * @brief  设置背光亮度
  * @param  val: 亮度(0~1000 -> 0~100%)
  * @retval 无
  */
void HAL::SetBackLight(int32_t val)
{
    val = constrain(val, 0, 1024);
    val = 1024 - val;
    ledcWrite(0, val);
}
