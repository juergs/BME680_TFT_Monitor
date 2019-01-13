//**********************************************************
// Visual Micro is in vMicro>General>Tutorial Mode
// https://www.visualmicro.com/page/User-Guide.aspx?doc=Visual-Micro-Menu.html
//**********************************************************
/*
 An example analogue meter using a ILI9341 TFT LCD screen

 This example uses the hardware SPI only and a board based
 on the ATmega328

 Needs Fonts 2, and 7 (also Font 4 if using large scale label)

 Comment out lines 153 and 197 to reduce needle flicker and
 to remove need for Font 4 (which uses ~8k of FLASH!)
 
    https://www.instructables.com/id/Arduino-sketch-for-a-retro-analogue-meter-graphic-/
  
  STM32-Working F103 TFT-Libs, but mirrored with 128x240 instead of 128x160 Pixel setting.
  Feature disabled, by adding setRotation-mode type 5 in landscape ( MADCTL_MV | MADCTL_MY| MADCTL_RGB)
    
    https://github.com/victorpv/Arduino_STM32/tree/master/STM32F1/libraries
    https://arduino.stackexchange.com/questions/13429/deprecated-conversion-from-string-constant-to-char

 Alternative:
    http://forum.arduino.cc/index.php?topic=181679.30

Using the first SPI port (SPI_1)
    SS    <-->  PA4 <-->  BOARD_SPI1_NSS_PIN
    SCK   <-->  PA5 <-->  BOARD_SPI1_SCK_PIN
    MISO  <-->  PA6 <-->  BOARD_SPI1_MISO_PIN
    MOSI  <-->  PA7 <-->  BOARD_SPI1_MOSI_PIN

Using the second SPI port (SPI_2)
    SS    <-->  PB12 <-->  BOARD_SPI2_NSS_PIN
    SCK   <-->  PB13 <-->  BOARD_SPI2_SCK_PIN
    MISO  <-->  PB14 <-->  BOARD_SPI2_MISO_PIN
    MOSI  <-->  PB15 <-->  BOARD_SPI2_MOSI_PIN

#include <SPI.h>

#define SPI1_NSS_PIN PA4    //SPI_1 Chip Select pin is PA4. You can change it to the STM32 pin you want.
#define SPI2_NSS_PIN PB12   //SPI_2 Chip Select pin is PB12. You can change it to the STM32 pin you want.
    
 Maple-STM32:   #elif defined (__arm__)

 MapleMini:
 Serial.print("Hello world"); will print via Serial USB (CDC).
 Serial1 prints to hardware USART 1
 Serial2 prints to hardware USART 2
 http://docs.leaflabs.com/static.leaflabs.com/pub/leaflabs/maple-docs/0.0.12/lang/api/serial.html
 https://github.com/rogerclarkmelbourne/Arduino_STM32/blob/master/STM32F1/libraries/A_STM32_Examples/examples/General/BlinkNcount/BlinkNcount.ino

 Language Index:
 http://docs.leaflabs.com/static.leaflabs.com/pub/leaflabs/maple-docs/0.0.12/language-index.html
 
 Room for improvements:
 https://github.com/sticilface/Tasker
 
 Binary-Upload:
 Program BME680_Maple_TFT_Monitor size: 69.976 bytes (used 63% of a 110.592 byte maximum)
 ...Documents\Arduino\hardware\Arduino_STM32\tools\win\maple_upload.bat COM18 1 1EAF:0003 C:\Users\juergs\AppData\Local\Temp\VMBuilds\BME680~3\ARDUIN~1\Release/BME680~1.BIN

 Credentials:
 Alan Senior 23/2/2015 & juergs 01/12/2019 enhancements for BME680 aquisition.

 Remarks:
 ========
 20190112_juergs: Anpassungen und BME680-Implementierung. 
 20190112_juergs: "Adafruit_ILI9341_STM.cpp" changed from BRG to RGB color scheme @setRotation(5)   
 20190112_juergs: minor changes overwrite rectangle for textual output inside meter. 
==============================================================================================================================*/
//--- including all necessary, also specialized, adapted stuff. (SPI + GFX_AS)  
#include <Adafruit_ILI9341_STM.h>
#include <Adafruit_GFX_AS.h>
#include <SPI.h>


