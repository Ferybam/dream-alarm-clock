#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;
Adafruit_NeoPixel strip(2, 12, NEO_RGB + NEO_KHZ800);

auto awaitingReset = false;
auto animationRunning = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("--------", "-------");

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255);
}

void loop() {
  auto pressed = digitalRead(4);

  if (WiFiMulti.run() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    strip.setPixelColor(0, strip.Color( 0, 0, 255));
    strip.show();
  } else {
    strip.setPixelColor(0, strip.Color(0, 0, 255));
    strip.show();
    delay(100);
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.show();
    delay(100);
    return;
  }


  WiFiClient client;

  HTTPClient http;

  if (pressed == LOW) {
    if (!awaitingReset) {
      awaitingReset = true;
      animationRunning = !animationRunning;
      Serial.println("Try to reset");
      if (http.begin(client, "http://192.168.178.38/reset")) {  // HTTP
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0) {
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();

      }
    } else {
      awaitingReset = false;
    }
  }

  if (http.begin(client, "http://192.168.178.38/status")) {  // HTTP
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        if (payload != "0") {
          strip.setPixelColor(1, strip.Color(0, 255,  0));
          strip.show();
        } else {
          strip.setPixelColor(1, strip.Color(0, 0, 0));
          strip.show();
        }
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    delay(100);
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}
