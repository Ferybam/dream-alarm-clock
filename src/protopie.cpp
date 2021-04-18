#include <variables.hpp>
#include <protopie.hpp>
#include <ArduinoWebsockets.h>
#include <time.hpp>
#include <alarm.hpp>
#include <inputs.hpp>

using namespace websockets;

enum TimePart
{
    NONE,
    TIME,
    HOURS,
    MINUTES
};

TimePart modifyingTimePart = NONE;
WebsocketsClient client;
int lastEncoderPosition = 0;
bool enWiFi = false;

/*
if (WiFi.status() == WL_CONNECTED && !enWiFi && client.available()){
    enWiFi == true;
    sendToProtopie("SET-WLAN-ICON-ENABLED");
}*/

void ensureWebSocketConnected()
{
    if (!client.ping())
    {
        Serial.println("Websocket connection lost!");
        while (!client.connect(PROTOPIE_WS_SERVER, PROTOPIE_WS_PORT, PROTOPIE_WS_PATH))
        {
            Serial.println("Trying to connect to protopie websocket at " + String(PROTOPIE_WS_SERVER) + " " + PROTOPIE_WS_PORT + " " + String(PROTOPIE_WS_PATH));
        }
        Serial.println("Websocket connected!");
    }
}

void sendToProtopie(String message)
{
    //Serial.println(message);
    ensureWebSocketConnected();
    client.send(message);
}

void sendCurrentTimeToProtopie()
{
    int hours = getHours();
    bool am = true;
    if(hours >= 12){
        hours = hours -12;
        am = false;
    }
    if(hours == 0){
        hours = 12;
    }



    int hour1 = (hours - (hours % 10)) / 10;
    int hour2 = hours % 10;

    sendToProtopie("SET-HOUR-1-" + String(hour1));
    sendToProtopie("SET-HOUR-2-" + String(hour2));

    int minutes = getMinutes();
    int minute1 = (minutes - (minutes % 10)) / 10;
    int minute2 = minutes % 10;

    sendToProtopie("SET-MINUTE-1-" + String(minute1));
    sendToProtopie("SET-MINUTE-2-" + String(minute2));

    if (am)
    {
        sendToProtopie("SET-AM");
    }
    else
    {
        sendToProtopie("SET-PM");
    }

    if(am == (getAlarmTime() == 0) && hours == getAlarmHours() && minutes >= getAlarmMinutes() && minutes <= getAlarmMinutes() + 5 ){
        setAlarmState(ALARM);
    }else{
        setAlarmState(STANDBY);
    }
}

void encoderUpdate(float progress)
{
    float result = progress - lastEncoderPosition;

    lastEncoderPosition = progress;
    switch (modifyingTimePart)
    {
    case HOURS:
        setAlarmHours(((int)(getAlarmHours() + result)));
        if (getAlarmHours() < 1)
        {
            setAlarmHours(getAlarmHours() + 12);
        }
        if (getAlarmHours() > 12)
        {
           setAlarmHours( getAlarmHours() - 12);
        }
        break;
    case MINUTES:
        setAlarmMinutes( ((int)(getAlarmMinutes() + result)) % 60);
        if (getAlarmMinutes() < 0)
            setAlarmMinutes(getAlarmMinutes() + 60);
        break;
    case TIME:
        setAlarmTime((int)((getAlarmTime() + result)) % 2);
        if (getAlarmTime() < 0)
            setAlarmTime(getAlarmTime() + 2);
        break;
    case NONE:
        return;
    }

    int hour1 = (getAlarmHours() - (getAlarmHours() % 10)) / 10;
    int hour2 = getAlarmHours() % 10;

    int minute1 = (getAlarmMinutes() - (getAlarmMinutes() % 10)) / 10;
    int minute2 = getAlarmMinutes() % 10;

    sendToProtopie("SET-HOUR-1-" + String(hour1));
    sendToProtopie("SET-HOUR-2-" + String(hour2));

    sendToProtopie("SET-MINUTE-1-" + String(minute1));
    sendToProtopie("SET-MINUTE-2-" + String(minute2));

    if (getAlarmTime() == 0)
    {
        sendToProtopie("SET-AM");
    }
    else
    {
        sendToProtopie("SET-PM");
    }
}

void sendTimeBlendToProtopie()
{
    switch (modifyingTimePart)
    {
    case HOURS:
        sendToProtopie("SET-MINUTE-1-DISABLED");
        sendToProtopie("SET-MINUTE-2-DISABLED");
        sendToProtopie("SET-HOUR-1-ENABLED");
        sendToProtopie("SET-HOUR-2-ENABLED");
        sendToProtopie("SET-TIME-DISABLED");
        break;
    case MINUTES:
        sendToProtopie("SET-MINUTE-1-ENABLED");
        sendToProtopie("SET-MINUTE-2-ENABLED");
        sendToProtopie("SET-HOUR-1-DISABLED");
        sendToProtopie("SET-HOUR-2-DISABLED");
        sendToProtopie("SET-TIME-DISABLED");
        break;

    case TIME:
        sendToProtopie("SET-MINUTE-1-DISABLED");
        sendToProtopie("SET-MINUTE-2-DISABLED");
        sendToProtopie("SET-HOUR-1-DISABLED");
        sendToProtopie("SET-HOUR-2-DISABLED");
        sendToProtopie("SET-TIME-ENABLED");
        break;
    case NONE:
        sendToProtopie("SET-MINUTE-1-ENABLED");
        sendToProtopie("SET-MINUTE-2-ENABLED");
        sendToProtopie("SET-HOUR-1-ENABLED");
        sendToProtopie("SET-HOUR-2-ENABLED");
        sendToProtopie("SET-TIME-ENABLED");
        break;
    }
}

void encoderPushed(bool pushed)
{
    int hour1 = (getAlarmHours() - (getAlarmHours() % 10)) / 10;
    int hour2 = getAlarmHours() % 10;

    int minute1 = (getAlarmMinutes() - (getAlarmMinutes() % 10)) / 10;
    int minute2 = getAlarmMinutes() % 10;
    if (pushed)
    {
        switch (modifyingTimePart)
        {
        case NONE:
            modifyingTimePart = HOURS;

            sendToProtopie("SET-HOUR-1-" + String(hour1));
            sendToProtopie("SET-HOUR-2-" + String(hour2));

            sendToProtopie("SET-MINUTE-1-" + String(minute1));
            sendToProtopie("SET-MINUTE-2-" + String(minute2));

            if (getAlarmTime() == 0)
            {
                sendToProtopie("SET-AM");
            }
            else
            {
                sendToProtopie("SET-PM");
            }
            break;
        case HOURS:
            modifyingTimePart = MINUTES;
            break;
        case MINUTES:
            modifyingTimePart = TIME;
            break;
        case TIME:
            modifyingTimePart = NONE;
            break;
        }
        sendTimeBlendToProtopie();
    }
}

void loopProtopie()
{
    if (modifyingTimePart == NONE)
    {
        sendCurrentTimeToProtopie();

    }
}

void setupProtopie()
{
    ensureWebSocketConnected();
    registerEncoderUpdateCallback(encoderUpdate);
    registerEncoderPushedCallback(encoderPushed);
}
