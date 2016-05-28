// Based on https://www.researchgate.net/publication/3728672_A_simple_real-time_QRS_detection_algorithm
// http://www.atmel.com/images/doc2527.pdf
// http://de.mathworks.com/matlabcentral/fileexchange/45840-complete-pan-tompkins-implementation-ecg-qrs-detector/content/pan_tompkin.m
// http://cnx.org/contents/YR1BUs9_@1/QRS-Detection-Using-Pan-Tompki

#ifndef PULSEDETECTOR_H
#define PULSEDETECTOR_H

#include <stdint.h>

#define PULSEDETECTOR_INIT_HOLDOFF                2000    // in ms, how long to wait before counting
#define PULSEDETECTOR_MASKING_HOLDOFF             200     // in ms, non-retriggerable window after beat detection
#define PULSEDETECTOR_BPFILTER_ALPHA              0.5     // EMA factor for the beat period value
#define PULSEDETECTOR_MIN_THRESHOLD               30      // minimum threshold (filtered) value
#define PULSEDETECTOR_MAX_THRESHOLD               500     // maximumg threshold (filtered) value
#define PULSEDETECTOR_THRESHOLD_FALLOFF_TARGET    0.3     // thr chasing factor of the max value when beat
#define PULSEDETECTOR_THRESHOLD_DECAY_FACTOR      0.99    // thr chasing factor when no beat
#define PULSEDETECTOR_INVALID_READOUT_DELAY       2000    // in ms, no-beat time to cause a reset
#define PULSEDETECTOR_SAMPLES_PERIOD              10      // in ms, 1/Fs


typedef enum PulseDetectorState {
    PULSEDETECTOR_STATE_INIT,
    PULSEDETECTOR_STATE_WAITING,
    PULSEDETECTOR_STATE_FOLLOWING_SLOPE,
    PULSEDETECTOR_STATE_MASKING
} PulseDetectorState;


class PulseDetector
{
public:
    PulseDetector();
    void addSample(float sample);
    float getHeartRate();
    float getCurrentThreshold();

private:
    void checkForBeat(float value);
    void decreaseThreshold();

    PulseDetectorState state;
    float threshold;
    float beatPeriod;
    float lastMaxValue;
    uint32_t tsLastPulse;
};

#endif
