#include <FastLED.h>

#define num_leds 110
#define pin 4
#define brightness 255     // global brightness - can set 0-255

int effect = 0; // light effect mode
int param = 10; // strenght of effect 0->10
int lastpot = 0;

CRGB leds[num_leds];

void setup() {
  pinMode(A0, INPUT); 
  Serial.begin(115200);
  FastLED.addLeds<WS2811, pin, GRB>(leds, num_leds);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,2400);
  FastLED.setBrightness(brightness);
}

  
// a colored dot sweeping back and forth, with fading trails
void sinelon(int bpm) {
  if (bpm < 1) {
    bpm = 1;
  }
  fadeToBlackBy( leds, num_leds, 20);
  int pos = beatsin16(bpm, 0, num_leds - 1);
  leds[pos] = CRGB(255, 255, 255);
  delay(12);
} 


// the whole strip pulses bright and dim
void flareup(int bpm) {
  if (bpm < 1) {
    bpm = 1;
  }
  bpm *=2; // so 2-20 BPM
  
  int bright = beatsin16(bpm, 0, 64);

  for (int i = 0; i < num_leds - 1; i++){
    leds[i] = CRGB(bright, bright, bright);
  }
  delay(10);
}

void loop() {
  int pot = analogRead(A0);
  //Serial.println(pot);

  pot == max(1,pot); // needed?
  int lasteffect = effect;

  if (pot <= 512) {
    effect = 0;
    param = pot / 512.0 * 10.0;
  } else {
    effect = 1;
    param = (pot - 512.0) / 512.0 * 10.0;
  }
  
  if (effect != lasteffect) {
     FastLED.clear();
     
     lasteffect = effect;
  } else if (lastpot != pot) {

    lastpot = pot;
  }
  
  if (effect == 0) {
    flareup(param);
  } else {
    sinelon(param);  
  }

  FastLED.show();
}
