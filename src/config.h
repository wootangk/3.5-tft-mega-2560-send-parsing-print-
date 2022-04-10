// These are the pins for some typical shields!
// S6D0154: YP=A1, XM=A2, YM=7, XP=6
// ST7783 : YP=A2, XM=A1, YM=6, XP=7
// ILI9320: YP=A2, XM=A3, YM=8, XP=9
// ILI9325: YP=A1, XM=A2, YM=7, XP=6
// ILI9325BG: YP=A2, XM=A1, YM=6, XP=7
// ILI9341: YP=A2, XM=A1, YM=7, XP=6
// ILI9488: YP=A1, XM=A2, YM=7, XP=6
// R65109V: YP=A2, XM=A1, YM=6, XP=7

// most mcufriend shields use these pins and Portrait mode:
uint8_t YP = 55;  // must be an analog pin, use "An" notation!
uint8_t XM = 56;  // must be an analog pin, use "An" notation!
uint8_t YM = 7;   // can be a digital pin
uint8_t XP = 6;   // can be a digital pin
uint8_t SwapXY = 1;

//Portrait Mode
//uint16_t TS_LEFT = 153;
//uint16_t TS_RT  = 884;
//uint16_t TS_TOP = 123;
//uint16_t TS_BOT = 920;

//Orientation 3
uint16_t TS_LEFT = 920;
uint16_t TS_RT  = 123;
uint16_t TS_TOP = 153;
uint16_t TS_BOT = 884;


#define MINPRESSURE 20
#define MAXPRESSURE 1000



#define TFT_CS 53
#define TFT_DC 48


int16_t BOXSIZE;
int16_t PENRADIUS = 3;
uint16_t identifier, oldcolor, currentcolor;
uint8_t Orientation = 1;    //PORTRAIT



