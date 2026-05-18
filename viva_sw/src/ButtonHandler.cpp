#include "ButtonHandler.hpp"
#include <map>

struct ButtonState {
    bool lastPressed = false;
    bool longReported = false;
    unsigned long pressStart = 0;
};

static std::map<int, ButtonState> buttonStates;

static constexpr unsigned long LONG_PRESS_TIME = 1000; // ms

//--------------------------------------------------

void initButton(int pin)
{
    pinMode(pin, INPUT_PULLUP); // active low
    buttonStates[pin] = ButtonState{};
}

//--------------------------------------------------

ButtonEvent readButton(int pin)
{
    auto it = buttonStates.find(pin);
    if (it == buttonStates.end())
        return ButtonEvent::None;

    ButtonState &state = it->second;

    bool pressed = (digitalRead(pin) == LOW);
    unsigned long now = millis();

    ButtonEvent event = ButtonEvent::None;

    // --- Press edge ---
    if (pressed && !state.lastPressed) {
        state.pressStart = now;
        state.longReported = false;
    }

    // --- Held ---
    if (pressed && !state.longReported) {
        if (now - state.pressStart >= LONG_PRESS_TIME) {
            state.longReported = true;
            event = ButtonEvent::LongPress;
        }
    }

    // --- Release edge ---
    if (!pressed && state.lastPressed) {
        if (!state.longReported) {
            event = ButtonEvent::ShortPress;
        }
    }

    state.lastPressed = pressed;
    return event;
}