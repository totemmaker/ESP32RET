#pragma once

#include <Arduino.h>

#define WS2812B 0
#define GRB 0
#define CRGB Color
// #define TypicalLEDStrip

class FastLEDClass {
    CRGB *listPtr;
    int listCnt;
public:
    // template <uint8_t _type, uint8_t _pin, uint8_t _order>
    FastLEDClass& addLeds(CRGB *list, int cnt) {
        listPtr = list;
        listCnt = cnt;
        return *this;
    };
    void setBrightness(int brightness) { RGB.setDim(brightness); }
    void show() {
        RGB[0].color(listPtr[0]);
        RGB[1].color(listPtr[1]);
        RGB[2].color(listPtr[2]);
        RGB[3].color(listPtr[3]);
    }

    // void setCorrection( ) { }
};

extern FastLEDClass FastLED;
