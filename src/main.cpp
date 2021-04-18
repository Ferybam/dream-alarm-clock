#include <Arduino.h>
#include <led_bar.hpp>
#include <wifi_connection.hpp>
#include <ArduinoWebsockets.h>
#include <variables.hpp>
#include <inputs.hpp>
#include <protopie.hpp>
#include <alarm.hpp>
#include <time.hpp>
#include <task.hpp>

void setup()
{
  Serial.begin(115200);
  setupInputs();
  setupLedBar();
  setupWifi();
  setupAlarm();
  setupTime();
  setupProtopie();
  setLedBarStatus(BLACK);
  //sendToProtopie("DISPLAY-ON");
}


void loop()
{
  loopTime();
  ensureWifiConnection();
  loopProtopie();
  loopInputs();
  loopAlarm();

  
}