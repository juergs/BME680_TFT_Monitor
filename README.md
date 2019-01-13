# BME680_TFT_Monitor
Monitoring IAQ-values with implementation of a ST7735 Sainsmart TFT display (128x160)

Specially adapted libraries. Suited for Sainsmart 128x160 TFT-display with reversed coordinates to Adafruits-TFT-displays:

Adafruit_GFX_AS
Adafruit_ILI9341
Adafruit_ILI9341_STM

Place Libs in Platform-Arduino-Lib-Folder: C:\Users\jschw\Documents\Arduino\hardware\Arduino_STM32\STM32F1\libraries
In my case: STM32F1-type!

Preliminary first implementation is using BME680-Serial-Breakout-Board: GY-BME680MCUV1 with serial output,
no need for BSEC-stuff:

![Maple-TFT_2](https://raw.githubusercontent.com/juergs/BME680_TFT_Monitor/master/Maple-TFT_2.png)

Benefits taken from their valuable work:

[Meteo clock](https://create.arduino.cc/projecthub/edr1924/gorgy-meteo-clock-1bfc49)

[Meter](https://www.instructables.com/id/Arduino-sketch-for-a-retro-analogue-meter-graphic-/)

[STM32-GFX-Lib](https://github.com/victorpv/Arduino_STM32/tree/master/STM32F1/libraries)

[STM32-Serial](http://docs.leaflabs.com/static.leaflabs.com/pub/leaflabs/maple-docs/0.0.12/lang/api/serial.html)

[STM32_Serial_Sample](https://github.com/rogerclarkmelbourne/Arduino_STM32/blob/master/STM32F1/libraries/A_STM32_Examples/examples/General/BlinkNcount/BlinkNcount.ino)

[STM32_Development](http://www.farrellf.com/)

[Language Index](http://docs.leaflabs.com/static.leaflabs.com/pub/leaflabs/maple-docs/0.0.12/language-index.html)

[Effective Smoothing](https://www.norwegiancreations.com/2016/08/double-exponential-moving-average-filter-speeding-up-the-ema/)
 
Room for improvements:
https://github.com/sticilface/Tasker

[details in fhem forum](https://forum.fhem.de/index.php/topic,78619.435.html)
