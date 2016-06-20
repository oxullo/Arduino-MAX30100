/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

bool PulseDetector::addSample(float sample)
{
    return checkForBeat(sample);
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

bool PulseDetector::checkForBeat(float sample)
{
    bool beatDetected = false;

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
            if (sample < threshold) {
                state = PULSEDETECTOR_STATE_MAYBE_DETECTED;
            } else {
                threshold = min(sample, PULSEDETECTOR_MAX_THRESHOLD);
            }
            break;

        case PULSEDETECTOR_STATE_MAYBE_DETECTED:
            if (sample + PULSEDETECTOR_STEP_RESILIENCY < threshold) {
                // Found a beat
                beatDetected = true;
                lastMaxValue = sample;
                state = PULSEDETECTOR_STATE_MASKING;
                float delta = millis() - tsLastPulse;
                if (delta) {
                    beatPeriod = PULSEDETECTOR_BPFILTER_ALPHA * delta +
                            (1 - PULSEDETECTOR_BPFILTER_ALPHA) * beatPeriod;
                }

                tsLastPulse = millis();
            } else {
                state = PULSEDETECTOR_STATE_FOLLOWING_SLOPE;
            }
            break;

        case PULSEDETECTOR_STATE_MASKING:
            if (millis() - tsLastPulse > PULSEDETECTOR_MASKING_HOLDOFF) {
                state = PULSEDETECTOR_STATE_WAITING;
            }
            decreaseThreshold();
            break;
    }

    return beatDetected;
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
