#include "HAL/HAL.h"
#include "page_control.h"
#include "page_infrared.h"

//红外发射管引脚定义
const uint16_t kIrLed = 25;
IRGreeAC ac(kIrLed);

//红外接收管引脚定义
IRrecv irrecv(16);
decode_results results;     //声明一个独有的变量类型保存接收到的信息

LV_FONT_DECLARE(myfont)

lv_obj_t* box, * wendu, *mode, *speed, * staus, * page_study, *text_1, *text_2;
lv_timer_t* timer_rev;

int Power_staus = 0;    //0为OFF,1为ON;
int Now_Temp = 24;      //默认设置温度为24度
int Now_Mode = 1;       //默认设置模式为制冷
int Now_Speed = 1;      //默认设置风速为自动风速
int num = 0;

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

void sign_rev(lv_timer_t * timer_rev)       //通过红外接收管获取脉冲信号,每1s执行一次
{
    if (irrecv.decode(&results)) 
    {
        Serial.println(results.value, HEX);
        lv_label_set_text_fmt(text_2, "%X", results.value);
        Serial.println("");
        irrecv.resume();
    }
}

static void save(lv_event_t* t)       //保存当前项,进入下一项
{
    num++;
    if (num > 4)
    {
        num = 0;
    }
    switch (num)
    {
        case 0:
            lv_label_set_text(text_1, "#EE7700 请按开关键#");
            lv_label_set_text_fmt(text_2, " ");
            break;

        case 1:
            lv_label_set_text(text_1, "#EE7700 请按加键#");
            lv_label_set_text(text_2, " ");
            break;

        case 2:
            lv_label_set_text(text_1, "#EE7700 请按减键#");
            lv_label_set_text(text_2, " ");
            break;

        case 3:
            lv_label_set_text(text_1, "#EE7700 请按模式键#");
            lv_label_set_text(text_2, " ");
            break;

        case 4:
            lv_label_set_text(text_1, "#EE7700 请按风速键#");
            lv_label_set_text(text_2, " ");
            break;
    } 
}

static void quit(lv_event_t* t)       //退出学习模式,返回控制界面
{
    lv_timer_pause(timer_rev);
    irrecv.disableIRIn();    //关闭红外接收管
    
    lv_scr_load_anim(scr_page, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);
}

static void mode_study(lv_event_t* t)       //进入学习模式
{
    irrecv.enableIRIn();    //开启红外接收管

    page_study = lv_obj_create(NULL);
    lv_scr_load_anim(page_study, LV_SCR_LOAD_ANIM_NONE, 50, 0, false);

    lv_obj_t* save_btn = lv_btn_create(page_study);
    lv_obj_t* save_label = lv_label_create(save_btn);
    lv_obj_t* quit_btn = lv_btn_create(page_study);
    lv_obj_t* quit_label = lv_label_create(quit_btn);
    
    lv_label_set_text(save_label, "SAVE");
    lv_obj_center(save_label);
    lv_label_set_text(quit_label, "EXIT");
    lv_obj_center(quit_label);
    
    lv_obj_set_size(save_btn, 100, 30);
    lv_obj_align(save_btn, LV_ALIGN_BOTTOM_MID, -70, -20);
    lv_obj_set_size(quit_btn, 100, 30);
    lv_obj_align(quit_btn, LV_ALIGN_BOTTOM_MID, 70, -20);

    lv_obj_add_event_cb(save_btn, save, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(quit_btn, quit, LV_EVENT_CLICKED, NULL);

    lv_obj_t* new_box = lv_obj_create(page_study);
    lv_obj_set_size(new_box, 240, 80);
    lv_obj_center(new_box);

    text_1 = lv_label_create(page_study);
    text_2 = lv_label_create(new_box);
    lv_obj_align(text_1, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_center(text_2);

    lv_obj_set_style_text_font(text_1, &myfont, 0);
    lv_label_set_recolor(text_1, true);

    lv_timer_resume(timer_rev);

    lv_label_set_text(text_1, "#EE7700 请按开关键#");
    lv_label_set_text(text_2, " ");

}

void page_infrared()
{
    ac.begin();             //开启红外发射管
    
    //创建一个定时器用于接收红外信号
    timer_rev = lv_timer_create(sign_rev, 1000, NULL);
    lv_timer_pause(timer_rev);      //暂停定时器

    //创建一个信息显示窗口
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

    lv_obj_t* line = lv_line_create(box);
    lv_line_set_points(line, line_points, 2);
    lv_obj_add_style(line, &style_line, 0);

    lv_obj_align(staus, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_align(wendu, LV_ALIGN_TOP_MID, 0, 70);
    lv_obj_align(mode, LV_ALIGN_CENTER, 0, 30);
    lv_obj_align_to(speed, mode, LV_ALIGN_CENTER, -20, 30);

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

    //调整按钮的大小和位置
    lv_obj_set_size(btn_power, 150, 30);
    lv_obj_set_size(btn_add, 60, 60);
    lv_obj_set_size(btn_reduce, 60, 60);
    lv_obj_set_size(btn_mode, 40, 110);
    lv_obj_set_size(btn_speed, 40, 110);
    lv_obj_set_size(btn_study, 150, 30);

    lv_obj_align(btn_power, LV_ALIGN_TOP_MID, 65, 15);
    lv_obj_align_to(btn_add, btn_power, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_align_to(btn_reduce, btn_add, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_align_to(btn_mode, btn_add, LV_ALIGN_CENTER, -60, 35);
    lv_obj_align_to(btn_speed, btn_add, LV_ALIGN_CENTER, 60, 35);
    lv_obj_align(btn_study, LV_ALIGN_BOTTOM_MID, 65, -15);

    //调整按钮文字的字号和位置
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
    lv_label_set_text(label_mode, "模\n式");
    lv_label_set_text(label_speed, "风\n速");
    lv_label_set_text(label_study, "学习模式");

    //创建按钮响应事件
    lv_obj_add_event_cb(btn_power, power, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_add, temp_add, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_reduce, temp_reduce, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_mode, change_mode, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_speed, change_speed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_study, mode_study, LV_EVENT_CLICKED, NULL);
}