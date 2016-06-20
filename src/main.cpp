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

#include "max30100.h"
#include "pulsedetector.h"
#include "filter.h"

#define SAMPLING_FREQUENCY                  100
#define HEARTRATE_REPORTING_PERIOD_MS       1000
#define CURRENT_ADJUSTMENT_PERIOD_MS        500
#define IR_LED_CURRENT                      MAX30100_LED_CURR_50MA
#define RED_LED_CURRENT_START               MAX30100_LED_CURR_27_1MA
#define DC_REMOVER_ALPHA                    0.95

MAX30100 hrm;
// SaO2 Look-up Table
// http://www.ti.com/lit/an/slaa274b/slaa274b.pdf
const uint8_t spO2lookup[43] = {100,100,100,100,99,99,99,99,99,99,98,98,98,98,
                                98,97,97,97,97,97,97,96,96,96,96,96,96,95,95,
                                95,95,95,95,94,94,94,94,94,93,93,93,93,93};

void setup()
{
    Serial.begin(115200);
    hrm.begin();
    hrm.setMode(MAX30100_MODE_SPO2_HR);
    hrm.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT_START);
}

void loop()
{
    static uint32_t t1 = 0;
    static uint32_t t2 = 0;
    static uint32_t t3 = 0;
    static uint32_t tsLastCurrentAdjustment = 0;
    static uint8_t beatsDetectedNum = 0;
    static uint32_t samplesRecorded = 0;
    static PulseDetector pulseDetector;
    static DCRemover irDCRemover(DC_REMOVER_ALPHA);
    static DCRemover redDCRemover(DC_REMOVER_ALPHA);
    static FilterBuLp1 lpf;
    static uint8_t redLedPower = (uint8_t)RED_LED_CURRENT_START;
    static float irACValueSqSum = 0;
    static float redACValueSqSum = 0;

    if (millis() - t1 > 1.0 / SAMPLING_FREQUENCY * 1000.0) {
        hrm.update();
        float irACValue = irDCRemover.step(hrm.rawIRValue);
        float redACValue = redDCRemover.step(hrm.rawRedValue);

        // The signal fed to the beat detector is mirrored since the cleanest monotonic spike is below zero
        bool beatDetected = pulseDetector.addSample(lpf.step(-irACValue));

        if (pulseDetector.getHeartRate() > 0) {
            irACValueSqSum += irACValue * irACValue;
            redACValueSqSum += redACValue * redACValue;
            ++samplesRecorded;

            if (beatDetected) {
                ++beatsDetectedNum;
                if (beatsDetectedNum == 3) {
                    float acSqRatio = 100.0 * log(redACValueSqSum/samplesRecorded) / log(irACValueSqSum/samplesRecorded);
                    uint8_t index = 0;

                    if (acSqRatio > 66) {
                        index = (uint8_t)acSqRatio - 66;
                    } else if (acSqRatio > 50) {
                        index = (uint8_t)acSqRatio - 50;
                    }
                    Serial.print("O:");
                    Serial.println(spO2lookup[index]);

                    samplesRecorded = 0;
                    redACValueSqSum = 0;
                    irACValueSqSum = 0;
                    beatsDetectedNum = 0;
                }
            }
        }

        Serial.print("R:");
        Serial.print(irACValue);
        Serial.print(",");
        Serial.println(redACValue);

        if (beatDetected) {
            Serial.println("B:1");
        }
        t1 = millis();
    }

    if (millis() - t2 > HEARTRATE_REPORTING_PERIOD_MS) {
        Serial.print("H:");
        Serial.println(pulseDetector.getHeartRate());

        t2 = millis();
    }

    // Follower that adjusts the red led current in order to have comparable DC baselines between
    // red and IR leds. The numbers are really magic: the less possible to avoid oscillations
    if (millis() - t3 > CURRENT_ADJUSTMENT_PERIOD_MS) {
        bool changed = false;
        if (irDCRemover.getDCW() - redDCRemover.getDCW() > 70000 && redLedPower < MAX30100_LED_CURR_50MA) {
            ++redLedPower;
            changed = true;
        } else if (redDCRemover.getDCW() - irDCRemover.getDCW() > 70000 && redLedPower > 0) {
            --redLedPower;
            changed = true;
        }

        if (changed) {
            Serial.print("C:");
            Serial.print((uint8_t)IR_LED_CURRENT);
            Serial.print(",");
            Serial.println(redLedPower);
            hrm.setLedsCurrent(IR_LED_CURRENT, (LEDCurrent)redLedPower);
            tsLastCurrentAdjustment = millis();
        }

        t3 = millis();
    }
}
