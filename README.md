# Arduino-MAX30100

[![Build Status](https://travis-ci.org/oxullo/Arduino-MAX30100.svg?branch=master)](https://travis-ci.org/oxullo/Arduino-MAX30100)

Arduino library for the Maxim Integrated MAX30100 oximetry / heart rate sensor.

![MAX30100](http://www.mouser.com/images/microsites/Maxim_MAX30100.jpg)

## Hardware

This library has been tested with the MikroElektronika Heart rate click daughterboard:

http://www.mikroe.com/click/heart-rate/

along with an Arduino UNO r3. Any Arduino supporting the Wire library should work.

The only required connection to the sensor is the I2C bus (SDA, SCL lines, pulled up).

An example which shows a possible way to wire up the sensor is shown in
[extras/arduino-wiring.pdf](extras/arduino-wiring.pdf)

Note: The schematics above shows also how to wire up the interrupt line, which is
currently not used by the library.

### Pull-ups

Since the I2C interface is clocked at 400kHz, make sure that the SDA/SCL lines are pulled
up by 4,7kOhm or less resistors.

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
 * MAX_30100_RawData: demonstrates how to access raw data from the sensor

## Troubleshooting

Typical issues when attempting to run the examples:

### The module fails to initialize, getting stuck at the begin()

This is likely to be caused by an improper pullup setup for the I2C lines.
Make sure to use 4,7kOhm resistors, checking if the breakout board in use is equipped
with pullups.

### Sketchy beat frequency readouts

The beat detector uses the IR LED to track the heartbeat. The IR LED is biased
by default at 50mA but it can be adjusted using PulseOximeter::setIRLedCurrent().
Check the _MAX30100_Minimal_ example.
