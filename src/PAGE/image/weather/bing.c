#include "../lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif


const LV_ATTRIBUTE_MEM_ALIGN uint8_t bing_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
//Pixel format: Alpha 8 bit, Red: 3 bit, Green: 3 bit, Blue: 2 bit
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
//Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
//Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit  BUT the 2  color bytes are swapped
#endif
#if LV_COLOR_DEPTH == 32
#endif
};

const lv_img_dsc_t bing = {
    .header.always_zero = 0,
    .header.w = 0,
    .header.h = 0,
    .data_size = 0 * LV_IMG_PX_SIZE_ALPHA_BYTE,
    .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
    .data = bing_map,
};

//end of file