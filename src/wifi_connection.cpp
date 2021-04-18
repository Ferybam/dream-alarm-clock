#include <variables.hpp>
#include <wifi_connection.hpp>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

WiFiMulti wifiMulti;

void setupWifi()
{
    wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
    
    ensureWifiConnection();
}


void ensureWifiConnection()
{
    if (wifiMulti.run() == WL_CONNECTED)
    {   
        return;
    }
    while (wifiMulti.run() != WL_CONNECTED)
    {
        Serial.println("WiFi connecting...");
        yield();
        delay(1000);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
}