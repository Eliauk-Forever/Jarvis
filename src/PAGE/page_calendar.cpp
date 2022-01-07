#include "HAL/HAL.h"
#include "page_control.h"
#include "page_setting.h"
#include "page_calendar.h"

lv_obj_t* calendar, * current_date;

static void event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_current_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_calendar_date_t date;
        if (lv_calendar_get_pressed_date(obj, &date))
        {
            lv_calendar_set_today_date(calendar, date.year, date.month, date.day);
            lv_label_set_text_fmt(current_date, "%02d/%02d/%02d", date.year, date.month, date.day);
        }
    }
}

void page_calendar()
{
    calendar = lv_calendar_create(scr_calendar);

    current_date = lv_label_create(scr_calendar);

    lv_obj_set_pos(current_date, 180, 15);
    lv_label_set_text_fmt(current_date, "%02d/%02d/%02d", currentYear, currentMonth, monthDay);
    
    lv_obj_set_size(calendar, 280, 180);
    lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 20);
    lv_obj_add_event_cb(calendar, event_handler, LV_EVENT_ALL, NULL);

    lv_calendar_set_today_date(calendar, currentYear, currentMonth, monthDay);
    lv_calendar_set_showed_date(calendar, currentYear, currentMonth);

    /*日期高亮*/
    static lv_calendar_date_t highlighted_days[2];
    highlighted_days[0].year = 2022;
    highlighted_days[0].month = 01;
    highlighted_days[0].day = 1;

    highlighted_days[1].year = 2022;
    highlighted_days[1].month = 01;
    highlighted_days[1].day = 31;

    lv_calendar_set_highlighted_dates(calendar, highlighted_days, 2);
}