//===========================================================================================================================
//* GY_MCU680 air quality sensor ARDUINO/ESP8266 wit BME680 sensor connected via serial interface  
//* Author: juergs@fhem 20181230
//--- https://www.norwegiancreations.com/2016/08/double-exponential-moving-average-filter-speeding-up-the-ema/
//===========================================================================================================================
/*  0   1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
*  5A 5A 3F 0F 08 0B 11 1E 01 8F 20 00 19 00 01 98 EE FF BA 4D
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  |  |   |  |  | |  |  | |  |  | |  |  | |  |  |   |  |  |
*  |  --------------------------------------------------------------- Start-Präamble_2 5A
*  |  --------------------------------------------------------------- Start-Präamble_1 5A
*/

//#define USE_PLOTTER_OUTPUT 1
#define VERBOSE 1 

uint16_t temp1 = 0;
int16_t  temp2 = 0;

//int ema_a = 0.06;
//int ema_ema = 0;
//int ema = 0;
float EMA_a = 0.6;      //initialization of EMA alpha (Exponential Moving Average) filtering 
int EMA_S = 0;          //initialization of EMA S
uint8_t buf[30];
unsigned char counter = 0;
unsigned char sign = 0;
//int led = 13;


#define SPI2_NSS_PIN PB12   //SPI_2 Chip Select pin is PB12. You can change it to the STM32 pin you want.
#define SPI1_NSS_PIN PA4    //SPI_1 Chip Select pin is PA4. You can change it to the STM32 pin you want.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// These are the connections for the MapleMini to tft-display
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define sclk  PA5
#define mosi  PA7  
#define cs    PA4   // If cs and dc pin allocations are changed then 
#define dc    PB0   // comment out #define F_AS_T line in "Adafruit_ILI9341_FAST.h" which is inside "Adafruit_ILI9341_AS" library.
#define rst   0     // Can alternatively connect this to the Arduino reset
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define M_SIZE 0.667  //--- Define meter size scale factor for 128x160 instead of 128x240 (basic library setting)

#define ILI9341_GREY 0x5AEB
#define YELLOW 0xFFE0
#define TFT_WHITE 0xFFFF

//--- tft object 
Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(cs, dc, rst); // Invoke custom library

