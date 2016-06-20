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

#ifndef PULSEOXIMETER_H
#define PULSEOXIMETER_H

#define SAMPLING_FREQUENCY                  100
#define HEARTRATE_REPORTING_PERIOD_MS       1000
#define CURRENT_ADJUSTMENT_PERIOD_MS        500
#define IR_LED_CURRENT                      MAX30100_LED_CURR_50MA
#define RED_LED_CURRENT_START               MAX30100_LED_CURR_27_1MA
#define DC_REMOVER_ALPHA                    0.95

#include <stdint.h>

#include "max30100.h"
#include "beatdetector.h"
#include "filter.h"


class PulseOximeter {
public:
    PulseOximeter();

    void begin();
    void update();

private:
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t tsLastCurrentAdjustment;
    uint8_t beatsDetectedNum;
    uint32_t samplesRecorded;
    BeatDetector beatDetector;
    DCRemover irDCRemover;
    DCRemover redDCRemover;
    FilterBuLp1 lpf;
    uint8_t redLedPower;
    float irACValueSqSum;
    float redACValueSqSum;
    MAX30100 hrm;
};
#endif
