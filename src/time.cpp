#include <time.hpp>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);

void test123(){
    Serial.println("Test123");
}

void setupTime(){
    timeClient.begin();
}



void loopTime(){
    timeClient.update();
}

int getHours(){
    return timeClient.getHours();
}

int getMinutes(){
    return timeClient.getMinutes();
}