float     ltx = 0;                // Saved x coord of bottom of needle
uint16_t  osx = 120, osy = 120;   // Saved x & y coords
uint32_t  updateTime = 0;         // time for next update
int       old_analog =  -999;   // Value last displayed
int       old_digital = -999;   // Value last displayed
int       value[6] = {0, 0, 0, 0, 0, 0};
int       old_value[6] = { -1, -1, -1, -1, -1, -1};
int       d = 0;
//-------------------------------------------------------------------------
void setup(void) 
{
    //--- using USB - CDC output for debug purpouses
    Serial.begin(115200); // For debug

    //--- using serial1 on Maple ( 2nd of three hardware serial ports ) 
    //--- for BME680-device "QV-MCUBME680V1" 
    Serial1.begin(9600);  

    //--- using special tft-lib, included in zip-files in github project 
    tft.begin();
    tft.fillScreen(ILI9341_BLACK);

    //--- setRotation was originally "3".Now added "5" -feature. Defining landscape mode for 128x160-ST7735 from Sainsmart(!) needs reversed display mode. 
    //--- using BRG-color scheme (stuff added  to library !)
    //--- using RBG-color scheme now!
    tft.setRotation(5); 
    tft.fillScreen(ILI9341_BLACK);

    delay(10); 

    //-- setup device and serial port for continuous output @9600 baud
    setupMcuBme680(); 

    delay(3000);  //--- let arduino ide switch to serial after having flashed binary.

    Serial.println("\n\n*** Setup done. Setting GFX.");

    //tft.setTextColor(ILI9341_DARKCYAN);
    //tft.drawRightString("1234", 1, M_SIZE * 125 + 2, 2);

    //--- setup static display text 


    float y = M_SIZE * 125 + 2;   // Y:85.37F 0<x<85   
    Serial.print("Y:"); Serial.println(y);

    analogMeter(); // Draw analogue meter
    
    updateTime = millis(); // Next update time
}
//-------------------------------------------------------------------------
void loop() 
{
    //--- jump to this loop.
    BME_loop();

    //if (updateTime <= millis()) 
    //{
    //    updateTime = millis() + 35; // Update meter every 35 milliseconds
 
    //    //--- create a sine wave for testing
    //    d += 4; 
    //    if (d >= 360) d = 0;
    //    value[0] = 50 + 50 * sin((d + 0) * 0.0174532925);
    //
    //    //value[0] = random(0,100);
    //    //unsigned long tt = millis();

    //    plotNeedle(value[0], 0); // It takes between 2 and 14ms to replot the needle with zero delay
   
    //    //Serial.println(millis()-tt);
    //}
}
// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
void analogMeter()
{
  //--- draw meter outline
  tft.fillRect(0, 0, M_SIZE*239, M_SIZE*131, ILI9341_GREY);
  tft.fillRect(1, M_SIZE*3, M_SIZE*234, M_SIZE*125, ILI9341_WHITE);


  tft.setTextColor(ILI9341_BLACK);  // Text colour


  //--- draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  //   0   1   2   3   4   5   6   7   8  9   10  11  12  13  14  15  16  17  18  19  20    = 21 Ticks 
  // -50,-45,-40,-35,-30,-25,-20,-15,-10,-5 , 0 , 5 , 10, 15, 20, 25, 30, 35, 40, 45, 50,
  for (int i = -50; i < 51; i += 5) 
  {
    //--- long scale tick length
    int tl = 15;

    //--- coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (M_SIZE*100 + tl) + M_SIZE*120;
    uint16_t y0 = sy * (M_SIZE*100 + tl) + M_SIZE*150;
    uint16_t x1 = sx * M_SIZE*100 + M_SIZE*120;
    uint16_t y1 = sy * M_SIZE*100 + M_SIZE*150;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (M_SIZE*100 + tl) + M_SIZE*120;
    int y2 = sy2 * (M_SIZE*100 + tl) + M_SIZE*150;
    int x3 = sx2 * M_SIZE*100 + M_SIZE*120;
    int y3 = sy2 * M_SIZE*100 + M_SIZE*150;

    //--- green zone limits
    if (i >= -50 && i < -15)
    {
        tft.fillTriangle(x0, y0, x1, y1, x2, y2, ILI9341_GREEN);
        tft.fillTriangle(x1, y1, x2, y2, x3, y3, ILI9341_GREEN);
    }

    //--- yellow zone limits
    if (i >= -15 && i < 0) 
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, ILI9341_YELLOW);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, ILI9341_YELLOW);
    }

    //--- orange zone limits
    if (i >= 0 && i < 25) 
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, ILI9341_ORANGE);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, ILI9341_ORANGE);
    }

    //--- red zone limits
    if (i >= 25 && i < 50) 
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, ILI9341_RED);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, ILI9341_RED);
    }

    //--- short scale tick length
    if (i % 25 != 0) tl = 8;

    //--- recalculate coords incase tick lenght changed
    x0 = sx * (M_SIZE*100 + tl) + M_SIZE*120;
    y0 = sy * (M_SIZE*100 + tl) + M_SIZE*150;
    x1 = sx * M_SIZE*100 + M_SIZE*120;
    y1 = sy * M_SIZE*100 + M_SIZE*150;

    //--- draw tick
    tft.drawLine(x0, y0, x1, y1, ILI9341_BLACK);

    //--- check if labels should be drawn, with position tweaks
    if (i % 25 == 0) 
    {
      //--- calculate label positions
      x0 = sx * (M_SIZE*100 + tl + 10) + M_SIZE*120;
      y0 = sy * (M_SIZE*100 + tl + 10) + M_SIZE*150;
      switch (i / 25) 
      {
        case -2: tft.drawCentreString("0", x0+4, y0-6, 2); break;
        case -1: tft.drawCentreString("125", x0+2, y0-6, 2); break;
        case 0: tft.drawCentreString("250", x0, y0-4, 2); break;
        case 1: tft.drawCentreString("375", x0, y0-6, 2); break;
        case 2: tft.drawCentreString("500", x0-3, y0-12, 2); break;
      }
    }

    //--- draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * M_SIZE*100 + M_SIZE*120;
    y0 = sy * M_SIZE*100 + M_SIZE*150;
    
    //--- draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0, y0, x1, y1, ILI9341_BLACK);
  }

  //--- textuals
  tft.drawString("IAQ", M_SIZE*(3 + 230 - 40), M_SIZE*(119 - 20), 2);    // Units at bottom right
  //--- meter item
  tft.drawCentreString("IAQ", M_SIZE*120, M_SIZE*75, 4);                 // Comment out to avoid font 4
  //--- bezel
  tft.drawRect(1, M_SIZE*3, M_SIZE*236, M_SIZE*126, ILI9341_BLACK);      // Draw bezel line

  plotNeedle(0, 0, 0); // Put meter needle at 0
}
//-------------------------------------------------------------------------
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
//-------------------------------------------------------------------------
void plotNeedle(int value,int org_value, byte ms_delay)
{
  //tft.setTextColor(ILI9341_BLACK , ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK );

  //--- delete from previous drawn letters (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
  //--- spawning for 3 digits 
  tft.fillRect(5
      , 68
      , 30
      , 12
      , ILI9341_WHITE);

  char vbuf[8]; 
  dtostrf( (double) org_value, 4, 0, vbuf);
  /*
      Serial.print("buf:   "); Serial.println(vbuf);
      Serial.print("value: "); Serial.println(value);
      Serial.print("YL: "); Serial.println(M_SIZE*(119 - 20));
  */ 
  tft.drawRightString(vbuf, 33, M_SIZE*(119 - 20), 2);

  //--- limit value to emulate needle end stops
  if (value < -10) value = -10; 
  if (value > 110) value = 110;

  //--- move needle until new value reached
  while (!(value == old_analog)) 
  {
    if (old_analog < value) 
       old_analog++;
    else 
       old_analog--;

    if (ms_delay == 0) old_analog = value;  //--- update immediately, if delay is 0

    float sdeg = map(old_analog, -10, 110, -150, -30); // Map value to angle
    
    //--- calculate tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    //--- calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    //--- erase old needle image
    tft.drawLine(M_SIZE*(120 + 24 * ltx) - 1
                , M_SIZE*(150 - 24)
                , osx - 1
                , osy
                , ILI9341_WHITE);

    
    tft.drawLine(M_SIZE*(120 + 24 * ltx)
                ,M_SIZE*(150 - 24)
                , osx
                , osy
                , ILI9341_WHITE);
    
    tft.drawLine(M_SIZE*(120 + 24 * ltx) + 1
                , M_SIZE*(150 - 24)
                , osx + 1
                , osy
                , ILI9341_WHITE);

    //--- re-plot text under needle
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    
    tft.drawCentreString( "IAQ"
                        , M_SIZE*120
                        , M_SIZE*75
                        , 4); // // Comment out to avoid font 4

    //--- store new needle end coords for next erase
    ltx = tx;
    osx = M_SIZE*(sx * 98 + 120);
    osy = M_SIZE*(sy * 98 + 150);

    //--- draw the needle in the new postion, magenta makes needle a bit bolder
    //--- draws 3 lines to thicken needle
    //--- drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
    tft.drawLine(M_SIZE*(120 + 24 * ltx) - 1   //x0
                ,M_SIZE*(150 - 24)             //y0 
                , osx - 1                      //x1 
                , osy                          //y1 
                , ILI9341_RED);
    
    tft.drawLine(M_SIZE*(120 + 24 * ltx)
                , M_SIZE*(150 - 24)
                , osx
                , osy
                , ILI9341_MAGENTA);
    
    tft.drawLine(M_SIZE*(120 + 24 * ltx) + 1
                , M_SIZE*(150 - 24)
                , osx + 1
                , osy
                , ILI9341_RED);

    //--- low needle down slightly as it approaches new postion
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;

    //--- wait before next update
    delay(ms_delay);
  }
}


