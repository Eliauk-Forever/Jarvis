#include "HAL/HAL.h"
#include "page_control.h"
#include "page_news.h"

LV_IMG_DECLARE(null)

static void back_delete_cb(lv_event_t* event)
{
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);   //退出后删除页面
}

void page_news()
{
    lv_obj_add_event_cb(scr_page, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);

    if(Wifi_status == 2 || Wifi_status == 3)
    {
        lv_obj_t* news_tab = lv_tabview_create(scr_page, LV_DIR_LEFT, 50);
        lv_obj_add_event_cb(news_tab, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
        lv_obj_clear_flag(lv_tabview_get_content(news_tab), LV_OBJ_FLAG_SCROLLABLE);		//禁止滚动
        lv_obj_set_style_text_font(news_tab, &myfont, 0);
        lv_obj_t* tab1 = lv_tabview_add_tab(news_tab, "深圳疫情");
        lv_obj_t* tab2 = lv_tabview_add_tab(news_tab, "东莞疫情");
        lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_OFF);
        lv_obj_clear_flag(tab1, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_scrollbar_mode(tab2, LV_SCROLLBAR_MODE_OFF);
        lv_obj_clear_flag(tab2, LV_OBJ_FLAG_SCROLLABLE);

        //深圳疫情数据
        lv_obj_t* text1 = lv_label_create(tab1);    //现有确诊
        lv_obj_t* text2 = lv_label_create(tab1);    //累积确诊
        lv_obj_t* text3 = lv_label_create(tab1);    //无症状
        lv_obj_t* text4 = lv_label_create(tab1);    //境外输入
        lv_obj_t* text5 = lv_label_create(tab1);    //累计治愈
        lv_obj_t* text6 = lv_label_create(tab1);    //累计死亡
        lv_obj_t* text7 = lv_label_create(tab1);    //更新时间
        lv_obj_t* text8 = lv_label_create(tab1);    //TIP

        //设置文本格式
        Text_Format(text1, 10, 0, true, true);
        Text_Format(text2, 150, 0, true, true);
        Text_Format(text3, 10, 50, true, true);
        Text_Format(text4, 150, 50, true, true);
        Text_Format(text5, 10, 100, true, true);
        Text_Format(text6, 150, 100, true, true);
        Text_Format(text7, 10, 150, true, true);
        Text_Format(text8, 10, 175, true, true);

        lv_label_set_text_fmt(text1, "现有确诊\n#FFA400 %d#", Local_curConfirm);
        lv_label_set_text_fmt(text2, "累计确诊\n#9B4400 %d#", Local_confirm);
        lv_label_set_text_fmt(text3, "无症状\n#A78E44 %d#", Local_asymptomatic);
        lv_label_set_text_fmt(text4, "新增病例\n#44CEF6 %d#", Local_nativeRelative);
        lv_label_set_text_fmt(text5, "累计治愈\n#40DE5A %d#", Local_heal);
        lv_label_set_text_fmt(text6, "累计死亡\n#758A99 %d#", Local_died);
        lv_label_set_text_fmt(text7, "更新时间: #FF3300 %02d/%02d#", currentMonth, monthDay);
        lv_label_set_text(text8, "#F47983 疫情未结束,防疫别大意#\n#F47983 外出佩戴好口罩!#");

        //东莞疫情数据
        lv_obj_t* text9 = lv_label_create(tab2);    //现有确诊
        lv_obj_t* text10 = lv_label_create(tab2);    //累积确诊
        lv_obj_t* text11 = lv_label_create(tab2);    //无症状
        lv_obj_t* text12 = lv_label_create(tab2);    //境外输入
        lv_obj_t* text13 = lv_label_create(tab2);    //累计治愈
        lv_obj_t* text14 = lv_label_create(tab2);    //累计死亡
        lv_obj_t* text15 = lv_label_create(tab2);    //更新时间
        lv_obj_t* text16 = lv_label_create(tab2);    //TIP

        //设置文本格式
        Text_Format(text9, 10, 0, true, true);
        Text_Format(text10, 150, 0, true, true);
        Text_Format(text11, 10, 50, true, true);
        Text_Format(text12, 150, 50, true, true);
        Text_Format(text13, 10, 100, true, true);
        Text_Format(text14, 150, 100, true, true);
        Text_Format(text15, 10, 150, true, true);
        Text_Format(text16, 10, 175, true, true);

        lv_label_set_text_fmt(text9, "现有确诊\n#FFA400 %d#", National_curConfirm);
        lv_label_set_text_fmt(text10, "累计确诊\n#9B4400 %d#", National_confirm);
        lv_label_set_text_fmt(text11, "无症状\n#A78E44 %d#", National_asymptomatic);
        lv_label_set_text_fmt(text12, "新增病例\n#44CEF6 %d#", National_nativeRelative);
        lv_label_set_text_fmt(text13, "累计治愈\n#40DE5A %d#", National_heal);
        lv_label_set_text_fmt(text14, "累计死亡\n#758A99 %d#", National_died);
        lv_label_set_text_fmt(text15, "更新时间: #FF3300 %02d/%02d#", currentMonth, monthDay);
        lv_label_set_text(text16, "#F47983 遵守防疫政策#\n#F47983 出行请持有48h内核酸!#");
    }
    else
    {
        lv_obj_t* bg_null = lv_img_create(scr_page);
        lv_img_set_src(bg_null, &null);
    }
}