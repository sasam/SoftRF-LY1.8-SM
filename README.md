# SoftRF LY1.8 Firmware (by Linar, modified)

This repository contains the SoftRF LY1.8 firmware source code, originally developed by Linar Yusupov (https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/SoftRF).

## Modifications

- Adjustments for ESP32-S3 based devices (e.g., HE/Midi, T-Beam/Prime MkII):
  - NMEA $G... sequence conversion: any sentence starting with $G but not $GP is converted to $GPx
    - Ensures compatibility with LXNav S7 variometers, which previously ignored $G... sentences that were not $GP...
  - Checksum recalculated to maintain valid NMEA format
  - ForceGP option implemented in the web interface: can be enabled or disabled by the user
- Firmware version set to 1.8SM

## Build Script

arduino-cli compile /home/sasa/Arduino/SoftRF-LY1.8 \
  -e \
  --libraries /home/sasa/Arduino/libraries_ly \
  -b "esp32:esp32:esp32s3:USBMode=default,CDCOnBoot=cdc,CPUFreq=80,FlashSize=8M,PartitionScheme=default_8MB,EraseFlash=all" \
  --jobs 12 \
  --verbose
#  --clean

## Supported Devices

- Midi Edition (HE): https://github.com/lyusupov/SoftRF/wiki/Midi-Edition
- Prime Edition MkII (T-Beam): https://github.com/lyusupov/SoftRF/wiki/Prime-Edition-MkII

## Original Project

- Based on the original SoftRF project by Linar Yusupov: https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/SoftRF
