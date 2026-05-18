#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "patterns.hpp"
#include "ButtonHandler.hpp"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET
);

// Pins
const int motors[] = {D7, D8, D9, D10};   // GPIO8, GPIO9, GPIO10, GPIO11
const int buttons[] = {D2, D3};             // GPIO0, GPIO1, GPIO2, GPIO3

// State machine variables
enum State {
    STARTUP,
    MENU,
    RUN
};

State current_state = STARTUP;

// Session variables
float total_time = 5.0; // Total session time in seconds
float max_time = 60.0 * 60.0; // Maximum session time in seconds (1 hour)
float pattern_start_time = 0.0; // Time when current pattern started in seconds, for starting patterns at time 0
float session_start_time = 0.0; // Time when session started in seconds, for tracking total session time
int current_pattern_index = 0; // Index of the current pattern in the PATTERNS array
bool pattern_selected = false; // Flag to indicate if a pattern has been selected
motor_value current_motor_values = {0, 0, 0, 0, false}; // Current motor values to be updated by patterns
float intensity = 0.0; // Intensity of the patterns, can be adjusted based on user input or other factors

void setup() {
    Serial.begin(115200);
    Serial.println("SETUP STARTED");
    
    // // --- Initialize I2C for XIAO ESP32C3 ---
    // Wire.begin(6, 7);   // SDA=D4(GPIO6), SCL=D5(GPIO7)
    
    // // --- Initialize OLED ---
    // if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    //     Serial.println("SSD1306 allocation failed");
    //     while(true);
    //     }
    
    // // Setup motors
    // pinMode(motors[0], OUTPUT);
    // pinMode(motors[1], OUTPUT);
    // pinMode(motors[2], OUTPUT);
    // pinMode(motors[3], OUTPUT);

    // // Setup buttons
    // pinMode(buttons[0], INPUT_PULLUP);
    // pinMode(buttons[1], INPUT_PULLUP);

    // initButton(buttons[0]);
    // initButton(buttons[1]);

    // // Setup display
    // display.clearDisplay();
    
    // display.setTextSize(1);
    // display.setTextColor(SSD1306_WHITE);
    // display.setCursor(0, 0);
    
    // display.println("Starting...");
    
    // display.display();   // push buffer to screen

    delay(3000);   // VERY important for native USB

    Serial.println("SETUP COMPLETED");
}

void loop() {

    // // Frame setup
    // display.clearDisplay();

    // Serial.print("Running: ");
    // Serial.print((millis() / 1000.0));

    
    // // --- Read buttons ---
    // ButtonEvent button_0_event = readButton(buttons[0]);
    // ButtonEvent button_1_event = readButton(buttons[1]);


    // switch (current_state)
    // {
    //     case STARTUP:
    //     current_state = MENU;
    //     break;
        
    //     case MENU:
    //     display.setCursor(0, 0);
    //     display.print("Set time: ");
    //     display.print(total_time / 60.0, 1);
    //     display.print(" min");
        
    //     if (button_1_event == ButtonEvent::ShortPress) {
    //         total_time += 5.0 * 60.0;
    //         if (total_time > max_time)
    //         {
    //             total_time = max_time;
    //         }
    //     }

    //     if (button_0_event == ButtonEvent::ShortPress) {
    //         total_time -= 5.0 * 60.0;
    //         if (total_time < 5.0 * 60.0)
    //         {
    //             total_time = 5.0 * 60.0;
    //         }
    //     }

    //     if (button_0_event == ButtonEvent::LongPress) {
    //         current_state = RUN;
    //         pattern_start_time = 0.0; // Start patterns at time 0
    //         intensity = 0.0; // Set intensity to maximum for now, can be adjusted based on user input or other factors
    //         session_start_time = millis() / 1000.0; // Start session timer
    //     }
    //     break;
        
    //     case RUN:
    //     display.setCursor(0, 0);
    //     display.print("Time: ");
    //     display.print((millis() / 1000.0) - session_start_time, 1);
    //     display.print(" seconds");

    //     // Exit to menu if long press on button 1
    //     if (button_1_event == ButtonEvent::LongPress) {
    //         current_state = MENU;
    //     }

    //     // Update intensity
    //     intensity = (millis() / 1000.0 - session_start_time) / total_time; // Time since pattern started
    //     if (intensity > 1.0) {
    //         intensity = 1.0; // Cap intensity at 1.0
    //     }

    //     // Update motors
    //     if (pattern_selected) {
    //         current_motor_values = PATTERNS[current_pattern_index](intensity, (millis() / 1000.0) - pattern_start_time);
    //         if (current_motor_values.complete) {
    //             pattern_selected = false; // Mark pattern as completed
    //         }
    //         else {
    //             // Update motor outputs based on current_motor_values
    //             // analogWrite(motors[0], current_motor_values.motor_1);
    //             analogWrite(motors[0], 0);
    //             // analogWrite(motors[1], current_motor_values.motor_2);
    //             analogWrite(motors[1], 0);
    //             analogWrite(motors[2], current_motor_values.motor_3);
    //             analogWrite(motors[3], current_motor_values.motor_4);
    //         }
    //     }
    //     else {
    //         // Select a new pattern if none is currently selected
    //         current_pattern_index = random(PATTERN_COUNT); // Cycle through patterns
    //         pattern_start_time = millis() / 1000.0; // Reset pattern start time
    //         pattern_selected = true; // Mark pattern as selected
    //     }
    //     break;
        
    //     default:
    //     current_state = STARTUP;
    //     break;
    // }
    
    // display.display();
}

