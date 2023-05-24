#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

/********Modes*******/
#define NUM_BASIC_MODES 4
#define BLAST_MODE 999
#define OFF_MODE 998
#define PEWPEW_MODE 997
#define CHANGE_MODE 996
#define SOLID_MODE 995
#define TWINKLE_MODE 994
#define SHIMMER_MODE 993
int mode = OFF_MODE;
/********************/


#define NUM_LEDS 299
#define LAST_LED 298
#define MODE_LED 294
#define LED_PIN 5
#define BUTTON_PIN 3

CRGB leds[NUM_LEDS] = {0};

uint8_t hue = 0;
uint8_t curHue = 0;

bool altSOS = true;
int spinLocation = 0;
bool shimmerOn = true;

unsigned long prevMillis = 0;
unsigned long startMillis = 0;
unsigned long fadeMillis = 0;

uint8_t currState = LOW;
uint8_t lastState = LOW;
uint8_t cycle = 0;

int rings[] = {25, 23, 23, 21, 20, 19, 19, 18, 19, 17, 17, 16, 15, 12, 12, 14, 9};
int breakpoints[18]= {0};

int ringsLength = 17;

void changeMode() {
  if(cycle == 1) {
    mode = OFF_MODE;
  } else if(cycle >= 7) {
    mode = 0;
  } else if (cycle == 4) {
    mode = PEWPEW_MODE;
    FastLED.clear();
  } else if (cycle == 5) {
    mode = SOLID_MODE;
    FastLED.clear();
  } else if (cycle == 2) {
    FastLED.setBrightness(255);
    mode = BLAST_MODE;
    FastLED.clear();
  } else if (cycle == 6) {
    mode = TWINKLE_MODE;
    FastLED.clear();
  } else if (cycle == 3){
    mode = SHIMMER_MODE;
    FastLED.clear();
  }
  lastState = LOW;
  cycle = 0;
}

void holdChange() {
  if(millis() >= startMillis + 500) {
    FastLED.setBrightness(50);
    mode = CHANGE_MODE;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  for(int i = 0; i < ringsLength; i++) {
    breakpoints[i+1] = breakpoints[i] + rings[i];
  }

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);           

  FastLED.setBrightness(50);
  FastLED.clear();
  FastLED.setMaxPowerInMilliWatts(13000);
}

