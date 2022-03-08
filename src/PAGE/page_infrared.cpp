#include "HAL/HAL.h"
#include "page_control.h"
#include "page_infrared.h"

LV_FONT_DECLARE(myfont)

lv_obj_t* box, * wendu, *mode, *speed, * staus;

int Power_staus = 0;    //0为OFF,1为ON;
int Now_Temp = 24;
int Now_Mode = 1;
int Now_Speed = 1;

//红外发射管引脚定义
const uint16_t kIrLed = 25;
IRGreeAC ac(kIrLed);

static void power(lv_event_t* t)       //开关机
{
    if (Power_staus == 0)
    {
        ac.on();
        ac.send();
        lv_label_set_text(staus, "#2E8B57 开机状态#");
        Power_staus = 1;
    }
    else
    {
        ac.off();
        ac.send();
        lv_label_set_text(staus, "#B22222 关机状态#");
        Power_staus = 0;
    }
}

static void temp_add(lv_event_t* t)       //升高温度
{
    Now_Temp++;
    if (Now_Temp > 30)
    {
        Now_Temp = 30;
    }
    ac.setTemp(Now_Temp);
    ac.send();
    lv_label_set_text_fmt(wendu, "%d °C", Now_Temp);
}

static void temp_reduce(lv_event_t* t)       //降低温度
{
    Now_Temp--;
    if (Now_Temp < 16)
    {
        Now_Temp = 16;
    }
    ac.setTemp(Now_Temp);
    ac.send();
    lv_label_set_text_fmt(wendu, "%d °C", Now_Temp);
}

static void change_mode(lv_event_t* t)       //工作模式
{
    Now_Mode++;
    if (Now_Mode > 3)
    {
        Now_Mode = 1;
    }
    switch (Now_Mode)
    {
    case 1:
        ac.setMode(kGreeCool);
        ac.send();
        lv_label_set_text(mode, "制冷");
        break;

    case 2:
        ac.setMode(kGreeDry);
        ac.send();
        lv_label_set_text(mode, "除湿");
        break;

    case 3:
        ac.setMode(kGreeHeat);
        ac.send();
        lv_label_set_text(mode, "制热");
        break;
    }
}

static void change_speed(lv_event_t* t)       //风速
{
    Now_Speed++;
    if (Now_Speed > 4)
    {
        Now_Speed = 1;
    }
    switch (Now_Speed)
    {
    case 1:
        ac.setFan(kGreeFanAuto);
        ac.send();
        lv_label_set_text(speed, "自动风速");
        break;

    case 2:
        ac.setFan(kGreeFanMin);
        ac.send();
        lv_label_set_text(speed, "低档风速");
        break;

    case 3:
        ac.setFan(kGreeFanMed);
        ac.send();
        lv_label_set_text(speed, "中档风速");
        break;

    case 4:
        ac.setFan(kGreeFanMax);
        ac.send();
        lv_label_set_text(speed, "高档风速");
        break;
    }
}

static void mode_study(lv_event_t* t)       //学习模式
{

}

void page_infrared()
{
    ac.begin();

    box = lv_obj_create(scr_page);
    lv_obj_set_size(box, 130, 200);
    lv_obj_align(box, LV_ALIGN_LEFT_MID, 5, 0);

    wendu = lv_label_create(box);
    mode = lv_label_create(box);
    speed = lv_label_create(box);
    staus = lv_label_create(box);

    lv_obj_set_style_text_font(staus, &myfont, 0);
    lv_obj_set_style_text_font(mode, &myfont, 0);
    lv_obj_set_style_text_font(speed, &myfont, 0);

    lv_label_set_recolor(staus, true);

    static lv_point_t line_points[] = { {10, 50}, {90, 50} };

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 5);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BROWN));
    lv_style_set_line_rounded(&style_line, true);

    lv_obj_t* line;
    line = lv_line_create(box);
    lv_line_set_points(line, line_points, 2);
    lv_obj_add_style(line, &style_line, 0);

    lv_obj_align(staus, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_align(wendu, LV_ALIGN_TOP_MID, 0, 70);
    lv_obj_align_to(mode, wendu, LV_ALIGN_BOTTOM_MID, -18, 30);
    lv_obj_align_to(speed, mode, LV_ALIGN_CENTER, -25, 30);

    lv_label_set_text(staus, "#DC143C 关机状态#");
    lv_label_set_text_fmt(wendu, "%d °C", Now_Temp);
    lv_label_set_text(mode, "制冷");
    lv_label_set_text(speed, "自动风速");

    lv_obj_t* btn_power = lv_btn_create(scr_page);
    lv_obj_t* btn_add = lv_btn_create(scr_page);
    lv_obj_t* btn_reduce = lv_btn_create(scr_page);
    lv_obj_t* btn_mode = lv_btn_create(scr_page);
    lv_obj_t* btn_speed = lv_btn_create(scr_page);
    lv_obj_t* btn_study = lv_btn_create(scr_page);

    lv_obj_set_size(btn_power, 150, 30);
    lv_obj_set_size(btn_add, 60, 60);
    lv_obj_set_size(btn_reduce, 60, 60);
    lv_obj_set_size(btn_mode, 60, 60);
    lv_obj_set_size(btn_speed, 60, 60);
    lv_obj_set_size(btn_study, 150, 30);

    lv_obj_align(btn_power, LV_ALIGN_TOP_MID, 65, 15);
    lv_obj_align_to(btn_add, btn_power, LV_ALIGN_OUT_BOTTOM_MID, 40, 10);
    lv_obj_align_to(btn_reduce, btn_add, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_align_to(btn_mode, btn_add, LV_ALIGN_OUT_LEFT_MID, -20, 0);
    lv_obj_align_to(btn_speed, btn_reduce, LV_ALIGN_OUT_LEFT_MID, -20, 0);
    lv_obj_align(btn_study, LV_ALIGN_BOTTOM_MID, 65, -15);

    lv_obj_t* label_power = lv_label_create(btn_power);
    lv_obj_center(label_power);
    lv_obj_t* label_add = lv_label_create(btn_add);
    lv_obj_center(label_add);
    lv_obj_t* label_reduce = lv_label_create(btn_reduce);
    lv_obj_center(label_reduce);
    lv_obj_t* label_mode = lv_label_create(btn_mode);
    lv_obj_center(label_mode);
    lv_obj_t* label_speed = lv_label_create(btn_speed);
    lv_obj_center(label_speed);
    lv_obj_t* label_study = lv_label_create(btn_study);
    lv_obj_center(label_study);

    lv_obj_set_style_text_font(label_power, &myfont, 0);
    lv_obj_set_style_text_font(label_mode, &myfont, 0);
    lv_obj_set_style_text_font(label_speed, &myfont, 0);
    lv_obj_set_style_text_font(label_study, &myfont, 0);

    lv_label_set_text(label_power, "开/关");
    lv_label_set_text(label_add, LV_SYMBOL_PLUS);
    lv_label_set_text(label_reduce, LV_SYMBOL_MINUS);
    lv_label_set_text(label_mode, "模式");
    lv_label_set_text(label_speed, "风速");
    lv_label_set_text(label_study, "学习模式");

    lv_obj_add_event_cb(btn_power, power, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_add, temp_add, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_reduce, temp_reduce, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_mode, change_mode, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_speed, change_speed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_study, mode_study, LV_EVENT_CLICKED, NULL);
}