#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define NUM_LEDS 60

#define BRIGHTNESS 200

int colorInputPin = A0;
int tinyInputPin = A1;
int colorInputValue = 0;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int whiteValue = 0;
int inputMax = 1024;
int brightnessValue = 0;
int wait = 10;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(colorInputPin, INPUT);
  pinMode(tintInputPin, INPUT);
}

void loop() {
  // read the value from the input:
  setColorWheelInput();
  setTintWheelInput();

  //loop through the pixels and set thier values
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(redValue,greenValue,blueValue,whiteValue)) );
        }
        strip.show();
      }
        delay(wait);


}

//take input from color wheel and set color values
void setColorWheelInput(){
  colorInputValue = analogRead(colorInputPin);

 // start at full red
 // increase green    0     - 255
 // decrease red      256   - 511
 // increase blue     512   -767
 // decrease green    768   - 1023
 // increase red      1024  - 1279
 // decrease blue     1280  - 1535
  x = map(colorInputValue, 0, inputMax, 0, 1535);
  if (x<=255) {
    redValue = 255;
    greenValue = x;
    blueValue = 0;
  }else if (x>=256 && x<=511){
    redValue = 511-x;
    greenValue = 255;
    blueValue = 0;
  }else if (x>=512 && x<=767){
    redValue = 0;
    greenValue = 255;
    blueValue = x-512;
  }else if (x>=768 && x<=1023){
    redValue = 0;
    greenValue = 1023-x;
    blueValue = 255;
  }else if (x>=1024 && x<=1279){
    redValue = x-1024;
    greenValue = 0;
    blueValue = 255;
  }else if (x>=1280 && x<=1536){
    redValue = 255;
    greenValue = 0;
    blueValue = 1536-x;
  }
  
}

//take input from tiny wheel and set brightness and white values
void setTintWheelInput(){
  colorTintValue = analogRead(colorTintPin);

 // start at brightness and white off
 // increase brightness                         0     - 255
 // decrease brightness and increase white      256   - 511

  x = map(tintInputValue, 0, inputMax, 0, 511);
  if (x<=255) {
    brightnessValue = x;
    whiteValue = 0;
  }else if (x>=256 && x<=511){
    brightnessValue = 511-x;
    whiteValue = x-256;
  }
  redValue = redValue*(neopix_gamma[brightnessValue]/255);
  greenValue = greenValue*(neopix_gamma[brightnessValue]/255);
  blueValue = blueValue*(neopix_gamma[brightnessValue]/255);
  
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

