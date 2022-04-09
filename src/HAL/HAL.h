#ifndef __HAL_H
#define __HAL_H

#include <stdint.h>
#include <Arduino.h>
#include <FreeRTOS.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <ESP32Time.h>

#include <WebServer.h>
#include <ESPmDNS.h>
#include <esp_now.h>
#include <HTTPClient.h>
#include "Version.h"
#include "CommonMacro.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ir_Gree.h>

extern uint8_t Wifi_status;
extern uint32_t currentBacklight;

namespace HAL
{
	void Init();
	void Update();
    void Wifi_Connect();
    void Wifi_Close();

/* Backlight */
    void Backlight_Init();
    uint32_t Backlight_GetValue();
    void Backlight_SetValue(int32_t val);
    void Backlight_SetGradual(uint32_t target, uint16_t time = 500);
    void Backlight_ForceLit(bool en);
    
/* SD */
    bool SD_Init();
    float SD_GetCardSizeMB();

/* WIFI */
    bool AutoConfig();
    void Wifi_Config();
    void Wifi_Close();
    void HandleClient();

/* Sensor */
    bool AHT_begin();
    bool AHT_reset();
    bool AHT_measure(float *temp, float *humi);
    float AHT_getTemperature();
    float AHT_getHumidity();

/* Buzzer */
    void Buzz_init();
    void Buzz_SetEnable(bool en);
    bool Get_Buzzer_staus();
    void Buzz_Tone(uint32_t freq, int32_t duration = 0);

/* Audio */
    void Audio_Init();
    void Audio_Update();
    bool Audio_PlayMusic(const char* name);
}

#endif