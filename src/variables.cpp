#include <variables.hpp>

const int ALARM_BEEP_INTERVAL = 1*1000;
const int LED_STRIP_PIN = 13;
const int LED_STRIP_SIZE = 24;
const int LED_STRIP_BRIGHTNESS = 150;
const int LED_STRIP_FADE_TIME_MS = 150000;

const int BUTTON_SNOOZ_PIN_INPUT = 12;
const int BUTTON_SNOOZ_PIN_OUTPUT = 14;
const int BUTTON_SNOOZ_INTERVAL = 100;

const char* PROTOPIE_WS_SERVER = "192.168.178.21";
const int PROTOPIE_WS_PORT = 9981;
const char* PROTOPIE_WS_PATH = "/";

const int ENCODER_A_PIN = 25;
const int ENCODER_B_PIN = 26;
const int ENCODER_BTN_PIN = 27;
const int ENCODER_GND_PIN = 5;

const char *WIFI_SSID = "FRITZ!Box 7530 OG";
//const char *WIFI_SSID = "Jan - Mate 20 5Ghz";
const char *WIFI_PASS = "66954548456431564790";
//const char *WIFI_PASS = "1234567890";

const RgbColor RED(255, 0, 0);
const RgbColor YELLOW(255, 255, 0);
const RgbColor GREEN(0, 255, 0);
const RgbColor CYAN(0, 255, 255);
const RgbColor BLUE(0, 0, 255);
const RgbColor MAGENTA(255, 0, 255);
const RgbColor WHITE(255, 255, 255);
const RgbColor BLACK(0, 0, 0);
const RgbColor WHITE_WARM(255, 242, 194);
