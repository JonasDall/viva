#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Definitions
#define TFT_DC D8
#define TFT_CS D7
#define TFT_RST D2
#define TFT_MOSI D9
#define TFT_SCLK D10

Adafruit_ST7735 tft = Adafruit_ST7735 (TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const int frequency = 20000;
const int resolution = 8;
const int range = pow(2, 8);

// Data structures
struct pos{
  float x, y, z;
};

// Functions
float distance (pos pos1, pos pos2){
  return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) + pow(pos2.z - pos1.z, 2));
}

float linear (float distance, float size){
  return 1 - distance/size;
}

// Object definitions
const pos motors[4] = {{1.0, 1.0 ,0.0}, {1.0, -1.0 ,0.0}, {-1.0, 1.0 ,0.0}, {-1.0, -1.0 ,0.0}};
const int pin[4] = {21, 20, 10, 9};

pos actuator = {0.0, 0.0, 0.0};
float size = 1.0;


void setup() {
  for (int i=0; i<4; ++i){
    ledcSetup(i, frequency, resolution);
    ledcAttachPin(pin[i], i);
  }
  pinMode(A0, INPUT); // ADC

  tft.initR(INITR_MINI160x80);
  tft.fillScreen(ST7735_BLACK);
}

void loop() {
  // battery stuff
  uint32_t Vbatt = 0;
  for(int i = 0; i < 16; i++) {
    Vbatt = Vbatt + analogReadMilliVolts(A0); // ADC with correction   
  }
  float Vbattf = 2 * Vbatt / 16 / 1000.0;
  if (Vbattf <= 3.0){
    esp_deep_sleep_start();
  }


  // ledcWrite(channel, (range * sin(2 * PI * 5 * millis() / 1000)) / 2 + range/2);
  for (int i=0; i<4; ++i){
    if (distance(actuator, motors[i]) < size){
      ledcWrite(i, range * linear(distance(actuator, motors[i]), size));
    }
  }
}