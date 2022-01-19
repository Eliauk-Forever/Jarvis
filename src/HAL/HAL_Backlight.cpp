#include "HAL/HAL.h"

#define CONFIG_SCREEN_BLK_PIN 2

static uint32_t backlightValue = 0;

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
    ledcWrite(0, 0);
}

/**
  * @brief  ���ñ�������
  * @param  val: ����(0~1000 -> 0~100%)
  * @retval ��
  */
void HAL::Backlight_SetValue(int32_t val)
{
    val = constrain(val, 0, 1024);
    backlightValue = val;
    val =   backlightValue;
    ledcWrite(0, val);
}
