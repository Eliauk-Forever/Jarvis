#ifndef PAGE_SETTING_H
#define PAGE_SETTING_H

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t currentHour, currentMinute, currentSecond, weekDay, monthDay, currentMonth, currentYear;

void page_setting();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  
