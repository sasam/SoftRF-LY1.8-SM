# SoftRF 1.8 Firmware (by Linar Yusupov, modified)

This repository contains the SoftRF LY1.8 firmware source code, originally developed by Linar Yusupov (https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/SoftRF).

## Modifications

#### NMEA / LXNAV compatibility
- Adjustments for ESP32-S3 based devices (for example HE/Midi and T-Beam/Prime MkII)
- NMEA `$G...` sequence conversion: any sentence starting with `$G` but not `$GP` is converted to `$GP...`
- Checksum is recalculated after conversion to keep the NMEA sentence valid
- This improves compatibility with LXNav variometers that accept only `GPx` NMEA talker IDs
- `ForceGP` option implemented in the web interface, so the conversion can be enabled or disabled by the user

#### Screen control (Heltec Wireless Tracker / HE-Midi)
- Added screen ON / OFF control through the web interface
- The screen is controlled through GPIO21
- Two links/buttons are available on the main web status page:
  - `Screen ON`
  - `Screen OFF`
- This allows the display to be turned off during operation in order to reduce power consumption
- Measured current reduction with screen OFF is approximately 20 mA
  - Screen ON: about 180 mA
  - Screen OFF: about 160 mA
  - Screen OFF, Wi-Fi OFF: about 85 mA

#### Status LED blink tuning

- Implemented and tested on Heltec / HE-Midi.
- The white status LED is mapped to GPIO18. Its blink behavior was retuned to reduce visible brightness and average LED on-time. Behavior on other boards has not been verified.
- In the current implementation:
  - low battery is indicated by a repeating short pulse
  - no GNSS fix is indicated by a single short pulse with a longer interval
  - valid GNSS fix is indicated by a double short pulse with a longer interval
- The GNSS indication uses very short pulses, and the low-battery indication was updated to use short-pulse blinking as well.

## Build Script
```
arduino-cli compile /home/sasa/Arduino/SoftRF-LY1.8 \
  -e \
  --libraries /home/sasa/Arduino/libraries_ly \
  -b "esp32:esp32:esp32s3:USBMode=default,CDCOnBoot=cdc,CPUFreq=80,FlashSize=8M,PartitionScheme=default_8MB,EraseFlash=all" \
  --jobs 12 \
  --verbose 
```
## Supported Devices

- Midi Edition (HE): https://github.com/lyusupov/SoftRF/wiki/Midi-Edition
- Prime Edition MkII (T-Beam): https://github.com/lyusupov/SoftRF/wiki/Prime-Edition-MkII

## Original Project

- Based on the original SoftRF project by Linar Yusupov: https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/SoftRF
