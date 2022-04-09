#include "HAL/HAL.h"
#include "page_control.h"
#include "page_infrared.h"

//红外发射管引脚定义
const uint16_t kIrLed = 25;
IRGreeAC ac(kIrLed);
IRsend irsend(kIrLed);

//红外接收管引脚定义
// IRrecv irrecv(16);
// decode_results results;     //声明一个独有的变量类型保存接收到的信息

LV_FONT_DECLARE(myfont)

lv_obj_t* box, * wendu, *mode, *speed, * staus, * page_light, *text_1, *text_2, * Led_Staus, * Led_Color, * Led_Mode;
lv_timer_t* timer_rev;

int Power_staus = 0;    //0为OFF,1为ON;
int Now_Temp = 24;      //默认设置温度为24度
int Now_Mode = 1;       //默认设置模式为制冷
int Now_Speed = 1;      //默认设置风速为自动风速
int num = 0;

int L_Staus = 0;    //0为OFF,1为ON;
int L_Color = 0;    //0为OFF,1为红灯,2为绿灯,3为蓝灯;
int L_Mode = 0;     //0为普通模式,1为呼吸灯模式,2为流水灯模式,3跑马灯模式;

#define RED 0xA1
#define GREEN 0xA2
#define BLUE 0xA3
#define CLOSE 0xB1
#define MODE_1 0xC1
#define MODE_2 0xC2
#define MODE_3 0xC3
#define MODE_4 0xC4

static void power(lv_event_t* t)       //开关机
{
    HAL::Buzz_Tone(300, 5);
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
    HAL::Buzz_Tone(300, 5);
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
    HAL::Buzz_Tone(300, 5);
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
    HAL::Buzz_Tone(300, 5);
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
    HAL::Buzz_Tone(300, 5);
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

static void back_desktop(lv_event_t* t)       //返回桌面界面
{
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);
}

static void light_power(lv_event_t* t)       //开关灯
{
    HAL::Buzz_Tone(300, 5);
    if (L_Staus == 1)
    {
        irsend.sendSony(CLOSE, 12);
        lv_label_set_text(Led_Staus, "#B22222 关闭状态#");
        lv_label_set_text(Led_Color, "未开启");
        L_Staus = 0;
        L_Color = 0;
    }
}

static void light_color(lv_event_t* t)       //切换灯的颜色
{
    HAL::Buzz_Tone(300, 5);
    L_Color++;
    if (L_Color > 3)
    {
        L_Color = 1;
    }
    switch (L_Color)
    {
        case 1:
            irsend.sendSony(RED, 12);
            lv_label_set_text(Led_Color, "#FF461F 红色#");
            lv_label_set_text(Led_Staus, "#2E8B57 开启状态#");
            L_Staus = 1;
            break;
        case 2:
            irsend.sendSony(GREEN, 12);
            lv_label_set_text(Led_Color, "#057748 绿色#");
            lv_label_set_text(Led_Staus, "#2E8B57 开启状态#");
            L_Staus = 1;
            break;
        case 3:
            irsend.sendSony(BLUE, 12);
            lv_label_set_text(Led_Color, "#177CB0 蓝色#");
            lv_label_set_text(Led_Staus, "#2E8B57 开启状态#");
            L_Staus = 1;
            break;
    }
}

static void light_mode(lv_event_t* t)       //切换灯的模式
{
    HAL::Buzz_Tone(300, 5);
    L_Mode++;
    if (L_Mode > 3)
    {
        L_Mode = 0;
    }
    switch (L_Mode)
    {
        case 0: 
            lv_label_set_text(Led_Mode, "普通模式#");
            irsend.sendSony(MODE_1, 12);
            break;
        case 1: 
            lv_label_set_text(Led_Mode, "呼吸灯");
            irsend.sendSony(MODE_2, 12);
            break;
        case 2: 
            lv_label_set_text(Led_Mode, "流水灯");
            irsend.sendSony(MODE_3, 12);
            break;
        case 3: 
            lv_label_set_text(Led_Mode, "跑马灯");
            irsend.sendSony(MODE_4, 12);
            break;
    }
}

