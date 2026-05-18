#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

enum class ButtonEvent {
    None,
    ShortPress,
    LongPress
};

/**
 * Initializes a button pin.
 * Enables internal pullup automatically (active-low button).
 */
void initButton(int pin);

/**
 * Poll button state.
 *
 * Must be called repeatedly (e.g. every loop iteration).
 *
 * Returns:
 *   ButtonEvent::None
 *   ButtonEvent::ShortPress
 *   ButtonEvent::LongPress
 */
ButtonEvent readButton(int pin);

#endif