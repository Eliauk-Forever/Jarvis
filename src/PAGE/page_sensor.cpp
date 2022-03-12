#include "HAL/HAL.h"
#include "page_control.h"
#include "page_sensor.h"

lv_obj_t* bar_t, *bar_h, * text_t, * text_h;
lv_timer_t* timer_th;

LV_IMG_DECLARE(img_t)
LV_IMG_DECLARE(img_h)

//温湿度变量
float temp, humi;
uint8_t t, h;

static void back_delete_cb(lv_event_t* event)       //退出当前页面
{
    lv_timer_del(timer_th);
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);
}

void sensor_measure(lv_timer_t * timer_th)    //每2s获取一次温湿度信息
{
	if (HAL::AHT_measure(&temp, &humi)) 
    {
        Serial.print(F("Temperature is "));
        Serial.print(temp);
        Serial.print(F(" °C, humidity is "));
        Serial.print(humi);
        Serial.println(" %");

		t = char(temp);     //将浮点型转为整型
		h = char(humi);

		lv_bar_set_value(bar_t, t, LV_ANIM_OFF);
        lv_bar_set_value(bar_h, h, LV_ANIM_OFF);

        lv_label_set_text_fmt(text_t, "%d°C", t);
        lv_label_set_text_fmt(text_h, "%dRH", h);
    }
}

void page_sensor()
{
	//打开温湿度传感器
    if(HAL::AHT_begin())
	{
		timer_th = lv_timer_create(sensor_measure, 2000, NULL);		//创建一个定时器获取温湿度测量值
		lv_timer_ready(timer_th);
	}

    lv_obj_add_event_cb(scr_page, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);

    //小贴士
    lv_obj_t* tips = lv_label_create(scr_page);
    static lv_style_t tips_style;
    lv_style_init(&tips_style);
    lv_style_set_text_color(&tips_style, lv_palette_main(LV_PALETTE_GREEN));
    lv_obj_add_style(tips, &tips_style, 0);
    lv_obj_set_style_text_font(tips, &myfont, 0);
    lv_label_set_text(tips, "Tips:人体适宜的温度夏天是\n26-28℃,冬天应在18℃-20℃,\n湿度在45%-65%");
    lv_obj_set_pos(tips, 30, 25);

	//创建温湿度图标
    lv_obj_t* img1 = lv_img_create(scr_page);
    lv_obj_t* img2 = lv_img_create(scr_page);
    lv_img_set_src(img1, &img_t);
    lv_img_set_src(img2, &img_h);
    lv_obj_add_style(img1, &img_bg, 0);
    lv_obj_add_style(img2, &img_bg, 0);
    lv_obj_align(img1, LV_ALIGN_LEFT_MID, 30, 40);
    lv_obj_align_to(img2, img1, LV_ALIGN_BOTTOM_MID, 0 ,45);

	//设置进度条的背景和显示效果
    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_PINK));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);    //增加颜色渐变效果

	//创建两个进度条指示温湿度值
    bar_t = lv_bar_create(scr_page);
    bar_h = lv_bar_create(scr_page);
    lv_obj_add_style(bar_t, &style_indic, LV_PART_INDICATOR);
    lv_obj_add_style(bar_h, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar_t, 150, 20);
    lv_obj_set_size(bar_h, 150, 20);
    lv_bar_set_range(bar_t, -20, 40);
    lv_bar_set_range(bar_h, 0, 100);
    lv_obj_align_to(bar_t, img1, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
    lv_obj_align_to(bar_h, bar_t, LV_ALIGN_BOTTOM_MID, 0, 50);
    
	//创建两个文本指示温湿度值
    text_t = lv_label_create(scr_page);
    text_h = lv_label_create(scr_page);
    lv_obj_align_to(text_t, bar_t, LV_ALIGN_RIGHT_MID, 60, 0);
    lv_obj_align_to(text_h, text_t, LV_ALIGN_BOTTOM_MID, 0, 50);
}