//----------------------------------------------------------------------------
int EMA_function(float alpha, int latest, int stored) {
    return round(alpha*latest) + round((1 - alpha)*stored);
}
//----------------------------------------------------------------------------
void setupMcuBme680()
{
    //Serial1.begin(9600);    
    
    delay(1000);

    Serial1.write(0XA5);
    Serial1.write(0X55);
    Serial1.write(0X3F);
    Serial1.write(0X39);
    delay(100);

    Serial1.write(0XA5);
    Serial1.write(0X56);
    Serial1.write(0X02);
    Serial1.write(0XFD);
    delay(100);
    Serial.println("\n\n*** SetupMcuBme680 done.");
}
//----------------------------------------------------------------------------
void BME_loop() 
{
    float Temperature, Humidity;
    unsigned char i = 0, sum = 0;
    uint32_t Gas;
    uint32_t Pressure;
    uint16_t IAQ;
    int16_t  Altitude;
    uint8_t  IAQ_accuracy;
    
    while (Serial1.available())
    {
#ifdef VERBOSE
        //Serial.println("*** Bme680.");
#endif 
        buf[counter] = (unsigned char) Serial1.read();

        if (counter == 0 && buf[0] != 0x5A) return;
        if (counter == 1 && buf[1] != 0x5A)
        {
            counter = 0;
            return;
        };
        counter++;
        if (counter == 20)
        {
            counter = 0;
            sign = 1;
        }
    }
    if (sign)
    {
        sign = 0;
        if (buf[0] == 0x5A && buf[1] == 0x5A)
        {
            for (i = 0; i<19; i++)
                sum += buf[i];

            if (sum == buf[i])
            {

                /*   
                *  //http://forum.arduino.cc/index.php?topic=314674.msg2185385#msg2185385
                *  //https://appelsiini.net/2011/simple-usart-with-avr-libc/
                printf("Temperature: %.2f °C\n", float64(int(buf[0]) << 8 | int(buf[1])) / 100)
                printf("Humidity:    %.2f %%\n", float64(int(buf[0]) << 8 | int(buf[1])) / 100)
                printf("Pressure:    %d Pa\n", int(buf[0]) << 16 | int(buf[1]) << 8 | int(buf[2]))
                printf("IAQ:         %d\n", int(buf[0] >> 4) << 8 | int(buf[1]))
                printf("Gas:         %d ohm\n", int(buf[0]) << 24 | int(buf[1]) << 16 | int(buf[2]) << 8 | int(buf[0]))
                printf("Altitude:    %d m\n", int(buf[0]) << 8 | int(buf[1]))
                */

                temp1 = (buf[6] << 8 | buf[7]);
                temp2 = (buf[4] << 8 | buf[5]);

                Humidity = (float)temp1 / 100;
                Temperature = (float)temp2 / 100;

                Pressure = ((uint32_t)buf[8] << 16) | ((uint16_t)buf[9] << 8) | buf[10];
                IAQ_accuracy = (buf[11] & 0xf0) >> 4;
                IAQ = ((buf[11] & 0x0F) << 8) | buf[12];
                Gas = ((uint32_t)buf[13] << 24) | ((uint32_t)buf[14] << 16) | ((uint16_t)buf[15] << 8) | buf[16];
                Altitude = (buf[17] << 8) | buf[18];

                //--- smoothed IAQ value 
                EMA_S = (EMA_a*IAQ) + ((1 - EMA_a)*EMA_S);    //run the EMA

#ifdef USE_PLOTTER_OUTPUT  
                //https://www.arduino.cc/en/Tutorial/ArduinoSoundAmplitudeSerialPlotter   
                //https://www.norwegiancreations.com/2015/10/tutorial-potentiometers-with-arduino-and-filtering/
                //http://www.farrellf.com/
                //--- Anmerkung:  Ausgaben wegen Skalierung des Plotters etwas angepasst.
                //--- Ctrl + Shift + L                                     
                Serial.print(Temperature);
                Serial.print(",");
                Serial.print(Humidity);
                Serial.print(",");
                Serial.print(Pressure / 1000);
                Serial.print(",");
                Serial.print(IAQ);
                Serial.print(",");
                Serial.print(Gas / 10000);
                Serial.print(",");
                Serial.println(EMA_S);

#else
                //--- Ctrl + Shift + M for serial terminal on arduino ide
   /*             Serial.print("T:");
                Serial.print(Temperature);
                Serial.print(" ,H:");
                Serial.print(Humidity);
                Serial.print(" ,P:");
                Serial.print(Pressure / 100);
                Serial.print("  ,IAQ:");
                Serial.print(IAQ);
                Serial.print(" ,G:");
                Serial.print(Gas);
                Serial.print("  ,A:");
                Serial.print(Altitude);
                Serial.print("  ,IAQ_accuracy:");
                Serial.println(IAQ_accuracy);*/

                int IAQ_m = map(EMA_S,0,500,0,100);
    #ifdef VERBOSE  
                serial_timestamp(); 
                Serial.print(" - "); Serial.print("EMA_S,IAQ,IAQ_m: ");
                Serial.print(EMA_S); Serial.print(","); Serial.print(IAQ_m); Serial.print(","); Serial.println(IAQ);
    #endif
                //--- output IAQ to virtual gauge
                plotNeedle(IAQ_m, IAQ, 0); // It takes between 2 and 14ms to replot the needle with zero delay

#endif 

            }
            delay(1000);

#ifdef VERBOSE           
            //Serial.println("*** Exit Bme680_loop. (RX)");
#endif
        }
        else
        {
#ifdef VERBOSE           
            Serial.println("*** Exit Bme680_loop no RX.");
#endif
        }
    }
}
//---------------------------------------------------------------------
/*!
* @brief           Capture the system time in microseconds
* @return          system_current_time    current system timestamp in microseconds
*/
int64_t get_timestamp_us()
{
    return ((int64_t)millis() * 1000);
}
//---------------------------------------------------------------------
int64_t print_timestamp()
{
    return ((int64_t)get_timestamp_us() / 1e6);
}
//---------------------------------------------------------------------
int64_t serial_timestamp()
{
    //--- a jumper to GND on D10 does the job, when needed! 
    /* if (digitalRead(PIN_ENABLE_TIMESTAMP_IN_OUTPUT) == LOW)
    {*/
    Serial.print("[");
    Serial.print(get_timestamp_us() / 1e6);
    Serial.print("] ");
    //}
}

//-------------------------------
//-------------------------------


