#include "HAL/HAL.h"
#include "page_control.h"
#include "page_infrared.h"

static void event_handler(lv_event_t* b)
{
    lv_event_code_t code = lv_event_get_code(b);
    lv_obj_t* obj = lv_event_get_target(b);
    if (code == LV_EVENT_VALUE_CHANGED) 
    {
        Serial.printf("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
    }
}

void page_infrared()
{
    lv_obj_t* switch_1, * label_1, *label_2;
    switch_1 = lv_switch_create(scr_page);
    label_1 = lv_label_create(scr_page);
    label_2 = lv_label_create(scr_page);

    lv_obj_set_style_text_font(label_1, &myfont, 0);
    lv_obj_set_style_text_font(label_2, &myfont, 0);

    lv_label_set_text(label_1, "空调选择");
    lv_obj_align(label_1, LV_ALIGN_CENTER, 0, 30);

    lv_label_set_text(label_2, "开关");
    lv_obj_align(label_2, LV_ALIGN_CENTER, -60, 0);
    lv_obj_center(switch_1);

    lv_obj_add_event_cb(switch_1, event_handler, LV_EVENT_ALL, NULL);

}


