#ifndef __HAL_H
#define __HAL_H

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <ESP32Time.h>

#include <WebServer.h>
#include <ESPmDNS.h>
#include <esp_wifi.h>
#include <HTTPClient.h>
#include "Version.h"
#include "CommonMacro.h"

extern uint8_t Wifi_status;

namespace HAL
{
	void Init();
	void Update();
    void Wifi_Connect();

/* SD */
    bool SD_Init();
    float SD_GetCardSizeMB();

/* WIFI */
    bool AutoConfig();
    void Wifi_Config();
    void HandleClient();

/* Sensor */
    bool AHT_begin();
    bool AHT_reset();
    bool AHT_measure(float *temp, float *humi);
    float AHT_getTemperature();
    float AHT_getHumidity();
}

#endif