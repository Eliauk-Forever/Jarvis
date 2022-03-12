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
        lv_obj_set_style_text_font(news_tab, &myfont, 0);
        lv_obj_t* tab1 = lv_tabview_add_tab(news_tab, "国内疫情");
        lv_obj_t* tab2 = lv_tabview_add_tab(news_tab, "今日新闻");

        //疫情数据
        lv_obj_t* text1 = lv_label_create(tab1);    //现有确诊
        lv_obj_t* text2 = lv_label_create(tab1);    //累积确诊
        lv_obj_t* text3 = lv_label_create(tab1);    //无症状
        lv_obj_t* text4 = lv_label_create(tab1);    //境外输入
        lv_obj_t* text5 = lv_label_create(tab1);    //累计治愈
        lv_obj_t* text6 = lv_label_create(tab1);    //累计死亡
        lv_obj_t* text7 = lv_label_create(tab1);    //TIP

        lv_obj_set_style_text_font(text1, &myfont, 0);
        lv_obj_set_style_text_font(text2, &myfont, 0);
        lv_obj_set_style_text_font(text3, &myfont, 0);
        lv_obj_set_style_text_font(text4, &myfont, 0);
        lv_obj_set_style_text_font(text5, &myfont, 0);
        lv_obj_set_style_text_font(text6, &myfont, 0);
        lv_obj_set_style_text_font(text7, &myfont, 0);
        lv_label_set_recolor(text1, true);          //文本重新着色
        lv_label_set_recolor(text2, true);
        lv_label_set_recolor(text3, true);
        lv_label_set_recolor(text4, true);
        lv_label_set_recolor(text5, true);
        lv_label_set_recolor(text6, true);
        lv_label_set_recolor(text7, true);

        lv_label_set_text(text1, "现有确诊\n#FFA400 237111#");
        lv_label_set_text(text2, "累积确诊\n#9B4400 389823#");
        lv_label_set_text(text3, "无症状\n#A78E44 5076#");
        lv_label_set_text(text4, "境外输入\n#44CEF6 16106#");
        lv_label_set_text(text5, "累计治愈\n#40DE5A 143494#");
        lv_label_set_text(text6, "累计死亡\n#758A99 9218#");
        lv_label_set_text(text7, "#F47983 疫情未结束,防疫别大意#\n#F47983 外出佩戴好口罩!#");

        lv_obj_set_pos(text1, 15, 0);
        lv_obj_set_pos(text2, 145, 0);
        lv_obj_set_pos(text3, 15, 55);
        lv_obj_set_pos(text4, 145, 55);
        lv_obj_set_pos(text5, 15, 110);
        lv_obj_set_pos(text6, 145, 110);
        lv_obj_set_pos(text7, 15, 170);

        //新闻数据
        lv_obj_t* text8 = lv_label_create(tab2);
        lv_obj_t* text9 = lv_label_create(tab2);
        lv_obj_t* text10 = lv_label_create(tab2);

        static lv_style_t news_style;
        lv_style_init(&news_style);
        lv_style_set_text_color(&news_style, lv_palette_main(LV_PALETTE_BLUE_GREY));
        lv_obj_add_style(text8, &news_style, 0);
        lv_obj_add_style(text9, &news_style, 0);
        lv_obj_add_style(text10, &news_style, 0);
        lv_obj_set_style_text_font(text1, &myfont, 0);
        lv_obj_set_style_text_font(text2, &myfont, 0);
        lv_obj_set_style_text_font(text3, &myfont, 0);

        lv_label_set_text(text8, "1.巴黎圣日尔曼总比分\n2-3不敌皇家马德里,\n止步欧冠16强");
        lv_label_set_text(text9, "2.第18金!中国轮椅冰\n壶队摘得金牌");
        lv_label_set_text(text10, "3.乌方称俄首次向乌\n西部发起进攻,俄证实\n摧毁两机场");

        lv_obj_set_pos(text8, 10, 5);
        lv_obj_set_pos(text9, 10, 85);
        lv_obj_set_pos(text10, 10, 145);

    }
    else
    {
        lv_obj_t* bg_null = lv_img_create(scr_page);
        lv_img_set_src(bg_null, &null);
    }
}