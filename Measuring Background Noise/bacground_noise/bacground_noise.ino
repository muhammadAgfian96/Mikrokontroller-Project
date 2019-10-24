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
#define PINK 0xF81F


int CENTER_X, CENTER_Y;
uint16_t g_identifier;
bool logic, state = true;
int menu;




#include "profile.h"
#include "widgets.h"
int i = 0;

void setup(void) {
  Serial.begin(9600);
  uint32_t when = millis();

  if (!Serial) delay(5000);           //allow some time for Leonardo
  Serial.println("Serial took " + String((millis() - when)) + "ms to start");

  static uint16_t identifier;

  g_identifier = tft.readID();
  Serial.print("ID = 0x");
  Serial.println(g_identifier, HEX);
  if (g_identifier == 0x00D3 || g_identifier == 0xD3D3) g_identifier = 0x9481; // write-only shield
  if (g_identifier == 0xFFFF) g_identifier = 0x9341; // serial

  tft.begin(g_identifier); //    g_identifier = 0x9488 / 0x9486;
  tft.setFont(&FreeSans9pt7b);

  int16_t width(void);
  tft.width();
  int16_t height(void);
  tft.height();

  tft.fillScreen(BLACK);
  tft.setRotation(0);
  menu = 0;
  logic = true;
  home_page();
  delay(2000);
}


void loop(void) {
  switch (menu) {
    //menu utama
    case 0:
      selected_border(320 / 4, (3 * 480 / 4), (320 / 2), 40, true);
      selected_border((320 / 4), (3 * 480 / 4) + 8 + 40, (320 / 2), 40);
      delay(1000);

      selected_border(320 / 4, (3 * 480 / 4), (320 / 2), 40, false);
      selected_border((320 / 4), (3 * 480 / 4) + 8 + 40, (320 / 2), 40, true);
      delay(1000);
      i++;
      if (i >= 3) {
        delay(200);
        menu = 1;
        tft.fillScreen(BLACK);
        record_data_page();
        i = 0;
      }
      break;


    //menu record data
    case 1:
      if (i >= 3) {
        delay(200);
        menu = 0;

        tft.fillScreen(BLACK);
        home_page();
        i = 0;
      }
      i++;
      delay(1000);
      break;
  }

}
//---- PAGE ----
void home_page() {
  //HOME Page--FIX!
  selected_border((320 / 4) - 25, 35, (320 / 2) + 50, 40, true);
  center_text("KITTO PROJECT", (320 / 4) - 25, 35, (320 / 2) + 50, 40);
  text("This is a Telemetry for", 68, tft.height() / 2, 1, WHITE);
  text("Sound dB Measurement", 60, (tft.height() / 2) + 20, 1, WHITE);
  button_text(320 / 4, (3 * 480 / 4), (320 / 2), 40, DARKGREEN, "START");
  button_text((320 / 4), (3 * 480 / 4) + 8 + 40, (320 / 2), 40, MAROON, "QUIT");
}

void record_data_page() {
  //judul Page
  center_text("RECORD DATA", (320 / 4) - 25, 35, (320 / 2) + 50, 40);
  selected_border((320 / 4) - 25, 35, (320 / 2) + 50, 40, true);

  center_text("WELKOM!", (320 / 4) - 25, 235, (320 / 2) + 50, 40);
}




