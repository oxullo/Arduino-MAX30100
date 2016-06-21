# Arduino-MAX30100

Arduino library for the Maxim MAX30100 oximetry / heart rate integrated sensor.

## Hardware

This library has been tested with the MikroElektronika Heart rate click daughterboard:

http://www.mikroe.com/click/heart-rate/

The only requirement for the library to work is to have the sensor wired to the
host microcontroller's I2C bus.

## Architecture

The library offers a low-level driver class, MAX30100.
This component allows for low level communication with the device.

A rather simple but working implementation of the heart rate and SpO2 calculation
can be found in the PulseOximeter class.

This high level class sets up the sensor and data processing pipelines in order to
offer a very simple interface to the data:

 * Sampling frequency set to 100Hz
 * 1600uS pulse width, full sampling 16bit dynamic
 * IR LED current set to 50mA
 * Heart-rate + SpO2 mode

The PulseOximeter class is not optimised for battery-based projects.

## Examples

The included examples show how to use the PulseOximeter class:

 * MAX30100_Minimal: a minimal example that dumps human-readable results via serial
 * MAX30100_Debug: used in conjunction with the Processing pde "rolling_graph" (extras folder), to show the sampled data at various processing stages

