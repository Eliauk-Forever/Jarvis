#include "lvgl.h"
#include "TFT_eSPI.h"
#include "HAL/HAL.h"

static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 2 ];

lv_indev_state_t touch_state;
uint16_t touchX, touchY, X, Y;

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

uint16_t calData[5] = { 373, 3182, 394, 3202, 5 };

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.startWrite();
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    bool touched = tft.getTouch( &touchX, &touchY, 600 );
    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        /*Set the coordinates*/
        X = touchX * 1.33;
        Y = touchY * 0.75;
        data->point.x = X;
        data->point.y = Y;
        //Serial.println(X);
        //Serial.println(Y);
    }
}

void screen_init()
{
    tft.begin(); /* TFT init */
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTouch( calData );

    lv_init();

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 2 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register( &indev_drv );
    tft.writecommand(ST7789_DISPON); //Display on
}