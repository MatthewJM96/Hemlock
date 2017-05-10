#pragma once

namespace hemlock {
    class FpsTimer {
    public:
        FpsTimer() {}
        ~FpsTimer() {}

        void begin();
        f64 end();

        f64 getFrameTime() const { return m_frameTime; }
        f64 getFPS()       const { return m_fps; }
    protected:
        f64 m_frameTime   = 0.0;
        f64 m_fps         = 0.0;
        ui32 m_startTime  = 0;

        void calculateFps();
    };

    class FpsLimiter : public FpsTimer {
    public:
        FpsLimiter() {}
        ~FpsLimiter() {}

        void init(f64 maxFps) { m_maxFps = maxFps; }

        void setMaxFps(f64 maxFps) { m_maxFps = maxFps; }

        f64 end();
    private:
        f64 m_maxFps;
    };
}
namespace h = hemlock;
