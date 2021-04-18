#include <alarm.hpp>
#include <WebServer.h>
#include <wifi_connection.hpp>
#include <time.hpp>
#include <DFRobotDFPlayerMini.h>
#include <task.hpp>
#include <variables.hpp>


WebServer server(80);
AlarmState alarmState = STANDBY;
DFRobotDFPlayerMini Player;
Schedule alarmBeepInterval(ALARM_BEEP_INTERVAL, false);

int alarmHours = 0;
int alarmMinutes = 0;
int alarmTime = 0;

void handleStatus()
{
    server.send(200, "text/html", String(alarmState) + " " + getHours() + ":" + getMinutes());
}

void handleReset()
{
    alarmState = ALARM;
    handleStatus();
}

void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setupAlarmHttpServer()
{
    ensureWifiConnection();
    server.on("/status", handleStatus);
    server.on("/reset", handleReset);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

int getAlarmHours()
{
    return alarmHours;
}

int getAlarmMinutes()
{
    return alarmMinutes;
}

int getAlarmTime()
{
    return alarmTime;
}

void setAlarmHours(int hours)
{
    alarmHours = hours;
}

void setAlarmTime(int time)
{
    alarmTime = time;
}

void setAlarmMinutes(int minutes)
{
    alarmMinutes = minutes;
}

void setupAudio()
{
    Serial2.begin(9800);
    Player.begin(Serial2, false, false);
    Player.setTimeOut(500);
    Player.disableLoopAll();
    Player.EQ(DFPLAYER_EQ_NORMAL);
    Player.outputDevice(DFPLAYER_DEVICE_SD);
    Player.volume(5);
    Player.play(1);
    delay(300);
    Player.volume(0);
}

void setAlarmState(AlarmState state){
    alarmState = state;
}

AlarmState getAlarmState(){
    return alarmState;
}

void setupAlarm()
{
    setupAlarmHttpServer();
    setupAudio();
}

void loopAlarm()
{
    server.handleClient();

    if(alarmState == ALARM){
        if(!alarmBeepInterval.is_enabled()){
            alarmBeepInterval.set_enabled(true);
        }
    }else{
        if(alarmBeepInterval.is_enabled()){
            alarmBeepInterval.set_enabled(false);
        }
    }

    if (alarmBeepInterval.execute())
    {
        Player.volume(10);
        Player.play(1);
        Player.volume(10);
    }
}