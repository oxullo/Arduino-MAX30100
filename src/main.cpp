#include <Arduino.h>

#include "max30100.h"
#include "qrsdetector.h"
#include "filter.h"

MAX30100 hrm;
QRSDetector qrsDetector;
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
        qrsDetector.addSample(filteredValue);

        Serial.print("R:");
        Serial.print(filteredValue);
        Serial.print(" ");
        Serial.println(qrsDetector.getCurrentThreshold());

        t1 = millis();
    }

    if (millis() - t2 > 1000) {
        Serial.print("H:");
        Serial.println(qrsDetector.getHeartRate());

        t2 = millis();
    }
}
