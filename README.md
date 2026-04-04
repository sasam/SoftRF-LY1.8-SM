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

## Build Script

- The build script for compiling the firmware is located outside this repository (`Arduino/compile_LY.sh`)
- Example usage from Arduino folder:

```bash
arduino-cli compile /home/sasa/Arduino/SoftRF-LY1.8 \
  -e \
  --libraries /home/sasa/Arduino/libraries_ly \
  -b "esp32:esp32:esp32s3:USBMode=default,CDCOnBoot=cdc,CPUFreq=80,FlashSize=8M,PartitionScheme=default_8MB,EraseFlash=all" \
  --jobs 12 \
  --verbose
#  --clean