static void mode_light(lv_event_t* t)       //进入灯控模式
{
    HAL::Buzz_Tone(300, 5);

    static lv_style_t bg_style, btn_style;     //灰白背景
    lv_style_init(&btn_style);
    lv_style_set_bg_color(&bg_style, lv_color_make(0xD6, 0xEC, 0xF0));
    lv_style_set_bg_color(&btn_style, lv_color_make(0x06, 0x52, 0x79));
    lv_style_set_bg_grad_color(&bg_style, lv_color_make(0xE0, 0xEE, 0xE8));
    lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER);

    page_light = lv_obj_create(NULL);
    lv_obj_add_style(page_light, &bg_style, 0);
    lv_scr_load_anim(page_light, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);
    lv_obj_add_event_cb(page_light, back_desktop, LV_EVENT_LONG_PRESSED, NULL);

    //创建一个信息显示窗口
    lv_obj_t* box = lv_obj_create(page_light);
    lv_obj_set_size(box, 130, 170);
    lv_obj_align(box, LV_ALIGN_LEFT_MID, 5, 0);

    Led_Staus = lv_label_create(box);
    Led_Color = lv_label_create(box);
    Led_Mode = lv_label_create(box);
    Text_Format(Led_Staus, NULL, NULL, true, true);
    Text_Format(Led_Color, NULL, NULL, true, true);
    Text_Format(Led_Mode, NULL, NULL, true, true);
    lv_obj_align(Led_Staus, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_align(Led_Color, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_align(Led_Mode, LV_ALIGN_CENTER, 0, 45);

    static lv_point_t line_points[] = { {10, 35}, {90, 35} };
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 5);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BROWN));
    lv_style_set_line_rounded(&style_line, true);
    lv_obj_t* line = lv_line_create(box);
    lv_line_set_points(line, line_points, 2);
    lv_obj_add_style(line, &style_line, 0);

    lv_label_set_text(Led_Staus, "#DC143C 关闭状态#");
    lv_label_set_text(Led_Color, "未开启");
    lv_label_set_text(Led_Mode, "普通模式");

    lv_obj_t* btn_power = lv_btn_create(page_light);
    lv_obj_t* btn_color = lv_btn_create(page_light);
    lv_obj_t* btn_mode = lv_btn_create(page_light);
    lv_obj_add_style(btn_power, &btn_style, 0);
    lv_obj_add_style(btn_color, &btn_style, 0);
    lv_obj_add_style(btn_mode, &btn_style, 0);
    lv_obj_set_size(btn_power, 120, 60);
    lv_obj_set_size(btn_color, 120, 60);
    lv_obj_set_size(btn_mode, 120, 60);
    lv_obj_set_pos(btn_power, 165, 20);
    lv_obj_set_pos(btn_color, 165, 90);
    lv_obj_set_pos(btn_mode, 165, 160);

    lv_obj_t* label_power = lv_label_create(btn_power);
    lv_obj_center(label_power);
    lv_obj_t* label_color = lv_label_create(btn_color);
    lv_obj_center(label_color);
    lv_obj_t* label_mode = lv_label_create(btn_mode);
    lv_obj_center(label_mode);
    Text_Format(label_power, NULL, NULL, true, false);
    Text_Format(label_color, NULL, NULL, true, false);
    Text_Format(label_mode, NULL, NULL, true, false);
    lv_label_set_text(label_power, "关灯");
    lv_label_set_text(label_color, "颜色");
    lv_label_set_text(label_mode, "模式");

    //创建按钮响应事件
    lv_obj_add_event_cb(btn_power, light_power, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_color, light_color, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_mode, light_mode, LV_EVENT_CLICKED, NULL);
}

void page_infrared()
{
    static lv_style_t bg_style, btn_style;     //背景红灰渐变
    lv_style_init(&bg_style);
    lv_style_init(&btn_style);
    lv_style_set_bg_color(&bg_style, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_color(&btn_style, lv_palette_main(LV_PALETTE_CYAN));
    lv_style_set_bg_grad_color(&bg_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER);
    lv_obj_add_style(scr_page, &bg_style, 0);

    ac.begin();             //开启红外发射管

    //创建一个信息显示窗口
    box = lv_obj_create(scr_page);
    lv_obj_set_size(box, 130, 170);
    lv_obj_align(box, LV_ALIGN_LEFT_MID, 5, 0);

    wendu = lv_label_create(box);
    mode = lv_label_create(box);
    speed = lv_label_create(box);
    staus = lv_label_create(box);

    lv_obj_set_style_text_font(staus, &myfont, 0);
    lv_obj_set_style_text_font(mode, &myfont, 0);
    lv_obj_set_style_text_font(speed, &myfont, 0);

    lv_label_set_recolor(staus, true);

    static lv_point_t line_points[] = { {10, 35}, {90, 35} };

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 5);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BROWN));
    lv_style_set_line_rounded(&style_line, true);

    lv_obj_t* line = lv_line_create(box);
    lv_line_set_points(line, line_points, 2);
    lv_obj_add_style(line, &style_line, 0);

    lv_obj_align(staus, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_align(wendu, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_align(mode, LV_ALIGN_CENTER, 0, 25);
    lv_obj_align_to(speed, mode, LV_ALIGN_CENTER, -20, 25);

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

    //设置按钮背景颜色
    lv_obj_add_style(btn_power, &btn_style, 0);
    lv_obj_add_style(btn_add, &btn_style, 0);
    lv_obj_add_style(btn_reduce, &btn_style, 0);
    lv_obj_add_style(btn_mode, &btn_style, 0);
    lv_obj_add_style(btn_speed, &btn_style, 0);
    lv_obj_add_style(btn_study, &btn_style, 0);

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
    lv_obj_t* label_light = lv_label_create(btn_study);
    lv_obj_center(label_light);

    lv_obj_set_style_text_font(label_power, &myfont, 0);
    lv_obj_set_style_text_font(label_mode, &myfont, 0);
    lv_obj_set_style_text_font(label_speed, &myfont, 0);
    lv_obj_set_style_text_font(label_light, &myfont, 0);

    lv_label_set_text(label_power, "开/关");
    lv_label_set_text(label_add, LV_SYMBOL_PLUS);
    lv_label_set_text(label_reduce, LV_SYMBOL_MINUS);
    lv_label_set_text(label_mode, "模\n式");
    lv_label_set_text(label_speed, "风\n速");
    lv_label_set_text(label_light, "灯控模式");

    //创建按钮响应事件
    lv_obj_add_event_cb(btn_power, power, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_add, temp_add, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_reduce, temp_reduce, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_mode, change_mode, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_speed, change_speed, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_study, mode_light, LV_EVENT_CLICKED, NULL);
}