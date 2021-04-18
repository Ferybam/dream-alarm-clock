#include <variables.hpp>
#include <inputs.hpp>
#include <LinkedPointerList.h>

LinkedPointerList<void(float)> encoderUpdateCallbacks;
LinkedPointerList<void(bool)> encoderPushedCallbacks;

int state = 0;
int state_old = 0;
int encoderCounter = 0;
int oldPushedState = -1;
int oldEncoderState = 0;

void IRAM_ATTR encoderUpdate()
{
    //00,01,11,10
    state_old = state;
    state = digitalRead(ENCODER_A_PIN) | digitalRead(ENCODER_B_PIN) << 1;

    if (state_old == 0 && state == 1)
        encoderCounter++;
    if (state_old == 0 && state == 2)
        encoderCounter--;

    if (state_old == 1 && state == 0)
        encoderCounter--;
    if (state_old == 1 && state == 3)
        encoderCounter++;

    if (state_old == 3 && state == 1)
        encoderCounter--;
    if (state_old == 3 && state == 2)
        encoderCounter++;

    if (state_old == 2 && state == 0)
        encoderCounter++;
    if (state_old == 2 && state == 3)
        encoderCounter--;

    //1440 min

    float progress = (float)encoderCounter;

    if (abs(encoderCounter - oldEncoderState) >= 4)
    {

        for (int i = 0; i < encoderUpdateCallbacks.size(); i++)
        {
            encoderUpdateCallbacks.get(i)(encoderCounter / 4);
        }
        oldEncoderState = encoderCounter;
    }
}


void setupButton(int input, int output)
{
    pinMode(input, INPUT_PULLUP);
    pinMode(output, OUTPUT);
}

void setupEncoder()
{
    pinMode(ENCODER_GND_PIN, OUTPUT);
    pinMode(ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(ENCODER_B_PIN, INPUT_PULLUP);
    pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);
    //attachInterrupt(ENCODER_A_PIN, encoderUpdate, CHANGE);
    //attachInterrupt(ENCODER_B_PIN, encoderUpdate, CHANGE);
}

void loopInputs()
{

    int newPushedState = digitalRead(ENCODER_BTN_PIN);
    if (newPushedState != oldPushedState)
    {
        for (int i = 0; i < encoderPushedCallbacks.size(); i++)
        {
            encoderPushedCallbacks.get(i)(newPushedState == LOW);
        }
        oldPushedState = newPushedState;
    }

    encoderUpdate();
}

void setupInputs()
{
    setupButton(BUTTON_SNOOZ_PIN_INPUT, BUTTON_SNOOZ_PIN_OUTPUT);
    setupEncoder();
}

bool isPressed(int input)
{
    return digitalRead(input) == LOW;
}

void registerEncoderUpdateCallback(void (*callback)(float x))
{
    encoderUpdateCallbacks.add(callback);
}

void registerEncoderPushedCallback(void (*callback)(bool pressed))
{
    encoderPushedCallbacks.add(callback);
}
