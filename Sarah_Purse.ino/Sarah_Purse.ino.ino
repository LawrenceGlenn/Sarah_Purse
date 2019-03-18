#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define NUM_LEDS 23

#define BRIGHTNESS 200

#define colorInputPin A0

#define tintInputPin A1

#define brightnessInputPin A2

#define inputMax 1023

#define wait 10

int colorInputValue = 0;
int tintInputValue = 0;
int brightnessInputValue = 0;
int brightnessValue = 0;

int colorValue[4];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

int startColorMatrix[NUM_LEDS][4];
int endColorMatrix[NUM_LEDS][4];
int currentColorMatrix[4];
float startMillis[NUM_LEDS];
float endMillis[NUM_LEDS];

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
  Serial.begin(9600);
  pinMode(colorInputPin, INPUT);
  pinMode(tintInputPin, INPUT);
  pinMode(brightnessInputPin, INPUT);
 colorValue[0]=125;
 colorValue[1]=125;
 colorValue[2]=125;
 colorValue[3]=125;
}

void loop() {
  // read the value from the input:
 // setColorWheelInput();
 // setTintWheelInput();
 // setBrightnessWheelInput();
//  loop through the pixels and set thier values
 //     for(uint16_t i=0; i<strip.numPixels(); i++) {
 //         strip.setPixelColor(i, strip.Color(200,0,100,20));
 //       }
 // strip.setBrightness(200);
 //       strip.show();
  
  for(int k=0; k<strip.numPixels();k++){
    shimmer(k);
    setCurrentColorMatrix(k);
    strip.setPixelColor(k, strip.Color(neopix_gamma[currentColorMatrix[0]],neopix_gamma[currentColorMatrix[1]],neopix_gamma[currentColorMatrix[2]],neopix_gamma[currentColorMatrix[3]]));
    
  } 
  strip.show();
        strip.setBrightness(200);
        delay(wait);


}

void setCurrentColorMatrix(int pixel){
  
    for(int j=0; j<4;j++){
        currentColorMatrix[j] = colorValueLimit(map(millis(), startMillis[pixel], endMillis[pixel], startColorMatrix[pixel][j], endColorMatrix[pixel][j]));
      
    }
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
  int x = map(colorInputValue, 0, inputMax, 0, 1535);
  if (x<=255) {
    colorValue[0] = 255;
    colorValue[1] = x;
    colorValue[2] = 0;
  }else if (x>=256 && x<=511){
    colorValue[0] = 511-x;
    colorValue[1] = 255;
    colorValue[2] = 0;
  }else if (x>=512 && x<=767){
    colorValue[0] = 0;
    colorValue[1] = 255;
    colorValue[2] = x-512;
  }else if (x>=768 && x<=1023){
    colorValue[0] = 0;
    colorValue[1] = 1023-x;
    colorValue[2] = 255;
  }else if (x>=1024 && x<=1279){
    colorValue[0] = x-1024;
    colorValue[1] = 0;
    colorValue[2] = 255;
  }else if (x>=1280 && x<=1536){
    colorValue[0] = 255;
    colorValue[1] = 0;
    colorValue[2] = 1536-x;
  }
  
}

//take input from tiny wheel and set brightness and white values
void setTintWheelInput(){
  tintInputValue = analogRead(tintInputPin);

  colorValue[3] = map(tintInputValue, 0, inputMax, 0, 255);
  
}

void setBrightnessWheelInput(){
  brightnessInputValue = analogRead(brightnessInputPin);
  
  brightnessValue = map(brightnessInputValue, 0, inputMax, 0, 255);
}


void flicker(int pixel){
    if(millis()>endMillis[pixel]){
      for(int j=0; j<4;j++){
        startColorMatrix[pixel][j] = endColorMatrix[pixel][j];
        endColorMatrix[pixel][j] = randomColorValue(colorValue[j],80,80);
      }
      startMillis[pixel] = millis();
      endMillis[pixel] = startMillis[pixel]+5000;
    }
  
}

void shimmer(int pixel){
  
    if(millis()>endMillis[pixel]){
      for(int j=0; j<3;j++){
        startColorMatrix[pixel][j] = endColorMatrix[pixel][j];
        if(endColorMatrix[pixel][j] >= colorValue[j]){
          endColorMatrix[pixel][j] = colorValue[j]-50;
        }else{
          endColorMatrix[pixel][j] = colorValue[j]+50;
        }
      }
      startColorMatrix[pixel][3] = endColorMatrix[pixel][3];
      if(endColorMatrix[pixel][3] >= colorValue[3]){
          endColorMatrix[pixel][3] = colorValue[3]-30;
        }else{
          endColorMatrix[pixel][3] = colorValue[3]+80;
        }
      startMillis[pixel] = millis();
      endMillis[pixel] = startMillis[pixel]+random(2000,8000);
    }
}

int colorValueLimit(int color){
  if(color < 0){
    color = 0;
  }else if(color>255){
    color = 255;
  }
  return color;
}


int randomColorValue(int startValue, int decrease, int increase){
  int temp = startValue-decrease+random(decrease+increase);
  return colorValueLimit(temp);
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
