arduino-cli compile /home/sasa/Arduino/SoftRF-LY1.8-SM \
  -e \
  --libraries /home/sasa/Arduino/SoftRF-LY1.8-SM/libraries_ly \
  -b "esp32:esp32:esp32s3:USBMode=default,CDCOnBoot=cdc,CPUFreq=80,FlashSize=8M,PartitionScheme=default_8MB,EraseFlash=all" \
  --jobs 12 \
  --verbose 
#  --clean

###
# sa ovim radi
#  -b "esp32:esp32:esp32s3:USBMode=default,CDCOnBoot=cdc,CPUFreq=80,FlashSize=8M,PartitionScheme=default_8MB,EraseFlash=all" \
#
# Sketch uses 1934665 bytes (57%) of program storage space. Maximum is 3342336 bytes.
# Global variables use 112168 bytes (34%) of dynamic memory, leaving 215512 bytes for local variables. Maximum is 327680 bytes.

###
# sa ovim ne radi serija preko USB al ostane više RAM-a ali malo manje nego 
#  -b "esp32:esp32:esp32s3:USBMode=default,CPUFreq=80,FlashSize=8M,PartitionScheme=default_8MB,EraseFlash=all" \

##
# arduino-cli board details -b esp32:esp32:esp32s3
