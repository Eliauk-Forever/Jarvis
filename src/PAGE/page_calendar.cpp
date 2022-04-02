#include "HAL/HAL.h"
#include "page_control.h"
#include "page_calendar.h"

lv_obj_t* calendar, * scr_lunar;

static void back_desktop(lv_event_t* a)
{
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);
}

static void page_lunar(lv_event_t* b)
{  
    lv_scr_load_anim(scr_lunar, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);
    lv_obj_add_event_cb(scr_lunar, back_desktop, LV_EVENT_LONG_PRESSED, NULL);
}

void page_calendar()
{
    static lv_style_t bg_style;     //背景灰白渐变
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_grad_color(&bg_style, lv_color_make(0xFF, 0xFF, 0xFF));
    lv_style_set_bg_grad_dir(&bg_style, LV_GRAD_DIR_VER);
    lv_obj_add_style(scr_page, &bg_style, 0);

    calendar = lv_calendar_create(scr_page);
    lv_obj_set_size(calendar, 220, 220);
    lv_obj_align(calendar, LV_ALIGN_CENTER, -40, 0);

    lv_obj_t* year, * month, * day, * text1, * text2;
    text1 = lv_label_create(scr_page);
    year = lv_label_create(scr_page);
    month = lv_label_create(scr_page);
    day = lv_label_create(scr_page);

    Text_Format(text1, NULL, NULL, true, true);
    Text_Format(year, 240, 55, true, false);
    Text_Format(month, 240, 85, true, false);
    Text_Format(day, 240, 115, true, false);
    lv_obj_align_to(text1, year, LV_ALIGN_CENTER, 10, -25);

    lv_label_set_text(text1, "#FF3300 新历");
    if(currentMonth == 0)
    {
        lv_label_set_text(year, "年:2022");
        lv_label_set_text(month, "月:01");
        lv_label_set_text(day, "日:01");
        lv_calendar_set_today_date(calendar, 2022, 1, 1);
        lv_calendar_set_showed_date(calendar, 2022, 1);
    }
    else
    {
        lv_label_set_text_fmt(year, "年:%02d", currentYear);
        lv_label_set_text_fmt(month, "月:%02d", currentMonth);
        lv_label_set_text_fmt(day, "日:%02d", monthDay);
        lv_calendar_set_today_date(calendar, currentYear, currentMonth, monthDay);
        lv_calendar_set_showed_date(calendar, currentYear, currentMonth);
    }

    static lv_style_t btn_style;
    lv_style_init(&btn_style);
    lv_style_set_bg_color(&btn_style, lv_color_make(0x06, 0x52, 0x79));
    
    lv_obj_t* btn = lv_btn_create(scr_page);
    lv_obj_add_style(btn, &btn_style, 0);
    lv_obj_set_size(btn, 70, 50);
    lv_obj_set_pos(btn, 240, 170);
    text2 = lv_label_create(btn);
    Text_Format(text2, NULL, NULL, true, false);
    lv_obj_center(text2);
    lv_label_set_text(text2, "农历");

    scr_lunar = lv_obj_create(NULL);
    //lv_obj_add_event_cb(btn, page_lunar, LV_EVENT_SHORT_CLICKED, NULL);

    lv_calendar_header_dropdown_create(calendar);
}
