/*
  Modified for use with Camera Axe with written permission from Oliver Kraus.
  Maurice Ribble - 9/4/2010
  Modified 2016-03-29 for Arduino 1.6.8
  
  Now shared under Creative Commons Attribution 3.0 License
    (http://creativecommons.org/licenses/by-sa/3.0/)
*/   

#ifndef DogmCA_h
#define DogmCA_h

#include <stdint.h>
#include "utility/dogm128.h"
#include "Print.h"

class DogmCA : public Print {
    uint8_t a0Pin;
    uint8_t is_req_init;	// if call to Init() is required
    uint8_t size;
    PGM_P fptr;
    void Init(void);
  public:
    // text cursor position
    uint8_t tx, ty, charWidth, charHeight;
  
    DogmCA(uint8_t pin_a0); 
      
    void start(void);
    uint8_t next(void) { return dog_NextPage(); }
    
    void setPixel(uint8_t x, uint8_t y) { dog_SetPixel(x, y); }
    void clrPixel(uint8_t x, uint8_t y) { dog_ClrPixel(x, y); }

    /* y1 must be lower or equal to y2 */
    void setVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_SetVLine(x, y1, y2); }
    void clrVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_ClrVLine(x, y1, y2); }
    void xorVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_XorVLine(x, y1, y2); }

    /* x1 must be lower or equal to x2 */
    /* y1 must be lower or equal to y2 */
    void setBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_SetBox(x1, y1, x2, y2); }
    void clrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_ClrBox(x1, y1, x2, y2); }
    void xorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_XorBox(x1, y1, x2, y2); }

    /* Font */
    void setFont(PGM_P font, uint8_t w, uint8_t h) { fptr = font; charWidth = w; charHeight = h; }
    void setXY(uint8_t x, uint8_t y) { tx = x; ty = y; }
    void drawChar(uint8_t c) { tx += dog_DrawChar(tx, ty, fptr, c); }
    void drawStr(const char *s) { tx += dog_DrawStr(tx, ty, fptr, s); }
#if ARDUINO >= 100
    size_t write(uint8_t c) { tx += dog_DrawChar(tx, ty, fptr, c); return 1;} // Modified 2016-03-29 for Arduino 1.6.8
#else
    void write(uint8_t c) { tx += dog_DrawChar(tx, ty, fptr, c);}
#endif
};

#endif 
