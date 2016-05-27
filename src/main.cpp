#include <Arduino.h>

#include "max30100.h"

MAX30100 hrm;

void setup()
{
    Serial.begin(115200);
    hrm.begin();
    hrm.setMode(MAX30100_MODE_SPO2_HR);
}

void loop()
{
    hrm.update();
    Serial.print("IR=");
    Serial.print(hrm.rawIRValue);
    Serial.print(" RED=");
    Serial.println(hrm.rawRedValue);
}
