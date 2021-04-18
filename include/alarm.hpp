#pragma once

void setupAlarm();

void loopAlarm();

int getAlarmHours();

int getAlarmMinutes();

int getAlarmTime();

void setAlarmHours(int hours);

void setAlarmTime(int time);

void setAlarmMinutes(int minutes);

enum AlarmState
{
    STANDBY,
    ARMED,
    SNOOZE,
    ALARM
};

void setAlarmState(AlarmState alarmState);

AlarmState getAlarmState();
