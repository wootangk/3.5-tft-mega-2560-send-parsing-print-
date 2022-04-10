#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <McuFriendKbv.h>
#include <TouchScreen.h>
 #include <Wire.h>
 #include <SPI.h>
#include "mycolors.h"
#include "config.h"

MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
uint16_t xpos, ypos;  //screen coordinates

#include <iarduino_I2C_connect.h>                     // подключаем библиотеку для соединения arduino по шине I2C

// Объявляем переменные и константы:
iarduino_I2C_connect I2C2;                            // объявляем объект I2C2 для работы c библиотекой iarduino_I2C_connect
const byte           PIN_Button = 2;                  // объявляем константу с указанием номера цифрового вывода, к которому подключена кнопка
const byte           PIN_LED    = 13;                 // объявляем константу с указанием номера цифрового вывода, к которому подключен светодиод
      byte           REG_Array[2];        // объявляем массив из двух элементов, данные которого будут доступны мастеру (для чтения/записи) по шине I2C
      
      
                  
#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;




/*
  This method will draw a vertical bar graph for single input
  it has a rather large arguement list and is as follows

  &d = display object name
  x = position of bar graph (lower left of bar)
  y = position of bar (lower left of bar
  w = width of bar graph
  h =  height of bar graph (does not need to be the same as the max scale)
  loval = lower value of the scale (can be negative)
  hival = upper value of the scale
  inc = scale division between loval and hival
  curval = date to graph (must be between loval and hival)
  dig = format control to set number of digits to display (not includeing the decimal)
  dec = format control to set number of decimals to display (not includeing the decimal)
  barcolor = color of bar graph
  voidcolor = color of bar graph background
  bordercolor = color of the border of the graph
  textcolor = color of the text
  backcolor = color of the bar graph's background
  label = bottom lable text for the graph
  redraw = flag to redraw display only on first pass (to reduce flickering)

*/


/*
  This method will draw a dial-type graph for single input
  it has a rather large arguement list and is as follows

  &d = display object name
  cx = center position of dial
  cy = center position of dial
  r = radius of the dial
  loval = lower value of the scale (can be negative)
  hival = upper value of the scale
  inc = scale division between loval and hival
  sa = sweep angle for the dials scale
  curval = date to graph (must be between loval and hival)
  dig = format control to set number of digits to display (not includeing the decimal)
  dec = format control to set number of decimals to display (not includeing the decimal)
  needlecolor = color of the needle
  dialcolor = color of the dial
  textcolor = color of all text (background is dialcolor)
  label = bottom lable text for the graph
  redraw = flag to redraw display only on first pass (to reduce flickering)
*/


//some functions what we may no longer need for testing, etc...
void show_Serial(void)
{
    Serial.print(F("Found "));

    Serial.println(F(" LCD driver"));
    Serial.print(F("ID=0x"));
    Serial.println(identifier, HEX);
    Serial.println("Screen is " + String(tft.width()) + "x" + String(tft.height()));
    Serial.println("Calibration is: ");
    Serial.println("LEFT = " + String(TS_LEFT) + " RT  = " + String(TS_RT));
    Serial.println("TOP  = " + String(TS_TOP)  + " BOT = " + String(TS_BOT));
    Serial.print("Wiring is: ");
    Serial.println(SwapXY ? "SWAPXY" : "PORTRAIT");
    Serial.println("YP=" + String(YP)  + " XM=" + String(XM));
    Serial.println("YM=" + String(YM)  + " XP=" + String(XP));
}
void show_tft(void)
{
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.print(F("Found "));

    tft.println(F(" LCD"));
    tft.setTextSize(1);
    tft.print(F("ID=0x"));
    tft.println(identifier, HEX);
    tft.println("Screen is " + String(tft.width()) + "x" + String(tft.height()));
    tft.println("Calibration is: ");
    tft.println("LEFT = " + String(TS_LEFT) + " RT  = " + String(TS_RT));
    tft.println("TOP  = " + String(TS_TOP)  + " BOT = " + String(TS_BOT));
    tft.print("\nWiring is: ");
    if (SwapXY) {
        tft.setTextColor(CYAN);
        tft.setTextSize(2);
    }
    tft.println(SwapXY ? "SWAPXY" : "PORTRAIT");
    tft.println("YP=" + String(YP)  + " XM=" + String(XM));
    tft.println("YM=" + String(YM)  + " XP=" + String(XP));
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor((tft.width() - 48) / 2, (tft.height() * 2) / 4);
    tft.print("EXIT");
    tft.setTextColor(YELLOW, BLACK);
    tft.setCursor(0, (tft.height() * 6) / 8);
    tft.print("Touch screen for loc");
    while (1) {
        tp = ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        pinMode(XP, OUTPUT);
        pinMode(YM, OUTPUT);
        if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
        if (tp.x > 450 && tp.x < 570  && tp.y > 450 && tp.y < 570) break;
        tft.setCursor(0, (tft.height() * 3) / 4);
        tft.print("tp.x=" + String(tp.x) + " tp.y=" + String(tp.y) + "   ");
    }
}


/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = flag to redraw graph on fist call only
*/




void setup(void)
{
 Serial.begin(115200);
    // pinMode(MQ7_CO_PIN, INPUT);

    uint16_t tmp;
    tft.begin(20000);

    tft.reset();
    identifier = tft.readID();

tft.invertDisplay(true);
// tft.setRotation(1);

  tft.invertDisplay(false);

  tft.fillScreen(0xCC92);

 // tft.invertDisplay(1);

 
    Wire.setClock(400000);                              // устанавливаем скорость передачи данных по шине I2C = 400кБит/с
  Wire.begin(0x01);                                   // инициируем подключение к шине I2C в качестве ведомого (slave) устройства, с указанием своего адреса на шине.
  I2C2.begin(REG_Array);                              // инициируем возможность чтения/записи данных по шине I2C, из/в указываемый массив
  pinMode(PIN_Button, INPUT);                         // Устанавливаем режим работы вывода кнопки, как вход
  pinMode(PIN_LED, OUTPUT);       

   

    ts = TouchScreen(XP, YP, XM, YM, 300);     //call the constructor AGAIN with new values.
    tft.begin(identifier);
    show_Serial();
    // tft.setRotation();
    tft.fillScreen(BLACK);
    
    // BOXSIZE = tft.width() / 6;
  
tft.setRotation(0);

}


void loop()
{  tft.fillScreen(0x0000);
tft.invertDisplay(true);
   REG_Array[0] = digitalRead(PIN_Button);             // Сохраняем состояние кнопки в 0 ячейку массива REG_Array
  digitalWrite(PIN_LED, REG_Array[1]);                // Управляем светодиодом в соответствии со значением 1 ячейки массива REG_Array

show_tft();


    tp = ts.getPoint();   //tp.x, tp.y are ADC values

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(XP, OUTPUT);
    pinMode(YM, OUTPUT);


      if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        // is controller wired for Landscape ? or are we oriented in Landscape?
        // if (SwapXY != (Orientation & 1)) SWAP(tp.x, tp.y);
        // scale from 0->1023 to tft.width  i.e. left = 0, rt = width
        // most mcufriend have touch (with icons) that extends below the TFT
        // screens without icons need to reserve a space for "erase"
        // scale the ADC values from ts.getPoint() to screen values e.g. 0-239
        xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
        ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
        //tft.println(xpos, ypos);

         Serial.println("x="+tp.x);
         Serial.println("y="+tp.y);
         Serial.println("z="+tp.z);

        // Serial.print("CO VALUE: ");
        // Serial.println("co_value");
Serial.println("x="+tp.x);
      }
}
