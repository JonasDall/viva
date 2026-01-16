#include <Arduino.h>

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

pos actutor = {0.0, 0.0, 0.0};
float size = 1.0;


void setup() {
  for (int i=0; i<4; ++i){
    ledcSetup(i, frequency, resolution);
    ledcAttachPin(pin[i], i);
  }
}

void loop() {
  // ledcWrite(channel, (range * sin(2 * PI * 5 * millis() / 1000)) / 2 + range/2);
  for (int i=0; i<4; ++i){
    if (distance(actutor, motors[i]) < size){
      ledcWrite(i, range * linear(distance(actutor, motors[i]), size));
    }
  }
}