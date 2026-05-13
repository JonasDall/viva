#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "patterns.hpp"

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

const int battery = A0;  // GPIO4

float button_hold_threshold = 1.0; // seconds

enum State {
    STARTUP,
    MENU,
    RUN
};

State current_state = STARTUP;
float total_time = 10.0;
float current_time = 0.0;
float pattern_start_time = 0.0;

float button_0_hold_time = 0.0;
bool button_0_holding = false;
float button_1_hold_time = 0.0;
bool button_1_holding = false;

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

    pinMode(battery, INPUT);

    for (int i = 0; i < 4; i++) {
        digitalWrite(motors[i], LOW);  // Ensure motors are off at startup
    }

    pinMode(buttons[0], INPUT_PULLUP);
    pinMode(buttons[1], INPUT_PULLUP);

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.println("Starting...");

    display.display();   // push buffer to screen
}

void loop() {
    // --- Calculate deltatime ---
    float deltatime = millis() / 1000.0 - current_time;
    current_time += deltatime;

    // --- Read battery voltage ---
    uint32_t Vbatt = 0;
    for(int i = 0; i < 16; i++) {
      Vbatt = Vbatt + analogReadMilliVolts(A0); // ADC with correction   
    }

    float Vbattf = 2 * Vbatt / 16 / 1000.0;     // attenuation ratio 1/2, mV --> V

    // --- Read buttons ---
    //bool button_0_pressed

    if (buttons[0] == LOW) {
        if (!button_0_holding) {
            button_0_holding = true;
            button_0_hold_time = 0.0;
        } else {
            button_0_hold_time += deltatime;
        }
    } else {
        button_0_holding = false;
        button_0_hold_time = 0.0;
    }

    if (buttons[1] == LOW) {
        if (!button_1_holding) {
            button_1_holding = true;
            button_1_hold_time = 0.0;
        } else {
            button_1_hold_time += deltatime;
        }
    } else {
        button_1_holding = false;
        button_1_hold_time = 0.0;
    }

    if (button_0_hold_time > button_hold_threshold) {
        button_0_hold_time = 0.0;
        Serial.println("Button 0 held");
    }

    // --- Display battery voltage ---
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Battery: ");
    display.print(Vbattf, 2);  // Display voltage with 2 decimal places
    display.println(" V");

    switch (current_state)
    {
    case STARTUP:
        current_state = MENU;
        break;

    case MENU:
        display.setCursor(0, 20);
        display.print("Set time: ");
        display.print(total_time, 1);
        display.print(" min");

        if (button_0_pressed) {
            total_time += 10.0;
        }
        if (button_1_pressed) {
            total_time -= 10.0;
            if (total_time < 10.0)
            {
                total_time = 10.0;
            }
        }
        break;

    case RUN:
        /* code */
        break;

    default:
        current_state = STARTUP;
        break;
    }

    display.display();
}

