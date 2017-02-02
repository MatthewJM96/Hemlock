#include "stdafx.h"

#include "Timing.h"

#define MILLISECONDS 1000.0

void h::FpsTimer::begin() {
    m_startTime = SDL_GetTicks();
}

f64 h::FpsTimer::end() {
    calculateFps();

    return m_fps;
}

// TODO(Matthew): At some point compare with a sampling method that looks at previous N frame times.
void h::FpsTimer::calculateFps() {
    static const f64 smoothingFactor = 0.9;

    f64 timeThisFrame = (f64)(SDL_GetTicks() - m_startTime);

    m_frameTime = (m_frameTime * smoothingFactor) + timeThisFrame * (1.0 - smoothingFactor);
    
    if (m_frameTime == 0.0) {
        m_fps = 0.0;
    } else {
        m_fps = (f64)(MILLISECONDS / m_frameTime);
    }
}

f64 h::FpsLimiter::end() {
    calculateFps();

    f64 timeThisFrame = (f64)(SDL_GetTicks() - m_startTime);
    if (MILLISECONDS / m_maxFps > timeThisFrame) {
        SDL_Delay((MILLISECONDS / m_fps) - timeThisFrame);
    }

    return m_fps;
}
