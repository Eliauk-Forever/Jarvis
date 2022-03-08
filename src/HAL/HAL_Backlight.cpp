#include "HAL/HAL.h"

#define CONFIG_SCREEN_BLK_PIN 2

/**
  * @brief  �����ʼ��
  * @param  ��
  * @retval ��
  */
void HAL::Backlight_Init()
{
    /*PWM��ʼ����1024����5KHzƵ��*/
    ledcSetup(0, 5000, 10);
    ledcAttachPin(CONFIG_SCREEN_BLK_PIN, 0);
}

/**
  * @brief  ���ñ�������
  * @param  val: ����(0~1000 -> 0~100%)
  * @retval ��
  */
void HAL::SetBackLight(int32_t val)
{
    val = constrain(val, 0, 1024);
    val = 1024 - val;
    ledcWrite(0, val);
}
