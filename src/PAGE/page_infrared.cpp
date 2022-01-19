#include "HAL/HAL.h"
#include "page_control.h"
#include "page_infrared.h"

LV_FONT_DECLARE(myfont)

//暂存温度数据
int nowtemp = 25;
int num_Fan = 5;

//红外发射管引脚定义
const uint16_t kIrLed = 23;
IRCoolixAC ac(kIrLed);

static void power_on(lv_event_t* t)       //打开
{
    ac.on();
    ac.setMode(kCoolixCool);
    ac.setTemp(25);
    ac.send();
    Serial.println("ON");
}

static void power_off(lv_event_t* t)       //关闭
{
    ac.off();
    ac.send();
    Serial.println("OFF");
}

static void add_temp(lv_event_t* t)       //升温
{
    nowtemp++;
    ac.setTemp(nowtemp);
    ac.send();
    Serial.println("ADD");
}

static void reduce_temp(lv_event_t* t)       //降温
{
    nowtemp--;
    ac.setTemp(nowtemp);
    ac.send();
    Serial.println("REDUCE");
}

static void wind_speed(lv_event_t* t)       //风速
{
    num_Fan++;
    if (num_Fan >= 6)
    {
      num_Fan = 1;
    }
    switch (num_Fan)
    {
        case 1:
            ac.setFan(kCoolixFanFixed);
            break;
        case 2:
            ac.setFan(kCoolixFanMin);
            break;

        case 3:
            ac.setFan(kCoolixFanMed);
            break;

        case 4:
            ac.setFan(kCoolixFanMax);
            break;
        
        case 5:
            ac.setFan(kCoolixFanAuto0);
            break;
    }
    ac.send();
    Serial.println("SPEED");
}

static void mode_cool(lv_event_t* t)       //制冷
{
    ac.setMode(kCoolixCool);
    ac.send();
    Serial.println("COOL");
}

static void mode_dry(lv_event_t* t)       //干燥
{
    ac.setMode(kCoolixDry);
    ac.send();
    Serial.println("DRY");
}

static void mode_hot(lv_event_t* t)       //制热
{
    ac.setMode(kCoolixHeat);
    ac.send();
    Serial.println("HOT");
}

void page_infrared()
{
    ac.begin();

    lv_obj_t* btn_on = lv_btn_create(scr_page);
    lv_obj_t* btn_off = lv_btn_create(scr_page);
    lv_obj_t* btn_add = lv_btn_create(scr_page);
    lv_obj_t* btn_reduce = lv_btn_create(scr_page);
    lv_obj_t* btn_fan = lv_btn_create(scr_page);
    lv_obj_t* btn_cool = lv_btn_create(scr_page);
    lv_obj_t* btn_dry = lv_btn_create(scr_page);
    lv_obj_t* btn_hot = lv_btn_create(scr_page);

    lv_obj_set_size(btn_on, 80, 80);
    lv_obj_set_size(btn_off, 80, 80);

    lv_obj_set_size(btn_add, 80, 90);
    lv_obj_set_size(btn_reduce, 80, 90);

    lv_obj_set_size(btn_fan, 70, 40);
    lv_obj_set_size(btn_cool, 70, 40);
    lv_obj_set_size(btn_dry, 70, 40);
    lv_obj_set_size(btn_hot, 70, 40);

    lv_obj_align(btn_on, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_align(btn_off, LV_ALIGN_TOP_RIGHT, -20, 20);

    lv_obj_align(btn_add, LV_ALIGN_CENTER, 0, -60);
    lv_obj_align(btn_reduce, LV_ALIGN_CENTER, 0, 60);

    lv_obj_align(btn_fan, LV_ALIGN_CENTER, -100, 20);
    lv_obj_align_to(btn_cool, btn_fan, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_align(btn_dry, LV_ALIGN_CENTER, 100, 20);
    lv_obj_align_to(btn_hot, btn_dry, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t* label_on = lv_label_create(btn_on);
    lv_obj_center(label_on);
    lv_obj_t* label_off = lv_label_create(btn_off);
    lv_obj_center(label_off);
    lv_obj_t* label_add = lv_label_create(btn_add);
    lv_obj_center(label_add);
    lv_obj_t* label_reduce = lv_label_create(btn_reduce);
    lv_obj_center(label_reduce);
    lv_obj_t* label_fan = lv_label_create(btn_fan);
    lv_obj_center(label_fan);
    lv_obj_t* label_cool = lv_label_create(btn_cool);
    lv_obj_center(label_cool);
    lv_obj_t* label_dry = lv_label_create(btn_dry);
    lv_obj_center(label_dry);
    lv_obj_t* label_hot = lv_label_create(btn_hot);
    lv_obj_center(label_hot);

    lv_obj_set_style_text_font(label_on, &myfont, 0);
    lv_obj_set_style_text_font(label_off, &myfont, 0);
    lv_obj_set_style_text_font(label_on, &myfont, 0);
    lv_obj_set_style_text_font(label_fan, &myfont, 0);
    lv_obj_set_style_text_font(label_cool, &myfont, 0);
    lv_obj_set_style_text_font(label_dry, &myfont, 0);
    lv_obj_set_style_text_font(label_hot, &myfont, 0);

    lv_label_set_text(label_on, "开");
    lv_label_set_text(label_off, "关");
    lv_label_set_text(label_add, "+");
    lv_label_set_text(label_reduce, "_");
    lv_label_set_text(label_fan, "风速");
    lv_label_set_text(label_cool, "制冷");
    lv_label_set_text(label_dry, "干燥");
    lv_label_set_text(label_hot, "制热");
}


