/*
Arduino-MAX30102 oximetry / heart rate integrated sensor library by Shivam Gupta (gupta.shivam1996@gmail.com)

Based on MAX30100 library, Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>
All alogrithms and methods used are from the above author,
I have only modified this enough to make it work with the new MAX30102 sensor.

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

#ifndef MAX30102_H
#define MAX30102_H

#include <stdint.h>

#define CIRCULAR_BUFFER_XS
#include "CircularBuffer.h"
#include "MAX30102_Registers.h"

#define DEFAULT_MODE                MAX30102_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE       MAX30102_SAMPRATE_100HZ
#define DEFAULT_PULSE_WIDTH         MAX30102_SPC_PW_118US_16BITS
#define DEFAULT_RED_LED_CURRENT     0xff
#define DEFAULT_IR_LED_CURRENT      0xff
#define DEFAULT_ADC_RANGE			MAX30102_ADCRange_8192
//Max30102 part ID is 0x15
#define EXPECTED_PART_ID            0x15
//Max30102 fifo size is 32
#define RINGBUFFER_SIZE             32

#define I2C_BUS_SPEED               400000UL

typedef struct {
    uint16_t ir;
    uint16_t red;
} SensorReadout;

class MAX30102 {
public:
    MAX30102();
    bool begin();
    void setMode(Mode mode);
    void setLedsPulseWidth(LEDPulseWidth ledPulseWidth);
    void setSamplingRate(SamplingRate samplingRate);
	void setRangeADC(ADCRange adcRange);
	void setRedLedCurrent(uint8_t redLedCurrent);
	void setIRLedCurrent(uint8_t IRLedCurrent);
    void setSlot1(SlotSetting slotsetting);
	void setSlot2(SlotSetting slotsetting);
	void setSlot3(SlotSetting slotsetting);
	void setSlot4(SlotSetting slotsetting);
    void setHighresModeEnabled(bool enabled);
    void update();
    bool getRawValues(uint16_t *ir, uint16_t *red);
    void resetFifo();
    void startTemperatureSampling();
    bool isTemperatureReady();
    float retrieveTemperature();
    void shutdown();
    void resume();
    uint8_t getPartId();

private:
    CircularBuffer<SensorReadout, RINGBUFFER_SIZE> readoutsBuffer;

    uint8_t readRegister(uint8_t address);
    void writeRegister(uint8_t address, uint8_t data);
    void burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length);
    void readFifoData();
};

#endif
