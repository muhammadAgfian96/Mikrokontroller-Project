//https://electropeak.com/learn/absolute-beginners-guide-to-tft-lcd-displays-by-arduino/
int CENTER_X, CENTER_Y;
uint16_t g_identifier;
bool logic, state = true;
int menu;

#include "profile.h"
#include "widgets.h"
#include "navigasi.h"
#include "db.h"

int i = 0;

void setup(void) {
  Serial.begin(9600);
  uint32_t when = millis();

  init_SD_CARD();

// IDK
  pinMode(btn_ok, INPUT);
  digitalWrite(btn_ok, HIGH);
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

void list_menu( int, String [], int ); 
void list_menu(int point, String myArr[], int sizeArray){
  // FIX LIST BISA FLEKSIBLE/REUSABLE
  int space=35, new_start_y = 185, offside_x=70, w =100, h=190;
  int begin_x = (320/4)+offside_x;
  
  int begin_y = 5+new_start_y + (space-10) * point;
  int begin_y_end = new_start_y-10 + space *point;
  int pl = 10; //pr = padding left

  // draw container for list
  tft.drawRoundRect(begin_x-2, begin_y-2, w+4, h+4, 5, RED);
  tft.fillRoundRect(begin_x, begin_y, w, h, 3, DARKGREY);
    
  int16_t ht = 18, top = 3, line, lines = 4, scroll=begin_y;
        Serial.println("1 masuk list menu for");

  // tulis semua list
  for (int line=0; line<sizeArray; line++){
            Serial.println(myArr[line]);
     text(myArr[line], begin_x+pl, begin_y+ht+(line*ht), 1, RED);
  }
        Serial.println(" masuk list menu for");

  //select the first
  tft.fillRoundRect(begin_x, begin_y+4+(0*ht), w, ht, 2, RED);
  text(myArr[0], begin_x+pl, begin_y+ht+(0*ht), 1, WHITE);

  delay(200);
  int new_point=0, old_point;
  boolean flag=true;

  // running the selection mode
  while(flag){
    old_point=new_point;
    Serial.println("new: "+ String(new_point));
    new_point = XjoyStick(9, new_point);
    
    
    if(new_point-old_point!=0){
     tft.fillRoundRect(begin_x,   begin_y+4+(old_point*ht), w, ht, 2, DARKGREY);
     text(myArr[old_point], begin_x+pl, begin_y+ht+(old_point*ht), 1, RED);
    // current selected
     tft.fillRoundRect(begin_x, begin_y+4+(new_point*ht), w, ht, 2, RED);
     text(myArr[new_point], begin_x+pl, begin_y+ht+(new_point*ht), 1, WHITE);
    }
    
     Serial.println("new: "+ String(new_point));
     delay(100);
     
     if (ok==LOW){
       flag=false;
       Serial.println("looping OK 200 "+ String(point));
       tft.fillRoundRect(begin_x-2, begin_y-2, w+4, h+4, 0, BLACK); // untuk ngebersihin kotak list menu
       record_data_page();
       tft.fillRoundRect(begin_x+5, begin_y_end-ht, w, 25, 1, RED); // hapus tulisan x y w h r c
       text(myArr[new_point], begin_x+10,begin_y_end, 1, YELLOW); // timpa 
       Serial.println(begin_y_end);
       Serial.println(myArr[new_point]);
       
    }
  }
}

void loop(void) {
  switch (menu) {
    //menu utama
    case 0:
          home_nav();

            // JIKA USER MEMILIH            
            if (point == 1 && ok == LOW){
              //exit
              Serial.println("\t PILIH EXIT");
              delay(250);
              }
              
            if (point == 0 && ok == LOW){
              //Start
              Serial.println("\t PILIH START");
              delay(250);
              point= 0;
              menu = 1;
              tft.fillScreen(BLACK);
              record_data_page();
              i = 0;
              }
      break;


    //menu record data
    case 1:
        record_data_nav(); // return point and point_hzl
        // JIKA USER MEMILIH
        if (point == 0 && ok == LOW){
        // Type
        int sizeArray=10;
        baca_db_conf(FILE_TYPE);
        Serial.println("siap masuk list menu");
        list_menu(point, data, sizeArray); // how passing array String to function
        delay(250);
        }      
        else if (point == 1 && ok == LOW){
          int sizeArray=10;
          baca_db_conf(FILE_VIN);
          list_menu(point, data, sizeArray); // how passing array String to function
          delay(250);
        }
        else if (point == 2 && ok == LOW){
          int sizeArray=10;
          baca_db_conf(FILE_ENGINE);
          list_menu(point, data, sizeArray); // how passing array String to function
          delay(250);
        }
        else if (point == 3 && ok == LOW){
          int sizeArray=10;
          baca_db_conf(FILE_QIC);
          list_menu(point, data, sizeArray); // how passing array String to function
          delay(250);
        }
        else if (point == 4 && ok == LOW){
          int sizeArray=10;
          baca_db_conf(FILE_PIC);
          list_menu(point, data, sizeArray); // how passing array String to function
          delay(250);
        }
        else if (point_hzl == 0 && point == 5 && ok == LOW){
          // OK
          point= 0;
          menu = 2;
          tft.fillScreen(BLACK);
          action_page();
          delay(250);
        }
        else if (point_hzl == 1 && point == 5 && ok == LOW){
          // RESET
          point= 0;
          menu = 1;
          tft.fillScreen(BLACK);
          record_data_page();
          delay(250);
        }
      break;


    case 2:
      action_nav();

            // JIKA USER MEMILIH            
            if (point == 0 && ok == LOW){
              //record
              delay(250);
              point= 0;
              menu = 3;
              tft.fillScreen(BLACK);
              record_now_page();
              i = 0;
              }
              
            if (point == 1 && ok == LOW){
              // part ref
              delay(250);
              point= 0;
              menu = 4;
              tft.fillScreen(BLACK);
//              part_ref_page();
              i = 0;
              }      

            if (point == 2 && ok == LOW){
              // back
              delay(250);
              point= 0;
              menu = 1;
              tft.fillScreen(BLACK);
              record_data_page();
              i = 0;
              }      
      break;

      case 3:
        // navigasi

        // start-> send data to bluetooth

        // receive data from bluetooth + RTC 

        // ok-save + MicroSD

        // cancel-reset the data
      
        break;

      case 4:
      // navigasi

      // show array
      
        break;

  
  }

}


//---- PAGE and NAVIGATION----

void record_now_page(){
  selected_border((320 / 4) - 25, 35, (320 / 2) + 50, 40, true);
  center_text("RECORDING", (320 / 4) - 25, 35, (320 / 2) + 50, 40);

  button_text(20, 100, (320 / 4), 40, DARKGREEN, "REC!");
  int ht=16, begin_y = 170;
  // tulis semua list
  for (int line=0; line<20; line++){
    
     if(line<10){
        text("data-"+String(line), 60, begin_y+ht+ (line*ht), 1, ORANGE);
     Serial.println("sisi kiri");
     }else{
        text("data-"+String(line), 170, begin_y+ht+ ((line-10)*ht), 1, RED);  
        Serial.println("sisi kanan");
    }
  
  }


  text("*Average: 223.5 dB", (320 / 4)-60, (3 * 480 / 4) + 5, 1, WHITE);
  text("*Size Data: 2424", (320 / 4)-60, (3 * 480 / 4) + 28, 1, WHITE);  
  
  button_text((320 / 4)-60, (3 * 480 / 4) + 8 + 40, (320 / 4)+50, 40, DARKGREEN, "SAVE");
  button_text( 2*(320 / 4), (3 * 480 / 4) + 8 + 40, (320 / 4)+50, 40, MAROON, "CANCEL");
  
}



void home_page() {
  //HOME Page--FIX!
  selected_border((320 / 4) - 25, 35, (320 / 2) + 50, 40, true);
  center_text("KITTO PROJECT", (320 / 4) - 25, 35, (320 / 2) + 50, 40);
  text("This is a Telemetry for", 68, tft.height() / 2, 1, WHITE);
  text("Sound dB Measurement", 60, (tft.height() / 2) + 20, 1, WHITE);
  button_text(320 / 4, (3 * 480 / 4), (320 / 2), 40, DARKGREEN, "START");
  button_text((320 / 4), (3 * 480 / 4) + 8 + 40, (320 / 2), 40, MAROON, "QUIT");
}

void home_nav(){
        joyStick(1);
            // JIKA USER HANYA NAVIGASI
            if (point == 0){
              Serial.println("Start Menu Cur");
              selected_border( 320 / 4,           (3 * 480 / 4),  (320 / 2), 40, true); // start
              selected_border((320 / 4), (3 * 480 / 4) + 8 + 40,  (320 / 2), 40, false); // exit
            }

            else if(point == 1){
              Serial.println("EXIT Menu Cur");
              selected_border(  320 / 4,          (3 * 480 / 4), (320 / 2), 40, false); //start
              selected_border((320 / 4), (3 * 480 / 4) + 8 + 40, (320 / 2), 40, true); // exit
            }
}

void record_data_page() {
  //judul Page
  center_text("SET CONF.", (320 / 4) - 25, 35, (320 / 2) + 50, 40);
  selected_border((320 / 4) - 25, 40, (320 / 2) + 50, 40, true);
  
  int space=35, start_y = 175, offside_x=-20, offside_val=80;
    
  text("Type____:", (320/4)+offside_x, start_y,         1, WHITE); // text("---", (320/4)+offside_val, start_y,         1, WHITE); // 0 Type
  text("Vin_____:", (320/4)+offside_x, start_y+space*1, 1, WHITE); // text("---", (320/4)+offside_val, start_y+space*1, 1, WHITE); // 1 Vin
  text("Engine__:", (320/4)+offside_x, start_y+space*2, 1, WHITE); // text("---", (320/4)+offside_val, start_y+space*2, 1, WHITE); // 2 Engine
  text("QIC/QIS_:", (320/4)+offside_x, start_y+space*3, 1, WHITE); // text("---", (320/4)+offside_val, start_y+space*3, 1, WHITE); // 3 QIC/QIS
  text("PIC_____:", (320/4)+offside_x, start_y+space*4, 1, WHITE); // text("---", (320/4)+offside_val, start_y+space*4, 1, WHITE); // 4 PIC
  
  //5a 5b Button Reset and OK
  button_text(   (320 / 4)-20, (3 * 480 / 4) + 8 + 40, (320 / 4), 40, DARKGREEN, "  OK ");
  button_text( 2*(320 / 4)+20, (3 * 480 / 4) + 8 + 40, (320 / 4), 40, MAROON, "RESET");
}

void record_data_nav(){
  joyStick(5,1);
            int space=35, start_y = 175, offside_x=-50;
            if(point == 0){
              text(">>",    (320/4)+offside_x, start_y, 1, RED); //0
              text(">>",    (320/4)+offside_x, start_y+space*1, 1, BLACK);//1
            }
            else if (point == 1){
              text(">>",    (320/4)+offside_x, start_y, 1, BLACK); //0
              text(">>",    (320/4)+offside_x, start_y+space*1, 1, RED); //1
              text(">>",    (320/4)+offside_x, start_y+space*2, 1, BLACK);//2
            }
            else if (point == 2){
              text(">>",    (320/4)+offside_x, start_y+space*1, 1, BLACK); //1
              text(">>",    (320/4)+offside_x, start_y+space*2, 1, RED); //2
              text(">>",    (320/4)+offside_x, start_y+space*3, 1, BLACK);//3
            }
            else if (point == 3){
              text(">>",    (320/4)+offside_x, start_y+space*2, 1, BLACK);//2
              text(">>",    (320/4)+offside_x, start_y+space*3, 1, RED); //3
              text(">>",    (320/4)+offside_x, start_y+space*4, 1, BLACK);//4
            }
            else if ((point == 4 && point_hzl==1) || (point == 4 && point_hzl==0)){
              text(">>",    (320/4)+offside_x, start_y+space*3, 1, BLACK);//3
              text(">>",    (320/4)+offside_x, start_y+space*4, 1, RED);//4
              
              selected_border((320 / 4)-20, (3 * 480 / 4)+ 8 + 40, (320 / 4), 40); // ok
              selected_border(2*(320 / 4)+20, (3 * 480 / 4)+ 8 + 40, (320 / 4), 40); // reset
            }
            else if (point == 5 && point_hzl==0){
              text(">>",    (320/4)+offside_x, start_y+space*4, 1, BLACK);//4
              Serial.println("Start 1 Cur");
              selected_border((320 / 4)-20, (3 * 480 / 4)+ 8 + 40, (320 / 4), 40, true); // ok
              selected_border(2*(320 / 4)+20, (3 * 480 / 4)+ 8 + 40, (320 / 4), 40); // reset
            }
            else if(point == 5 && point_hzl==1){
              text(">>",    (320/4)+offside_x, start_y+space*4, 1, BLACK);//4
              Serial.println("EXIT Menu Cur");
              selected_border((320 / 4)-20, (3 * 480 / 4)+ 8 + 40, (320 / 4), 40); // ok
              selected_border(2*(320 / 4)+20, (3 * 480 / 4)+ 8 + 40, (320 / 4), 40, true); // reset
            }
            else{
              text("ERR",    (320/4)+offside_x, start_y+space*-1, 1, RED);//3
            }
}

void action_page(){
    //HOME Page--FIX!
  selected_border((320 / 4) - 25, 35, (320 / 2) + 50, 40, true);
  center_text("RECORD NOW!", (320 / 4) - 25, 35, (320 / 2) + 50, 40);
  text("Bluetooth is already", 75, tft.height() / 4, 1, GREENYELLOW);
//  text("YOW! Connected", 60, (tft.height() / 2) + 20, 1, WHITE);

  button_text(320 / 4, (tft.height()/2)- 20 - 40, (320 / 2), 40, DARKGREEN, "RECORD");
  button_text(320 / 4, (tft.height()/2), (320 / 2), 40, ORANGE, "PART REFF");
  button_text(320 / 4, (tft.height()/2) + 20 + 40, (320 / 2), 40, MAROON, "BACK");
}

void action_nav(){
        joyStick(2);
            // JIKA USER HANYA NAVIGASI
            if (point == 0){
              Serial.println("Start Menu Cur");
              selected_border(320 / 4, (tft.height()/2)- 20 - 40, (320 / 2), 40, true); // record
              selected_border(320 / 4, (tft.height()/2), (320 / 2), 40); // part ref
              selected_border(320 / 4, (tft.height()/2)+ 20 + 40, (320 / 2), 40); // back
            }

            else if(point == 1){
              Serial.println("EXIT Menu Cur");
              selected_border(320 / 4, (tft.height()/2)- 20 - 40, (320 / 2), 40); // record
              selected_border(320 / 4, (tft.height()/2), (320 / 2), 40, true); // part ref
              selected_border(320 / 4, (tft.height()/2)+ 20 + 40, (320 / 2), 40); // back            
              }
            else if(point ==2){  
              selected_border(320 / 4, (tft.height()/2)- 20 - 40, (320 / 2), 40); // record
              selected_border(320 / 4, (tft.height()/2), (320 / 2), 40); // part ref
              selected_border(320 / 4, (tft.height()/2)+ 20 + 40, (320 / 2), 40, true); // back    
             }
}


