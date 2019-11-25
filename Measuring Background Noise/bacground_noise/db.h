#include <SD.h>

String out = "";
String data[10]={};
#define ChipSelect 53

// data for set configuration
String FILE_TYPE = "type.txt";
String FILE_VIN = "vin.txt";
String FILE_ENGINE = "eng.txt";
String FILE_QIC = "qic.txt";
String FILE_PIC = "pic.txt";

void init_SD_CARD(){
    // Inisialisasi SD CARD
  Serial.print("Initializing SD card...");
  if (!SD.begin(ChipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

}


//  myFile = SD.open("type.txt", FILE_WRITE);

/*
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
*/
// re-open the file for reading:



void baca_db_conf(String FILE_NAME) {
//  open file
  int i = 0;
  File myFile;
  myFile = SD.open(FILE_NAME);
  
  int last_arr;
  
  if (myFile) {
//    jika kebaca maka, ambil isinya
    Serial.println(FILE_NAME);
    int size_arr = 0;
    char ch = "";
    
    while (myFile.available()) {
      ch = (myFile.read());

      if (ch == '\n' || ch == ',') {
        Serial.print(i);
        Serial.print("\t");
        Serial.println(out);
        data[i] = out;
        i++;
        out = "";
        if (i==10){
          return;
        }
      } else {
        out += ch;
      }
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file.txt");
  }

    Serial.println("loop baca void baca_db");

//  Serial.println(sizeof(data));
//  for (int i = 0; i < 20; i++) {
//    Serial.print(String(data[i]) + ",");
//  }

  Serial.println("keluar void baca_db");

}



