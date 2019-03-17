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

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  pinMode(colorInputPin, INPUT);
  pinMode(tintInputPin, INPUT);
  pinMode(brightnessInputPin, INPUT);
}

void loop() {
  // read the value from the input:
 // setColorWheelInput();
 // setTintWheelInput();
 // setBrightnessWheelInput();
 colorValue[0]=0;
 colorValue[1]=10;
 colorValue[2]=50;
 colorValue[3]=200;
shimmer();
//  loop through the pixels and set thier values
 //     for(uint16_t i=0; i<strip.numPixels(); i++) {
 //         strip.setPixelColor(i, strip.Color(200,0,100,20));
 //       }
 // strip.setBrightness(200);
 //       strip.show();
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


void shimmer(){
  for(int k=0; k<strip.numPixels();k++){
    if(millis()>endMillis[k]){
      for(int j=0; j<4;j++){
        startColorMatrix[k][j] = endColorMatrix[k][j];
        endColorMatrix[k][j] = randomColorValue(colorValue[j],80,80);
      }
      startMillis[k] = millis();
      endMillis[k] = startMillis[k]+5000;
    }

    for(int j=0; j<4;j++){
      if(currentColorMatrix[j] != endColorMatrix[k][j]){
        currentColorMatrix[j] = map(millis(), startMillis[k], endMillis[k], startColorMatrix[k][j], endColorMatrix[k][j]);
      }
    }
    strip.setPixelColor(k, strip.Color(currentColorMatrix[0],currentColorMatrix[1],currentColorMatrix[2],currentColorMatrix[3]));
  }
  
        strip.setBrightness(200);
        strip.show();
  
}


int randomColorValue(int startValue, int decrease, int increase){
  int temp = startValue-decrease+random(decrease+increase);
  if(temp < 0){
    temp = 0;
  }else if(temp>255){
    temp = 255;
  }
  return temp;
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