void loop() {
  //Button
  currState = digitalRead(BUTTON_PIN);
  if(mode < 500) {
    if (currState == HIGH && lastState == LOW) {
      startMillis = millis();
      lastState = HIGH;
    } else if (currState == LOW && lastState == HIGH) {
      FastLED.clear();
      mode = (mode + 1) % NUM_BASIC_MODES;
      lastState = LOW;
    } 
    if(currState == HIGH && lastState == HIGH) {
      holdChange();
    }
  } 
  
  else if(mode == CHANGE_MODE) {
    if (currState == HIGH && lastState == LOW) {
      startMillis = millis();
      lastState = HIGH;
    } else if (currState == LOW && lastState == HIGH) {
      changeMode(); 
    }
  } 
  //TODO: Change to marquee effect
  else if(mode == PEWPEW_MODE) {
    if (currState == HIGH && lastState == LOW) {
      startMillis = millis();
      uint8_t color = random8();
      leds[298] = CHSV(color, 255, 50);
      leds[297] = CHSV(color, 255, 100);
      leds[296] = CHSV(color, 255, 150);
      leds[295] = CHSV(color, 255, 200);
      leds[294] = CHSV(color, 255, 255);
      leds[293] = CHSV(color, 255, 200);
      leds[292] = CHSV(color, 255, 150);
      leds[291] = CHSV(color, 255, 100);
      leds[290] = CHSV(color, 255, 50);
      lastState = HIGH;
    } else if (currState == LOW && lastState == HIGH) {
      changeMode();  
      } else if(currState == HIGH && lastState == HIGH) {
        holdChange();
    }
  } 
  
  else if(mode == OFF_MODE) {
    if (currState == HIGH && lastState == LOW) {
      startMillis = millis();
      lastState = HIGH;
    } else if (currState == LOW && lastState == HIGH) {
      changeMode();  
    } else if(currState == HIGH && lastState == HIGH) {
      holdChange();
    }
  } 
  
  else if(mode == SOLID_MODE) {
    if (currState == HIGH && lastState == LOW) {
      startMillis = millis();
      lastState = HIGH;
    } else if (currState == LOW && lastState == HIGH) {
      uint8_t color = random8();
      for(uint8_t i = 0; i < rings[0]; i++) {
        leds[i] = CHSV(color, 255, 255);
      }
      changeMode();  
    } else if(currState == HIGH && lastState == HIGH) {
      holdChange();
    }
  }

  else if(mode == BLAST_MODE) {
    if (currState == HIGH && lastState == LOW) {
      startMillis = millis();
      lastState = HIGH;
    } else if (currState == LOW && lastState == HIGH) {
      uint8_t color = random8();
      for(uint8_t i = 0; i < rings[0]; i++) {
        leds[i] = CHSV(color, 255, 255);
      }
      changeMode();  
    } else if(currState == HIGH && lastState == HIGH) {
      holdChange();
    }
  }

  else if(mode == TWINKLE_MODE) {
    if (currState == HIGH && lastState == LOW) {
      startMillis = millis();
      lastState = HIGH;
    } else if (currState == LOW && lastState == HIGH) {
      curHue = random(100) > 70 ? 0 : random8();
      changeMode();  
    } else if(currState == HIGH && lastState == HIGH) {
      holdChange();
    }
  }

  else if(mode == SHIMMER_MODE) {
    if (currState == HIGH && lastState == LOW) {
      startMillis = millis();
      lastState = HIGH;
    } else if (currState == LOW && lastState == HIGH) {
      shimmerOn = !shimmerOn;
      changeMode();  
    } else if(currState == HIGH && lastState == HIGH) {
      holdChange();
    }
  }

  //Modes
  if(mode == OFF_MODE) {
    FastLED.clear();
  } else if(mode == CHANGE_MODE) {
    FastLED.clear();
    for(int i = 0; i < cycle; i++) {
      leds[MODE_LED-i] = CHSV(random8(), 255, 255);
    }
    if(millis() >= startMillis + 350) {
      cycle++;
      startMillis = millis();
    }
  } else if(mode == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue+i, 255, 255);
    }
  } else if (mode == 1) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue, 255, 255);
    }
  } else if (mode == 2) {
    int sofar = 0;
    CRGB ringColor = CRGB::Red;
    bool change = true;
    for(int i = 0; i <ringsLength; i++) {
      for(int j = 0; j <rings[i]; j++) {
        leds[sofar+j] = ringColor;
      }
      sofar += rings[i];
      ringColor = change ? CRGB::Blue: CRGB::Red;
      change = !change;
    }
  } else if (mode == 3) {
    if(millis() >= prevMillis + 500) {
      bool change = altSOS;
      for (int i = 0; i <ringsLength; i++) {
        if(change) {
          for(int j = breakpoints[i]; j < breakpoints[i+1]; j++) {
            leds[j] = CRGB::Red;
          }
        } else {
          for(int j = breakpoints[i]; j < breakpoints[i+1]; j++) {
            leds[j] = CRGB::White;
          }
        }
        change = !change;
      }
      altSOS = !altSOS;
      prevMillis = millis();
    }
  }
  // } else if(mode == 4) {
  //   for(int i = 0; i < ringsLength; i++) { 
  //     unsigned long time = millis();
  //     int reps = 0;
  //     uint8_t limit = 20;
  //     int mini = 200;
  //     int curLed = breakpoints[i]+(spinLocation % rings[i]);
  //     leds[curLed] = CRGB::Blue;
  //     leds[curLed].maximizeBrightness(5);
  //     FastLED.show();
  //     while(reps < 10) {
  //       if(millis() >= time + 10) {
  //         leds[curLed].maximizeBrightness(limit);
  //         leds[curLed-1].maximizeBrightness(mini);
  //         mini -= mini;
  //         limit += 20;
  //         time = millis();
  //         reps += 1;
  //         FastLED.show();
  //       }
  //     }
  //   }
  //   if(millis() >= prevMillis + 200) {
  //     spinLocation++;
  //     fadeToBlackBy(leds, NUM_LEDS, 50);
  //     prevMillis = millis();
  //   }
  // }


  //Effects
  if(mode == PEWPEW_MODE) {
    if(millis() >= prevMillis + 5) {
      for(int i = 0; i < NUM_LEDS-1; i++) {
        leds[i] = leds[i+1];
        leds[i+1] = CRGB::Black;
      }
      prevMillis = millis();
    }
  } else if(mode == 0 || mode == 1) {
    if(millis() >= prevMillis + 10) {
      hue++;
      prevMillis = millis();
    }
  } else if(mode == SOLID_MODE) {
    if(millis() >= prevMillis + 10) {
      for(int i = 0; i < rings[ringsLength-1]; i++) {
        leds[LAST_LED-i] = CRGB::Black;
      }
      int samplePos = LAST_LED-rings[ringsLength-1];
      for(int i = ringsLength-1; i > 0; i--) {
        CRGB samplePixel = leds[samplePos];
        for(int j = 0; j < rings[i]; j++) {
          leds[samplePos+1+j] = samplePixel;
        }
        samplePos -= rings[i-1];
        samplePixel = leds[samplePos];
      }
      prevMillis = millis();
    }
  } else if(mode == BLAST_MODE) {
    if(millis() >= prevMillis + 15) {
      for(int i = 0; i < rings[ringsLength-1]; i++) {
        leds[LAST_LED-i] = CRGB::Black;
      }
      int samplePos = LAST_LED-rings[ringsLength-1];
      for(int i = ringsLength-1; i > 0; i--) {
        CRGB samplePixel = leds[samplePos];
        for(int j = 0; j < rings[i]; j++) {
          leds[samplePos+1+j] = samplePixel;
        }
        samplePos -= rings[i-1];
        for(int j = 0; j < rings[i-1]; j++) { 
          leds[samplePos+1+j] = CRGB::Black;
        }
      }
      prevMillis = millis();
    }
  } else if(mode == TWINKLE_MODE) {
    if(millis() >= prevMillis + 10) {
      if(curHue == 0) {
        leds[random16(NUM_LEDS)] = CHSV(random8(), 255, 255);
      } else {
        leds[random16(NUM_LEDS)] = CHSV(curHue, 255, 255);
      }
    }
    if(millis() >= fadeMillis + 5) {
      fadeToBlackBy(leds, NUM_LEDS, 5);
      fadeMillis = millis();
    }
  } else if(mode == SHIMMER_MODE) {
    if(shimmerOn) {
      if(millis() >= prevMillis + 1) {
        leds[random16(NUM_LEDS)] = CHSV(random8(), 255, 255);
        prevMillis = millis();
      }
      if(millis() >= fadeMillis + 15) {
        fadeToBlackBy(leds, NUM_LEDS, 5);
        fadeMillis = millis();
      }
    } else {
      FastLED.clear();
    }
  }


  FastLED.show();

}

