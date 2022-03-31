#include "HAL/HAL.h"
#include "page_control.h"
#include "page_news.h"

LV_IMG_DECLARE(null)

int cuurent_quezhen, total_quezhen, yisi, jingwai, zhiyu, siwang;

static void back_delete_cb(lv_event_t* event)
{
    lv_scr_load_anim(scr_home, LV_SCR_LOAD_ANIM_NONE, 50, 0, true);   //退出后删除页面
}

void page_news()
{
    lv_obj_add_event_cb(scr_page, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);

    if(Wifi_status == 2)
    {
        lv_obj_t* news_tab = lv_tabview_create(scr_page, LV_DIR_LEFT, 50);
        lv_obj_add_event_cb(news_tab, back_delete_cb, LV_EVENT_LONG_PRESSED, NULL);
        lv_obj_clear_flag(lv_tabview_get_content(news_tab), LV_OBJ_FLAG_SCROLLABLE);		//禁止滚动
        lv_obj_set_style_text_font(news_tab, &myfont, 0);
        lv_obj_t* tab1 = lv_tabview_add_tab(news_tab, "本地疫情");
        lv_obj_t* tab2 = lv_tabview_add_tab(news_tab, "今日新闻");
        lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_OFF);
        lv_obj_clear_flag(tab1, LV_OBJ_FLAG_SCROLLABLE);

        //疫情数据
        lv_obj_t* text1 = lv_label_create(tab1);    //现有确诊
        lv_obj_t* text2 = lv_label_create(tab1);    //累积确诊
        lv_obj_t* text3 = lv_label_create(tab1);    //无症状
        lv_obj_t* text4 = lv_label_create(tab1);    //境外输入
        lv_obj_t* text5 = lv_label_create(tab1);    //累计治愈
        lv_obj_t* text6 = lv_label_create(tab1);    //累计死亡
        lv_obj_t* text7 = lv_label_create(tab1);    //更新时间
        lv_obj_t* text8 = lv_label_create(tab1);    //TIP

        lv_obj_set_style_text_font(text1, &myfont, 0);
        lv_obj_set_style_text_font(text2, &myfont, 0);
        lv_obj_set_style_text_font(text3, &myfont, 0);
        lv_obj_set_style_text_font(text4, &myfont, 0);
        lv_obj_set_style_text_font(text5, &myfont, 0);
        lv_obj_set_style_text_font(text6, &myfont, 0);
        lv_obj_set_style_text_font(text7, &myfont, 0);
        lv_obj_set_style_text_font(text8, &myfont, 0);
        lv_label_set_recolor(text1, true);          //文本重新着色
        lv_label_set_recolor(text2, true);
        lv_label_set_recolor(text3, true);
        lv_label_set_recolor(text4, true);
        lv_label_set_recolor(text5, true);
        lv_label_set_recolor(text6, true);
        lv_label_set_recolor(text7, true);
        lv_label_set_recolor(text8, true);

        lv_label_set_text_fmt(text1, "现有确诊\n#FFA400 %d#", retdata_curConfirm);
        lv_label_set_text_fmt(text2, "累计确诊\n#9B4400 %d#", retdata_confirm);
        lv_label_set_text_fmt(text3, "无症状\n#A78E44 %d#", retdata_asymptomatic);
        lv_label_set_text_fmt(text4, "新增病例\n#44CEF6 %d#", retdata_nativeRelative);
        lv_label_set_text_fmt(text5, "累计治愈\n#40DE5A %d#", retdata_heal);
        lv_label_set_text_fmt(text6, "累计死亡\n#758A99 %d#", retdata_died);
        lv_label_set_text_fmt(text7, "更新时间: #FF3300 %02d/%02d#", currentMonth, monthDay);
        lv_label_set_text(text8, "#F47983 疫情未结束,防疫别大意#\n#F47983 外出佩戴好口罩!#");

        lv_obj_set_pos(text1, 15, 0);
        lv_obj_set_pos(text2, 145, 0);
        lv_obj_set_pos(text3, 15, 50);
        lv_obj_set_pos(text4, 145, 50);
        lv_obj_set_pos(text5, 15, 100);
        lv_obj_set_pos(text6, 145, 100);
        lv_obj_set_pos(text7, 15, 150);
        lv_obj_set_pos(text8, 15, 175);

        //新闻数据
        lv_obj_t* text9 = lv_label_create(tab2);
        lv_obj_t* text10 = lv_label_create(tab2);
        lv_obj_t* text11 = lv_label_create(tab2);

        static lv_style_t news_style;
        lv_style_init(&news_style);
        lv_style_set_text_color(&news_style, lv_palette_main(LV_PALETTE_BLUE_GREY));
        lv_obj_add_style(text9, &news_style, 0);
        lv_obj_add_style(text10, &news_style, 0);
        lv_obj_add_style(text11, &news_style, 0);
        lv_obj_set_style_text_font(text9, &myfont, 0);
        lv_obj_set_style_text_font(text10, &myfont, 0);
        lv_obj_set_style_text_font(text11, &myfont, 0);

        lv_label_set_text(text9, "1.巴黎圣日尔曼总比分\n2-3不敌皇家马德里,\n止步欧冠16强");
        lv_label_set_text(text10, "2.第18金!中国轮椅冰\n壶队摘得金牌");
        lv_label_set_text(text11, "3.乌方称俄首次向乌\n西部发起进攻,俄证实\n摧毁两机场");

        lv_obj_set_pos(text9, 10, 5);
        lv_obj_set_pos(text10, 10, 85);
        lv_obj_set_pos(text11, 10, 145);

    }
    else
    {
        lv_obj_t* bg_null = lv_img_create(scr_page);
        lv_img_set_src(bg_null, &null);
    }
}