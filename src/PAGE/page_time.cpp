#include "HAL/HAL.h"
#include "page_control.h"
#include "page_setting.h"
#include "page_time.h"

LV_IMG_DECLARE(bg_time);
LV_IMG_DECLARE(hour);
LV_IMG_DECLARE(minute);
LV_IMG_DECLARE(second);

uint8_t Hour, Minute, Second;

lv_obj_t* shizhen, * fenzhen, * miaozhen;

static void time_update(lv_timer_t* timer1)
{
    if (shizhen != NULL)
    {
        uint16_t shi = Hour * 300 + Minute / 12 % 12 * 60;
        lv_img_set_angle(shizhen, shi);
        lv_img_set_angle(fenzhen, Minute * 6 * 10);
        lv_img_set_angle(miaozhen, Second * 6 * 10);
        printf("%02d:%02d:%02d\n", Hour, Minute, Second);
    }

    if (++Second >= 60)
    {
        Second = 0;
        if (++Minute >= 60)
        {
            Minute = 0;
            if (++Hour >= 12)
            {
                Hour = 0;
            }
        }
    }
}

void page_time()
{
    //时间更新
    Hour = currentHour;
    Minute = currentMinute;
    Second = currentSecond;

    lv_obj_t* bg = lv_img_create(scr_time);
    lv_img_set_src(bg, &bg_time);
    lv_obj_center(bg);

    shizhen = lv_img_create(scr_time);
    lv_img_set_src(shizhen, &hour);
    lv_obj_center(shizhen);
    uint16_t shi = Hour * 300 + Minute / 12 % 12 * 60;
    lv_img_set_angle(shizhen, shi);

    fenzhen = lv_img_create(scr_time);
    lv_img_set_src(fenzhen, &minute);
    lv_obj_center(fenzhen);
    lv_img_set_angle(fenzhen, Minute * 60);

    miaozhen = lv_img_create(scr_time);
    lv_img_set_src(miaozhen, &second);
    lv_obj_center(miaozhen);
    lv_img_set_angle(miaozhen, Second * 60);

    lv_timer_t* timer1 = lv_timer_create(time_update, 1000, NULL);
    lv_timer_ready(timer1);
}