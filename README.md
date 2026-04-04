# SoftRF LY1.8 Firmware (by Linar, modified)

This repository contains the SoftRF LY1.8 firmware source code, originally developed by [Linar Yusupov](https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/SoftRF). 

## Modifications

- Small adjustments for personal use with ESP32-S3:
  - Global NMEA sequence conversion: any sentence not starting with `$GP` is converted to `$GPx`
  - Checksum recalculated to maintain valid NMEA format
- Ensures devices correctly interpret GPS/variometer data
- Firmware version set to 1.8SM
- Removed unnecessary build artifacts
- Future plan: allow configurable enable/disable of NMEA modification via standard firmware config

## Build Notes

- Compilation scripts are located in the Arduino folder outside this repository
- This repo contains only the firmware source, not the full project structure

## Original Project

- Based on the original SoftRF project by [Linar Yusupov](https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/SoftRF)
