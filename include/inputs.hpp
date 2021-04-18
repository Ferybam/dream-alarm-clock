#pragma once

void setupInputs();

void loopInputs();

bool isPressed(int input);

void registerEncoderUpdateCallback(void(*callback)(float x));

void registerEncoderPushedCallback(void(*callback)(bool pressed));