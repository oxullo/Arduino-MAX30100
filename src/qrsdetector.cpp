#include <Arduino.h>

#include "qrsdetector.h"

QRSDetector::QRSDetector() :
    state(QRSDETECTOR_STATE_INIT),
    threshold(QRSDETECTOR_MIN_THRESHOLD),
    beatPeriod(0),
    lastMaxValue(0),
    tsLastPulse(0)
{
}

float QRSDetector::addSample(float sample)
{
    float filteredValue = filter.step(sample);

    checkForBeat(filteredValue);

    return filteredValue;
}

float QRSDetector::getHeartRate()
{
    if (beatPeriod != 0) {
        return 1 / beatPeriod * 1000 * 60;
    } else {
        return 0;
    }
}

float QRSDetector::getCurrentThreshold()
{
    return threshold;
}

void QRSDetector::checkForBeat(float sample)
{
    switch (state) {
        case QRSDETECTOR_STATE_INIT:
            if (millis() > QRSDETECTOR_INIT_HOLDOFF) {
                state = QRSDETECTOR_STATE_WAITING;
            }
            break;

        case QRSDETECTOR_STATE_WAITING:
            if (sample > threshold) {
                threshold = min(sample, QRSDETECTOR_MAX_THRESHOLD);
                state = QRSDETECTOR_STATE_FOLLOWING_SLOPE;
            }

            // Tracking lost, resetting
            if (millis() - tsLastPulse > QRSDETECTOR_INVALID_READOUT_DELAY) {
                beatPeriod = 0;
                lastMaxValue = 0;
            }

            decreaseThreshold();
            break;

        case QRSDETECTOR_STATE_FOLLOWING_SLOPE:
            if (sample > threshold) {
                threshold = min(sample, QRSDETECTOR_MAX_THRESHOLD);
            } else {
                // Found a beat
                lastMaxValue = sample;
                state = QRSDETECTOR_STATE_MASKING;
                float delta = millis() - tsLastPulse;
                if (delta) {
                    beatPeriod = QRSDETECTOR_BPFILTER_ALPHA * delta +
                            (1 - QRSDETECTOR_BPFILTER_ALPHA) * beatPeriod;
                }

                tsLastPulse = millis();
            }
            break;

        case QRSDETECTOR_STATE_MASKING:
            if (millis() - tsLastPulse > QRSDETECTOR_MASKING_HOLDOFF) {
                state = QRSDETECTOR_STATE_WAITING;
            }
            decreaseThreshold();
            break;
    }
}

void QRSDetector::decreaseThreshold()
{
    // When a valid pulse rate readout is present, target the
    if (lastMaxValue > 0 && beatPeriod > 0) {
        threshold -= lastMaxValue * (1 - QRSDETECTOR_THRESHOLD_FALLOFF_TARGET) /
                (beatPeriod / QRSDETECTOR_SAMPLES_PERIOD);
    } else {
        // Asymptotic decay
        threshold *= QRSDETECTOR_THRESHOLD_DECAY_FACTOR;
    }

    if (threshold < QRSDETECTOR_MIN_THRESHOLD) {
        threshold = QRSDETECTOR_MIN_THRESHOLD;
    }
}
