# BME680_TFT_Monitor
Monitoring IAQ-values with implementation of a ST7735 Sainsmart TFT display (128x160)

Specially adapted libraries. Suited for Sainsmart 128x160 TFT-display with reversed coordinates to Adafruits-TFT-displays:

Adafruit_GFX_AS
Adafruit_ILI9341
Adafruit_ILI9341_STM

Place Libs in Platform-Arduino-Lib-Folder: C:\Users\jschw\Documents\Arduino\hardware\Arduino_STM32\STM32F1\libraries
In my case: STM32F1-type!

First implementation is using BME680-Serial-Breakout-Board: GY-BME680MCUV1 with serial output,
no need for BSEC-stuff:

![Maple-TFT_2](https://raw.githubusercontent.com/juergs/BME680_TFT_Monitor/master/Maple-TFT_2.png)
