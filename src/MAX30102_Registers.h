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

#ifndef MAX30102_REGISTERS_H
#define MAX30102_REGISTERS_H

#define MAX30102_I2C_ADDRESS                    0x57

// Interrupt status register (RO)
#define MAX30102_REG_INTERRUPT_STATUS_A         0x00
#define MAX30102_IS_PWR_RDY                     (1 << 0)
#define MAX30102_IS_ALC_OVF                     (1 << 5)
#define MAX30102_IS_PPG_RDY                     (1 << 6)
#define MAX30102_IS_A_FULL                      (1 << 7)

// Interrupt status register (R1)
#define MAX30102_REG_INTERRUPT_STATUS_B         0x01
#define MAX30102_IS_TEMP_RDY                    (1 << 1)

// Interrupt enable register E1
#define MAX30102_REG_INTERRUPT_ENABLE_A         0x02
#define MAX30102_IE_ENB_ALC_OVF                 (1 << 5)
#define MAX30102_IE_ENB_PPG_RDY                 (1 << 6)
#define MAX30102_IE_ENB_A_FULL                  (1 << 7)

// Interrupt enable register E2
#define MAX30102_REG_INTERRUPT_ENABLE_B         0x03
#define MAX30102_IE_ENB_TEMP_RDY                (1 << 1)

// FIFO control and data registers
#define MAX30102_REG_FIFO_WRITE_POINTER         0x04
#define MAX30102_REG_FIFO_OVERFLOW_COUNTER      0x05
#define MAX30102_REG_FIFO_READ_POINTER          0x06
#define MAX30102_REG_FIFO_DATA                  0x07  // Burst read does not autoincrement addr

//FIFO config registers
#define MAX30102_REG_FIFO_CONFIG                0x08
#define MAX30102_FC_FIFO_ROLLOVER_EN			1 << 4
typedef enum FIFO_A_Full {
	MAX30102_FC_FIFO_A_FULL_0	= 0x0,
	MAX30102_FC_FIFO_A_FULL_1	= 0x1,
	MAX30102_FC_FIFO_A_FULL_2	= 0x2,
	MAX30102_FC_FIFO_A_FULL_3	= 0x3,
	MAX30102_FC_FIFO_A_FULL_4	= 0x4,
	MAX30102_FC_FIFO_A_FULL_5	= 0x5,
	MAX30102_FC_FIFO_A_FULL_6	= 0x6,
	MAX30102_FC_FIFO_A_FULL_7	= 0x7,
	MAX30102_FC_FIFO_A_FULL_8	= 0x8,
	MAX30102_FC_FIFO_A_FULL_9	= 0x9,
	MAX30102_FC_FIFO_A_FULL_10	= 0xA,
	MAX30102_FC_FIFO_A_FULL_11	= 0xB,
	MAX30102_FC_FIFO_A_FULL_12	= 0xC,
	MAX30102_FC_FIFO_A_FULL_15	= 0xF,
	MAX30102_FC_FIFO_A_FULL_13	= 0xD,
	MAX30102_FC_FIFO_A_FULL_14	= 0xE
	
} FIFO_A_Full;

typedef enum SampleAverage {
	MAX30102_SMP_AVE_1			= 0x0,
	MAX30102_SMP_AVE_2			= 0x1,
	MAX30102_SMP_AVE_4			= 0x2,
	MAX30102_SMP_AVE_8			= 0x3,
	MAX30102_SMP_AVE_16			= 0x4,
	MAX30102_SMP_AVE_32			= 0x5
} SampleAverage;


// Mode Configuration register
#define MAX30102_REG_MODE_CONFIGURATION         0x09
#define MAX30102_MC_RESET                       (1 << 6)
#define MAX30102_MC_SHDN                        (1 << 7)
typedef enum Mode {
    MAX30102_MODE_HRONLY    = 0x02,
    MAX30102_MODE_SPO2_HR   = 0x03,
	MAX30102_MODE_MULTI   	= 0x07
} Mode;

// SpO2 Configuration register
// Check tables 8 and 9, p19 of the MAX30102 datasheet to see the permissible
// combinations of sampling rates and pulse widths
#define MAX30102_REG_SPO2_CONFIGURATION         0x0A

typedef enum ADCRange {
	MAX30102_ADCRange_2048		= 0x0,
	MAX30102_ADCRange_4096		= 0x1,
	MAX30102_ADCRange_8192		= 0x2,
	MAX30102_ADCRange_16384		= 0x3,
} ADCRange;

typedef enum SamplingRate {
    MAX30102_SAMPRATE_50HZ      = 0x00,
	MAX30102_SAMPRATE_100HZ     = 0x01,
	MAX30102_SAMPRATE_167HZ     = 0x02,
	MAX30102_SAMPRATE_200HZ     = 0x03,
	MAX30102_SAMPRATE_400HZ     = 0x04,
	MAX30102_SAMPRATE_600HZ     = 0x05,
	MAX30102_SAMPRATE_800HZ     = 0x06,
	MAX30102_SAMPRATE_1000HZ    = 0x07
} SamplingRate;

typedef enum LEDPulseWidth {
    MAX30102_SPC_PW_69US_15BITS    	= 0x00,
    MAX30102_SPC_PW_118US_16BITS    = 0x01,
    MAX30102_SPC_PW_215US_17BITS    = 0x02,
    MAX30102_SPC_PW_411US_18BITS   	= 0x03
} LEDPulseWidth;

// LED Configuration registers
#define MAX30102_REG_LED_RED_PA          0x0C
#define MAX30102_REG_LED_IR_PA           0x0D

// Multi LED Mode Control registers, refer page 21 of datasheet
#define MAX30102_REG_MULTI_LED_CTRL_1AND2		0x11
#define MAX30102_REG_MULTI_LED_CTRL_3AND4		0x12

typedef enum SlotSetting {
	MAX30102_SLOT_SETTING_OFF		= 0x0,
	MAX30102_SLOT_SETTING_RED		= 0x1,
	MAX30102_SLOT_SETTING_IR		= 0x1
} SlotSetting;	

                      
// Temperature integer part register
#define MAX30102_REG_TEMPERATURE_DATA_INT       0x1F
// Temperature fractional part register
#define MAX30102_REG_TEMPERATURE_DATA_FRAC      0x20

//Temperature emable
#define MAX30102_REG_TEMP_CONFIG				0x21
#define MAX30102_TC_TEMP_EN						1 << 0

// Revision ID register (RO)
#define MAX30102_REG_REVISION_ID                0xfe
// Part ID register
#define MAX30102_REG_PART_ID                    0xff

#define MAX30102_FIFO_DEPTH                     0x20

#endif
