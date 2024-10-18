/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
 * Copyright (c) 2018 by Fabrice Weinberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ThingPulse invests considerable time and money to develop these open source libraries.
 * Please support us by buying our products (and not the clones) from
 * https://thingpulse.com
 *
 */

#ifndef SH1106ESP_h
#define SH1106ESP_h

#include "OLEDDisplay.h"
#include "variant.h"
#include <SPI.h>

class SH1106SpiESP : public OLEDDisplay {
  private:
      uint8_t             _rst;
      uint8_t             _dc;
      uint8_t             _cs;

  public:
    SPIClass *_SPI;
    /* pass _cs as -1 to indicate "do not use CS pin", for cases where it is hard wired low */
    SH1106SpiESP(uint8_t _rst, uint8_t _dc, uint8_t _cs, OLEDDISPLAY_GEOMETRY g = GEOMETRY_128_64) {
      setGeometry(g);

      this->_rst = _rst;
      this->_dc  = _dc;
      this->_cs  = _cs;
      // SPI3_HOST
      // OLED_SPI_HOST
      // this->_SPI = new SPIClass(SPI3_HOST); 
      LOG_INFO("SH1106SpiESP constructor with RST: %d, DC: %d, CS: %d", _rst, _dc, _cs);
    }

    bool connect(){
      pinMode(_dc, OUTPUT);
      if (_cs != (uint8_t) -1) {
        pinMode(_cs, OUTPUT);
      }  
      pinMode(_rst, OUTPUT);

    //   _SPI->begin ();
      _SPI->begin(OLED_SCLK, -1, OLED_MOSI, OLED_CS);
      _SPI->setClockDivider (SPI_CLOCK_DIV2);
      _SPI->setFrequency(1000000);

      // Pulse Reset low for 10ms
      digitalWrite(_rst, HIGH);
      delay(1);
      digitalWrite(_rst, LOW);
      delay(10);
      digitalWrite(_rst, HIGH);
      LOG_INFO("_SPI begin with SCLK: %d, MOSI: %d, CS: %d", OLED_SCLK, OLED_MOSI, OLED_CS);
      return true;
    }

    void display(void) {
    #ifdef OLEDDISPLAY_DOUBLE_BUFFER
       uint8_t minBoundY = UINT8_MAX;
       uint8_t maxBoundY = 0;

       uint8_t minBoundX = UINT8_MAX;
       uint8_t maxBoundX = 0;

       uint8_t x, y;

       // Calculate the Y bounding box of changes
       // and copy buffer[pos] to buffer_back[pos];
       for (y = 0; y < (displayHeight / 8); y++) {
         for (x = 0; x < displayWidth; x++) {
          uint16_t pos = x + y * displayWidth;
          if (buffer[pos] != buffer_back[pos]) {
            minBoundY = _min(minBoundY, y);
            maxBoundY = _max(maxBoundY, y);
            minBoundX = _min(minBoundX, x);
            maxBoundX = _max(maxBoundX, x);
          }
          buffer_back[pos] = buffer[pos];
        }
        yield();
       }

       // If the minBoundY wasn't updated
       // we can savely assume that buffer_back[pos] == buffer[pos]
       // holdes true for all values of pos
       if (minBoundY == UINT8_MAX) return;

       // Calculate the colum offset
       uint8_t minBoundXp2H = (minBoundX + 2) & 0x0F;
       uint8_t minBoundXp2L = 0x10 | ((minBoundX + 2) >> 4 );

       for (y = minBoundY; y <= maxBoundY; y++) {
         sendCommand(0xB0 + y);
         sendCommand(minBoundXp2H);
         sendCommand(minBoundXp2L);
         set_CS(HIGH);
         digitalWrite(_dc, HIGH);   // data mode
         set_CS(LOW);
         for (x = minBoundX; x <= maxBoundX; x++) {
           _SPI->transfer(buffer[x + y * displayWidth]);
         }
         set_CS(HIGH);
         yield();
       }
     #else
      for (uint8_t y=0; y<displayHeight/8; y++) {
        sendCommand(0xB0 + y);
        sendCommand(0x02);
        sendCommand(0x10);
        set_CS(HIGH);
        digitalWrite(_dc, HIGH);   // data mode
        set_CS(LOW);
        for( uint8_t x=0; x < displayWidth; x++) {
          _SPI->transfer(buffer[x + y * displayWidth]);
        }
        set_CS(HIGH);
        yield();
      }
     #endif
    }

  private:
	int getBufferOffset(void) {
		return 0;
	}
    inline void set_CS(bool level) {
      if (_cs != (uint8_t) -1) {
        digitalWrite(_cs, level);
      }
    };
    inline void sendCommand(uint8_t com) __attribute__((always_inline)){
      set_CS(HIGH);
      digitalWrite(_dc, LOW);
      set_CS(LOW);
      _SPI->transfer(com);
      set_CS(HIGH);
    }
};

#endif
