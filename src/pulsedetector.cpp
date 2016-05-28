#include <Arduino.h>

#include "pulsedetector.h"

PulseDetector::PulseDetector() :
    state(PULSEDETECTOR_STATE_INIT),
    threshold(PULSEDETECTOR_MIN_THRESHOLD),
    beatPeriod(0),
    lastMaxValue(0),
    tsLastPulse(0)
{
}

void PulseDetector::addSample(float sample)
{
    checkForBeat(sample);
}

float PulseDetector::getHeartRate()
{
    if (beatPeriod != 0) {
        return 1 / beatPeriod * 1000 * 60;
    } else {
        return 0;
    }
}

float PulseDetector::getCurrentThreshold()
{
    return threshold;
}

void PulseDetector::checkForBeat(float sample)
{
    switch (state) {
        case PULSEDETECTOR_STATE_INIT:
            if (millis() > PULSEDETECTOR_INIT_HOLDOFF) {
                state = PULSEDETECTOR_STATE_WAITING;
            }
            break;

        case PULSEDETECTOR_STATE_WAITING:
            if (sample > threshold) {
                threshold = min(sample, PULSEDETECTOR_MAX_THRESHOLD);
                state = PULSEDETECTOR_STATE_FOLLOWING_SLOPE;
            }

            // Tracking lost, resetting
            if (millis() - tsLastPulse > PULSEDETECTOR_INVALID_READOUT_DELAY) {
                beatPeriod = 0;
                lastMaxValue = 0;
            }

            decreaseThreshold();
            break;

        case PULSEDETECTOR_STATE_FOLLOWING_SLOPE:
            if (sample > threshold) {
                threshold = min(sample, PULSEDETECTOR_MAX_THRESHOLD);
            } else {
                // Found a beat
                lastMaxValue = sample;
                state = PULSEDETECTOR_STATE_MASKING;
                float delta = millis() - tsLastPulse;
                if (delta) {
                    beatPeriod = PULSEDETECTOR_BPFILTER_ALPHA * delta +
                            (1 - PULSEDETECTOR_BPFILTER_ALPHA) * beatPeriod;
                }

                tsLastPulse = millis();
            }
            break;

        case PULSEDETECTOR_STATE_MASKING:
            if (millis() - tsLastPulse > PULSEDETECTOR_MASKING_HOLDOFF) {
                state = PULSEDETECTOR_STATE_WAITING;
            }
            decreaseThreshold();
            break;
    }
}

void PulseDetector::decreaseThreshold()
{
    // When a valid pulse rate readout is present, target the
    if (lastMaxValue > 0 && beatPeriod > 0) {
        threshold -= lastMaxValue * (1 - PULSEDETECTOR_THRESHOLD_FALLOFF_TARGET) /
                (beatPeriod / PULSEDETECTOR_SAMPLES_PERIOD);
    } else {
        // Asymptotic decay
        threshold *= PULSEDETECTOR_THRESHOLD_DECAY_FACTOR;
    }

    if (threshold < PULSEDETECTOR_MIN_THRESHOLD) {
        threshold = PULSEDETECTOR_MIN_THRESHOLD;
    }
}
