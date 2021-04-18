#include <led_bar.hpp>
#include <NeoPixelBrightnessBus.h>
#include <variables.hpp>

NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_STRIP_SIZE, LED_STRIP_PIN);

void setupLedBar(){
    Serial.println(String(LED_STRIP_BRIGHTNESS) + " " + String(LED_STRIP_SIZE) + " " + String(LED_STRIP_PIN));
    strip.Begin();
    strip.SetBrightness(LED_STRIP_BRIGHTNESS);
    setLedBarStatus(BLACK);
    strip.Show();
}

void setLedBarStatus(RgbColor color){
    
  for (uint8_t i=0; i < LED_STRIP_SIZE; i++){
    strip.SetPixelColor(i, color);
  }
  strip.Show();
}