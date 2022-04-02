#include "HAL/HAL.h"
#include "page_control.h"
#include "page_weather.h"

//天气图标
LV_IMG_DECLARE(bing)
LV_IMG_DECLARE(duoyun1)
LV_IMG_DECLARE(duoyun2)
LV_IMG_DECLARE(leitian)
LV_IMG_DECLARE(qingtian1)
LV_IMG_DECLARE(qingtian2)
LV_IMG_DECLARE(sha)
LV_IMG_DECLARE(wumai)
LV_IMG_DECLARE(yintian)
LV_IMG_DECLARE(yutian)
LV_IMG_DECLARE(xuetian)
LV_IMG_DECLARE(NA)
LV_IMG_DECLARE(null)
LV_IMG_DECLARE(feng)

lv_obj_t* shijian;
lv_timer_t* timer_update;

static void back(lv_event_t* event)
{
    lv_timer_del(timer_update);
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);   //退出后删除页面
}

void update(lv_timer_t * timer_update)
{
    lv_label_set_text_fmt(shijian, "%02d:%02d:%02d", currentHour, currentMinute, currentSecond);
}

void page_weather()
{
    timer_update = lv_timer_create(update, 1000, NULL);		//创建一个定时器更新时间
    lv_obj_add_event_cb(scr_page, back, LV_EVENT_LONG_PRESSED, NULL);

    if(Wifi_status == 2)
    {
		lv_timer_ready(timer_update);

        lv_obj_set_scrollbar_mode(scr_page, LV_SCROLLBAR_MODE_ACTIVE);
        static lv_style_t bg_style;     //背景蓝白渐变
        lv_style_init(&bg_style);
        lv_style_set_bg_color(&bg_style, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_bg_grad_color(&bg_style, lv_color_make(0xFF, 0xFF, 0xFF));
        lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER);
        lv_obj_add_style(scr_page, &bg_style, 0);

        //上半部分
        shijian = lv_label_create(scr_page);
        lv_obj_t* date = lv_label_create(scr_page);
        lv_label_set_recolor(date, true);
        static lv_style_t text_style;
        lv_style_init(&text_style);
        lv_style_set_text_color(&text_style, lv_color_make(0x9B, 0x44, 0x00));
        lv_style_set_text_font(&text_style, &lv_font_montserrat_38);
        lv_obj_set_style_text_font(date, &myfont, 0);
        lv_obj_add_style(shijian, &text_style, 0);      
        
        switch(weekDay)
        {
            case 1: lv_label_set_text_fmt(date, "#C93756 星期一%u/%d#", currentMonth, monthDay);break;
            case 2: lv_label_set_text_fmt(date, "#C93756 星期二%u/%d#", currentMonth, monthDay);break; 
            case 3: lv_label_set_text_fmt(date, "#C93756 星期三%u/%d#", currentMonth, monthDay);break;
            case 4: lv_label_set_text_fmt(date, "#C93756 星期四%u/%d#", currentMonth, monthDay);break;
            case 5: lv_label_set_text_fmt(date, "#C93756 星期五%u/%d#", currentMonth, monthDay);break;
            case 6: lv_label_set_text_fmt(date, "#C93756 星期六%u/%d#", currentMonth, monthDay);break;
            default: lv_label_set_text_fmt(date, "#C93756 星期天%u/%d#", currentMonth, monthDay);break;
        }

        lv_obj_align(shijian, LV_ALIGN_TOP_LEFT, 15, 30);
        lv_obj_align_to(date, shijian, LV_ALIGN_BOTTOM_MID, 20, 30);

        lv_obj_t* img_wea = lv_img_create(scr_page);
        lv_obj_add_style(img_wea, &img_bg, 0);
        lv_obj_align(img_wea, LV_ALIGN_TOP_RIGHT, -20, 15);

        if(img_code == 0)
        {
            lv_img_set_src(img_wea, &qingtian1);
        }
        else if(img_code == 1)
        {
            lv_img_set_src(img_wea, &qingtian2);
        }
        else if(img_code >= 4 && img_code <= 8)
        {
            if(img_code == 6 || img_code == 8)
            {
                lv_img_set_src(img_wea, &duoyun2);
            }
            else
            {
                lv_img_set_src(img_wea, &duoyun1);
            }        
        }
        else if(img_code == 9)
        {
            lv_img_set_src(img_wea, &yintian);
        }
        else if(img_code == 10)
        {
            lv_img_set_src(img_wea, &yutian);
        }
        else if(img_code == 11 || img_code == 12)
        {
            lv_img_set_src(img_wea, &leitian);
        }
        else if(img_code >= 13 && img_code <= 20)
        {
            lv_img_set_src(img_wea, &yutian);
        }  
        else if(img_code >= 21 && img_code <= 25)
        {
            lv_img_set_src(img_wea, &xuetian);
        }
        else if(img_code >= 26 && img_code <= 29)
        {
            lv_img_set_src(img_wea, &sha);
        }
        else if(img_code == 30 || img_code == 31)
        {
            lv_img_set_src(img_wea, &wumai);
        }
        else if(img_code == 30 || img_code == 31)
        {
            lv_img_set_src(img_wea, &feng);
        }
        else if(img_code == 37)
        {
            lv_img_set_src(img_wea, &bing);
        }
        else if(img_code == 38)
        {
            lv_img_set_src(img_wea, &qingtian1);
        }
        else
        {
            lv_img_set_src(img_wea, &NA);
        }

        //下半部分
        lv_obj_t* city = lv_label_create(scr_page);
        lv_obj_t* tianqi = lv_label_create(scr_page);
        lv_obj_t* wendu = lv_label_create(scr_page);
        lv_obj_set_style_text_font(city, &myfont, 0);
        lv_obj_set_style_text_font(tianqi, &myfont, 0);
        lv_label_set_recolor(city, true);
        lv_label_set_text_fmt(city, "#CC0000 %s区#", results_chengshi);
        lv_label_set_text_fmt(tianqi, "%s", results_wea);
        lv_label_set_text_fmt(wendu, "%d°C", results_wendu);
        lv_obj_align(city, LV_ALIGN_LEFT_MID, 15, 45);
        lv_obj_align_to(tianqi, city, LV_ALIGN_CENTER, 0, 20);
        lv_obj_align_to(wendu, tianqi, LV_ALIGN_CENTER, 0, 20);

        static lv_style_t style_line;   //创建一条分界线
        lv_style_init(&style_line);
        lv_style_set_line_width(&style_line, 8);
        lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_PINK));
        lv_style_set_line_rounded(&style_line, true);

        static lv_point_t line_points[] = { {90, 145}, {90, 225} };
        lv_obj_t* line = lv_line_create(scr_page);
        lv_line_set_points(line, line_points, 2);
        lv_obj_add_style(line, &style_line, 0);
        
        lv_obj_t* text1 = lv_label_create(scr_page);
        lv_obj_t* text2 = lv_label_create(scr_page);
        lv_obj_t* air = lv_label_create(scr_page);
        lv_obj_t* shidu = lv_label_create(scr_page);
        Text_Format(text1, 120, 130, true, false);
        Text_Format(text2, 220, 130, true, false);
        lv_obj_set_style_text_font(air, &myfont, 0);
        lv_label_set_recolor(air, true);
        lv_label_set_recolor(shidu, true);
        lv_label_set_text(text1, "空气质量");
        lv_label_set_text(text2, "相对湿度");
        lv_label_set_text_fmt(air, "#003371 %s#", results_air);
        lv_label_set_text_fmt(shidu, "#003371 %s%#", results_shidu);
        lv_obj_align_to(air, text1, LV_ALIGN_BOTTOM_MID, 0, 25);
        lv_obj_align_to(shidu, text2, LV_ALIGN_BOTTOM_MID, 0, 25);

        lv_obj_t* text3 = lv_label_create(scr_page);
        lv_obj_t* text4 = lv_label_create(scr_page);
        lv_obj_t* nengjiandu = lv_label_create(scr_page);
        lv_obj_t* fengsu = lv_label_create(scr_page);
        Text_Format(text3, 130, 185, true, false);
        Text_Format(text4, 240, 185, true, false);
        lv_label_set_recolor(nengjiandu, true);
        lv_label_set_recolor(fengsu, true);
        lv_label_set_text(text3, "能见度");
        lv_label_set_text(text4, "风速");
        lv_label_set_text_fmt(nengjiandu, "#003371 %s#", results_nengjiandu);
        lv_label_set_text_fmt(fengsu, "#003371 %s#", results_fengsu);
        lv_obj_align_to(nengjiandu, text3, LV_ALIGN_BOTTOM_MID, 0, 25);
        lv_obj_align_to(fengsu, text4, LV_ALIGN_BOTTOM_MID, 0, 25);
    }
    else
    {
        lv_obj_t* bg_null = lv_img_create(scr_page);
        lv_timer_pause(timer_update);
        lv_img_set_src(bg_null, &null);
    }
}