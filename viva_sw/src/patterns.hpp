#pragma once

#include <stdint.h>
#include <stddef.h>

// ======================================================
// Data Types
// ======================================================

struct motor_value {
    unsigned int motor_1;
    unsigned int motor_2;
    unsigned int motor_3;
    unsigned int motor_4;
    bool complete;
};

// Function pointer type for patterns
using Pattern = motor_value (*)(float intensity, float time);

// ======================================================
// Pattern Declarations
// ======================================================

motor_value pattern_rotate(float intensity, float t);
motor_value pattern_breathe(float intensity, float t);
motor_value pattern_scanner(float intensity, float t);
motor_value pattern_flicker(float intensity, float t);
motor_value pattern_beats(float intensity, float t);
motor_value pattern_triangle(float intensity, float t);
motor_value pattern_chaos(float intensity, float t);
motor_value pattern_burst(float intensity, float t);
motor_value pattern_binary(float intensity, float t);
motor_value pattern_focus(float intensity, float t);

// ======================================================
// Pattern Library
// ======================================================

extern const Pattern PATTERNS[];
extern const size_t PATTERN_COUNT;