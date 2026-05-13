#include "patterns.hpp"
#include <math.h>

// ======================================================
// Helpers (internal linkage)
// ======================================================

static inline uint8_t clamp_pwm(float v)
{
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (uint8_t)v;
}

static float hash(float x)
{
    return sinf(x * 12.9898f) * 43758.5453f;
}

static float triangle(float x)
{
    return fabs(fmodf(x, 2.0f) - 1.0f);
}

// ======================================================
// Patterns
// ======================================================

motor_value pattern_rotate(float intensity, float t)
{
    float w = 0.5f + 6.0f * intensity;
    float A = 120 + 135 * intensity;

    motor_value v;
    v.motor_1 = clamp_pwm((sinf(w*t)+1)*0.5f*A);
    v.motor_2 = clamp_pwm((sinf(w*t+1.57f)+1)*0.5f*A);
    v.motor_3 = clamp_pwm((sinf(w*t+3.14f)+1)*0.5f*A);
    v.motor_4 = clamp_pwm((sinf(w*t+4.71f)+1)*0.5f*A);

    v.complete = t > 8.0f;
    return v;
}

motor_value pattern_breathe(float intensity, float t)
{
    float w = 0.3f + 2.5f * intensity;
    float x = (sinf(w*t)+1)*127;

    return {
        clamp_pwm(x),
        clamp_pwm(x),
        clamp_pwm(x),
        clamp_pwm(x),
        t > 10.0f
    };
}

motor_value pattern_scanner(float intensity, float t)
{
    float speed = 0.5f + 4.0f*intensity;
    float pos = (sinf(speed*t)+1)*1.5f;

    motor_value v;
    v.motor_1 = clamp_pwm(255*(1-fabs(pos-0)));
    v.motor_2 = clamp_pwm(255*(1-fabs(pos-1)));
    v.motor_3 = clamp_pwm(255*(1-fabs(pos-2)));
    v.motor_4 = clamp_pwm(255*(1-fabs(pos-3)));

    v.complete = t > 7.0f;
    return v;
}

motor_value pattern_flicker(float intensity, float t)
{
    float rate = 5 + 40*intensity;

    motor_value v;
    v.motor_1 = clamp_pwm(fabs(hash(t*rate))*255);
    v.motor_2 = clamp_pwm(fabs(hash(t*rate+1))*255);
    v.motor_3 = clamp_pwm(fabs(hash(t*rate+2))*255);
    v.motor_4 = clamp_pwm(fabs(hash(t*rate+3))*255);

    v.complete = t > 6.0f;
    return v;
}

motor_value pattern_beats(float intensity, float t)
{
    float bpm = 1.0f + 6.0f*intensity;
    float beat = fmodf(t*bpm,1.0f);

    float pulse = expf(-8*beat);

    motor_value v;
    v.motor_1 = clamp_pwm(255*pulse);
    v.motor_2 = clamp_pwm(200*pulse);
    v.motor_3 = clamp_pwm(255*pulse);
    v.motor_4 = clamp_pwm(150*pulse);

    v.complete = t > 9.0f;
    return v;
}

motor_value pattern_triangle(float intensity, float t)
{
    float w = 1 + 6*intensity;

    motor_value v;
    v.motor_1 = clamp_pwm(triangle(w*t)*255);
    v.motor_2 = clamp_pwm(triangle(w*t+0.5f)*255);
    v.motor_3 = clamp_pwm(triangle(w*t+1.0f)*255);
    v.motor_4 = clamp_pwm(triangle(w*t+1.5f)*255);

    v.complete = t > 8.0f;
    return v;
}

motor_value pattern_chaos(float intensity, float t)
{
    float s = 2 + 8*intensity;

    motor_value v;
    v.motor_1 = clamp_pwm((sinf(t*s)+sinf(t*1.3f*s))*64+128);
    v.motor_2 = clamp_pwm((sinf(t*1.7f*s))*127+128);
    v.motor_3 = clamp_pwm((sinf(t*2.1f*s))*127+128);
    v.motor_4 = clamp_pwm((sinf(t*2.9f*s))*127+128);

    v.complete = t > 9.0f;
    return v;
}

motor_value pattern_burst(float intensity, float t)
{
    float freq = 1 + 8*intensity;
    float phase = fmodf(t*freq,1.0f);

    float burst = phase < 0.15f ? 255*(1-phase/0.15f) : 0;

    motor_value v;
    v.motor_1 = clamp_pwm(burst);
    v.motor_2 = clamp_pwm(burst*0.6f);
    v.motor_3 = clamp_pwm(burst);
    v.motor_4 = clamp_pwm(burst*0.3f);

    v.complete = t > 7.0f;
    return v;
}

motor_value pattern_binary(float intensity, float t)
{
    int step = (int)(t*(2+10*intensity));

    motor_value v;
    v.motor_1 = (step & 1) ? 255 : 0;
    v.motor_2 = (step & 2) ? 255 : 0;
    v.motor_3 = (step & 4) ? 255 : 0;
    v.motor_4 = (step & 8) ? 255 : 0;

    v.complete = t > 6.0f;
    return v;
}

motor_value pattern_focus(float intensity, float t)
{
    float w = 0.5f + 5*intensity;

    float f1 = (sinf(w*t)+1)*0.5f;
    float f2 = (sinf(w*t+2)+1)*0.5f;

    motor_value v;
    v.motor_1 = clamp_pwm(255*f1);
    v.motor_2 = clamp_pwm(100*f2);
    v.motor_3 = clamp_pwm(60*(1-f1));
    v.motor_4 = clamp_pwm(30*(1-f2));

    v.complete = t > 8.0f;
    return v;
}

// ======================================================
// Pattern Table
// ======================================================

const Pattern PATTERNS[] = {
    pattern_rotate,
    pattern_breathe,
    pattern_scanner,
    pattern_flicker,
    pattern_beats,
    pattern_triangle,
    pattern_chaos,
    pattern_burst,
    pattern_binary,
    pattern_focus
};

const size_t PATTERN_COUNT =
    sizeof(PATTERNS) / sizeof(PATTERNS[0]);