#include <Arduino.h>

#include "qrsdetector.h"

QRSDetector::QRSDetector() :
    state(QRSDETECTOR_STATE_INIT),
    threshold(QRSDETECTOR_MIN_THRESHOLD),
    heartRate(0),
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
    return heartRate;
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
                threshold = sample;
                state = QRSDETECTOR_STATE_FOLLOWING_SLOPE;
            }

            if (millis() - tsLastPulse > QRSDETECTOR_INVALID_READOUT_DELAY) {
                heartRate = 0;
            }

            decreaseThreshold();
            break;

        case QRSDETECTOR_STATE_FOLLOWING_SLOPE:
            if (sample > threshold) {
                threshold = sample;
            } else {
                state = QRSDETECTOR_STATE_MASKING;
                float delta = millis() - tsLastPulse;
                if (delta) {
                    float presentHeartRate = 1 / delta * 1000 * 60;
                    heartRate = QRSDETECTOR_HRFILTER_ALPHA * presentHeartRate +
                            (1 - QRSDETECTOR_HRFILTER_ALPHA) * heartRate;
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
    threshold *= QRSDETECTOR_THRESHOLD_DECAY_FACTOR;
    if (threshold < QRSDETECTOR_MIN_THRESHOLD) {
        threshold = QRSDETECTOR_MIN_THRESHOLD;
    }
}
