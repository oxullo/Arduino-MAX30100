#ifndef MAX30100_REGS_H
#define MAX30100_REGS_H

#define MAX30100_I2C_ADDRESS                    0x57

// Interrupt status register (RO)
#define MAX30100_REG_INTERRUPT_STATUS           0x00
#define MAX30100_IS_PWR_RDY                     (1 << 0)
#define MAX30100_IS_SPO2_RDY                    (1 << 4)
#define MAX30100_IS_HR_RDY                      (1 << 5)
#define MAX30100_IS_TEMP_RDY                    (1 << 6)
#define MAX30100_IS_A_FULL                      (1 << 7)

// Interrupt enable register
#define MAX30100_REG_INTERRUPT_ENABLE           0x01
#define MAX30100_IE_ENB_SPO2_RDY                (1 << 4)
#define MAX30100_IE_ENB_HR_RDY                  (1 << 5)
#define MAX30100_IE_ENB_TEMP_RDY                (1 << 6)
#define MAX30100_IE_ENB_A_FULL                  (1 << 7)

// FIFO control and data registers
#define MAX30100_REG_FIFO_WRITE_POINTER         0x02
#define MAX30100_REG_FIFO_OVERFLOW_COUNTER      0x03
#define MAX30100_REG_FIFO_READ_POINTER          0x04
#define MAX30100_REG_FIFO_DATA                  0x05  // Burst read does not autoincrement addr

// Mode Configuration register
#define MAX30100_REG_MODE_CONFIGURATION         0x06
#define MAX30100_MC_TEMP_EN                     (1 << 3)
#define MAX30100_MC_RESET                       (1 << 6)
#define MAX30100_MC_SHDN                        (1 << 7)

// SpO2 Configuration register
// Check tables 8 and 9, p19 of the MAX30100 datasheet to see the permissible
// combinations of sampling rates and pulse widths
#define MAX30100_REG_SPO2_CONFIGURATION         0x07
#define MAX30100_SPC_SPO2_HI_RES_EN             (1 << 6)
#define MAX30100_SPC_RATE_50HZ                  0x00
#define MAX30100_SPC_RATE_100HZ                 0x01
#define MAX30100_SPC_RATE_167HZ                 0x02
#define MAX30100_SPC_RATE_200HZ                 0x03
#define MAX30100_SPC_RATE_400HZ                 0x04
#define MAX30100_SPC_RATE_600HZ                 0x05
#define MAX30100_SPC_RATE_800HZ                 0x06
#define MAX30100_SPC_RATE_1000HZ                0x07
#define MAX30100_SPC_PW_200US_13BITS            0x00
#define MAX30100_SPC_PW_400US_14BITS            0x01
#define MAX30100_SPC_PW_800US_15BITS            0x02
#define MAX30100_SPC_PW_1600US_16BITS           0x03

// LED Configuration register
#define MAX30100_REG_LED_CONFIGURATION          0x09
#define MAX30100_LC_IR_CURR_0MA                 0x00
#define MAX30100_LC_IR_CURR_4_4MA               0x01
#define MAX30100_LC_IR_CURR_7_6MA               0x02
#define MAX30100_LC_IR_CURR_11MA                0x03
#define MAX30100_LC_IR_CURR_14_2MA              0x04
#define MAX30100_LC_IR_CURR_17_4MA              0x05
#define MAX30100_LC_IR_CURR_20_8MA              0x06
#define MAX30100_LC_IR_CURR_24MA                0x07
#define MAX30100_LC_IR_CURR_27_1MA              0x08
#define MAX30100_LC_IR_CURR_30_6MA              0x09
#define MAX30100_LC_IR_CURR_33_8MA              0x0a
#define MAX30100_LC_IR_CURR_37MA                0x0b
#define MAX30100_LC_IR_CURR_40_2MA              0x0c
#define MAX30100_LC_IR_CURR_43_6MA              0x0d
#define MAX30100_LC_IR_CURR_46_8MA              0x0e
#define MAX30100_LC_IR_CURR_50MA                0x0f
#define MAX30100_LC_RED_CURR_0MA                0x00
#define MAX30100_LC_RED_CURR_4_4MA              0x10
#define MAX30100_LC_RED_CURR_7_6MA              0x20
#define MAX30100_LC_RED_CURR_11MA               0x30
#define MAX30100_LC_RED_CURR_14_2MA             0x40
#define MAX30100_LC_RED_CURR_17_4MA             0x50
#define MAX30100_LC_RED_CURR_20_8MA             0x60
#define MAX30100_LC_RED_CURR_24MA               0x70
#define MAX30100_LC_RED_CURR_27_1MA             0x80
#define MAX30100_LC_RED_CURR_30_6MA             0x90
#define MAX30100_LC_RED_CURR_33_8MA             0xa0
#define MAX30100_LC_RED_CURR_37MA               0xb0
#define MAX30100_LC_RED_CURR_40_2MA             0xc0
#define MAX30100_LC_RED_CURR_43_6MA             0xd0
#define MAX30100_LC_RED_CURR_46_8MA             0xe0
#define MAX30100_LC_RED_CURR_50MA               0xf0

// Temperature integer part register
#define MAX30100_REG_TEMPERATURE_DATA_INT       0x16
// Temperature fractional part register
#define MAX30100_REG_TEMPERATURE_DATA_FRAC      0x17

// Revision ID register (RO)
#define MAX30100_REG_REVISION_ID                0xfe
// Part ID register
#define MAX30100_REG_PART_ID                    0xff

#endif
