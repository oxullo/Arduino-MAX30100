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

MAX30100 hrm;
PulseDetector pulseDetector;
FilterBeBp2 filter;

void setup()
{
    Serial.begin(115200);
    hrm.begin();
    //hrm.setMode(MAX30100_MODE_SPO2_HR);
    //hrm.setLedsCurrent(MAX30100_LED_CURR_11MA, MAX30100_LED_CURR_11MA);
}

void loop()
{
    static uint32_t t1 = 0;
    static uint32_t t2 = 0;

    if (millis() - t1 > 10) {
        hrm.update();
        float filteredValue = filter.step(hrm.rawIRValue);
        pulseDetector.addSample(filteredValue);

        Serial.print("R:");
        Serial.print(filteredValue);
        Serial.print(" ");
        Serial.println(pulseDetector.getCurrentThreshold());

        t1 = millis();
    }

    if (millis() - t2 > 1000) {
        Serial.print("H:");
        Serial.println(pulseDetector.getHeartRate());

        t2 = millis();
    }
}
