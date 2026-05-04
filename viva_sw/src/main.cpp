#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET
);

const int motors[] = {D7, D8, D9, D10};   // GPIO8, GPIO9, GPIO10, GPIO11
const int buttons[] = {D2, D3};             // GPIO0, GPIO1, GPIO2, GPIO3
int current_motor = 0;

void update_motor(int motor_index){
    for (int i = 0; i < 4; i++) {
        if (i == motor_index) {
            digitalWrite(motors[i], HIGH);
        } else {
            digitalWrite(motors[i], LOW);
        }
    }
}

void setup() {
    Serial.begin(115200);

    // --- Initialize I2C for XIAO ESP32C3 ---
    Wire.begin(6, 7);   // SDA=D4(GPIO6), SCL=D5(GPIO7)

    // --- Initialize OLED ---
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        while(true);
    }

    pinMode(motors[0], OUTPUT);
    pinMode(motors[1], OUTPUT);
    pinMode(motors[2], OUTPUT);
    pinMode(motors[3], OUTPUT);

    for (int i = 0; i < 4; i++) {
        digitalWrite(motors[i], LOW);  // Ensure motors are off at startup
    }

    pinMode(buttons[0], INPUT_PULLUP);
    pinMode(buttons[1], INPUT_PULLUP);

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.println("Hello, world!");

    display.display();   // push buffer to screen
}

void loop() {
    if (digitalRead(buttons[0]) == LOW) {
        current_motor = (current_motor + 1) % 4;  // Cycle through motors
        update_motor(current_motor);
        delay(200);  // Debounce delay
    }

    if (digitalRead(buttons[1]) == LOW) {
        current_motor = (current_motor - 1 + 4) % 4;  // Cycle backwards through motors
        update_motor(current_motor);
        delay(200);  // Debounce delay
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Current Motor: ");
    display.println(current_motor + 1);  // Display motor number (1-4)
    display.display();
}

