#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
MCUFRIEND_kbv tft;

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <FreeDefaultFonts.h>

//--- INTERACTION ---

void move_cursor() {

}

//---- WIDGET-----

//show text
void text(String msg, int x, int y, int sz, int color){
  const GFXfont *f = &FreeSans9pt7b;
  
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(sz);
  tft.print(msg);
}

void center_text(String msg, int x, int y, int w, int h) {
  int pad_x = msg.length();
  pad_x = (w - (pad_x * 11)) / 2;
  pad_x = floor(pad_x);
  text(msg, x+pad_x, y+(3*h/4)-5, 1, WHITE);
}

void selected_border( int x, int y, int w, int h , bool sel = false) {
  int color_sel;
  
  if (sel == true) {
    color_sel = WHITE;
  } else {
    color_sel = BLACK;
  }
  
  tft.drawRoundRect(x-2, y-2, w+4, h+3, 18, color_sel);
}

void button_text(int x, int y, int w, int h, int color, String msg) {
  // pos =(x, y), dimensi = (w, h), r = 20, GREEN);
  tft.fillRoundRect( x, y, w, h, 15, color);
  center_text(msg, x, y, w, h);
}





