// untuk TDS SENSOR
#define TdsSensorPin A1
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, tdsValueSebelumnya = 0, temperature = 25;

// untuk SD CARD
#include <SPI.h>
#include <SD.h>
File myFile;
#define chip_select 4
String NAMA_BERKAS;

// untuk RTC (TIME)
#include <virtuabotixRTC.h>
const int CLK = 6, DAT = 7, RST = 8;
virtuabotixRTC myRTC(CLK, DAT, RST);
int tahun;
byte bulan, tanggal, jam, menit, detik;
String xData, xKolom = "tanngal;jam;ppm;ec;kontrol";
static unsigned long waktuTulis;

// buzzer
#define beep 2

//--for pid
float input, Sp, Kp, Ki, Kd;
float err, PID, lasterr, errSum, lasttime, a[4], e;
int gain;

//--servo
#include <Servo.h>
Servo servo_nutrisi;
Servo servo_air;
int pos = 0;

#define servoNutrisi 10 // 40-90
#define servoAir 3 // 80-130

String Data;

//############## START SETUP AWAL ###################
void setup() {
  Serial.begin(115200);

  //-- servo
  servo_nutrisi.attach(servoNutrisi);
  servo_air.attach(servoAir);

  //-----setting pid-----
  Sp = 800.0; // set point
  Kp = 1.0; // kp
  Ki = 0.0; // ki
  Kd = 0.0; // kd


  //---- setting buzzer ----
  pinMode(beep, OUTPUT);
  digitalWrite(beep, LOW);


  //----- Setup Awal Sensor -----
  pinMode(TdsSensorPin, INPUT);

  //----- Setting Awal Waktu -----
  tahun = 2019; bulan = 10; tanggal = 10;
  jam = 17; menit = 57 ; detik = 0;
  myRTC.setDS1302Time(detik, menit, jam, 6, tanggal, bulan, tahun);


  //----- Setting Awal SD CARD -----
  NAMA_BERKAS = "nuys.txt";
  Serial.print("Mengecek SD CARD: ");
  if (!SD.begin(4)) {
    Serial.println("GAGAL!"); return;
  }
  Serial.println("BISA!");
  //read_data();

  xKolom = "tanngal;jam;ppm;ec;kontrol_pid";
  write_data(xKolom);
  delay(3000);
}


//############## MAIN PROGRAM ###################

void loop() {
  // memanggil fungsi untuk update data waktu
  sensor_tds();
//  jalakan_kontrol(aktual, Setppint, val Kp, val Ki, val Kd);
  jalakan_kontrol(tdsValue, Sp, Kp, Ki, Kd);
  get_time();
  
  // tulis data jika berubah
  if (tdsValueSebelumnya != tdsValue) {
    tdsValueSebelumnya = tdsValue;
    write_data(xData);
  }
  
  delay(200);
}

//############## FUNGSI PENDUKUNG ###################
//----- untuk waktu ----
void get_time() {
  myRTC.updateTime();
  xData = "";
  xData += String(myRTC.dayofmonth) + "/";
  xData += String(myRTC.month) + "/";
  xData += String(myRTC.year) + ";";
  xData += String(myRTC.hours) + ":";
  xData += String(myRTC.minutes) + ":";
  xData += String(myRTC.seconds);
  //data dari sensor
  xData += ";" + String(tdsValue);
  xData += ";" + String(2 * tdsValue);
  //data dari kontrol
  xData += ";" + String(PID);

}

//----- untuk sd card ----
void write_data(String data_mu) {

  myFile = SD.open(NAMA_BERKAS, FILE_WRITE);
  if (myFile) {
    Serial.print(">>> Writing to " + NAMA_BERKAS + " = " + data_mu);
    myFile.println(data_mu);
    myFile.close();
    Serial.println(" [DONE]");
  } else {
    Serial.println("!!! GAK bisa buka " + NAMA_BERKAS);
  }
}

void read_data() {
  myFile = SD.open(NAMA_BERKAS);
  if (myFile) {
    Serial.println("Reading to " + NAMA_BERKAS + ":");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
    Serial.println("Reading to " + NAMA_BERKAS + " is [DONE]");

  } else {
    Serial.println("GAK bisa buka " + NAMA_BERKAS);
  }
}

//----- untuk sensor ----
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}

void sensor_tds() {
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U)
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U)
  {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value
    tdsValue = (2.0702 * tdsValue) - 255.91; //hasil persamaan kalibrasi
    //Serial.print("voltage:");
    //Serial.print(averageVoltage,2);
    //Serial.print("V   ");
    Serial.print("# TDS Value:");
    Serial.print(tdsValue, 0);
    Serial.println("ppm");
  }
}


//### FUNGSI PID ####
void jalakan_kontrol(float aktual, float Sp, float Kp, float Ki, float Kd) {

  float timeChange = (millis() - lasttime) / 1000;

  //Error 800 - aktual
  // E + maka dia kurang nutrisi => tambah NUTRISI
  // E - maka dia kelebihan nutrisi => tambah AIR

  err = Sp - aktual; //set point - nilai sensor aktual
  errSum = (err * timeChange);
  float dErr = (err - lasterr) / timeChange;

  // rumus pid nya
  PID = ((Kp * err) + (Ki * errSum) + (Kd * dErr));

  //----- Nilai PID di oper ke PWM servo---
  // E + maka dia kurang nutrisi => tambah NUTRISI  //servoNutrisi 10 // 40-90
  // E - maka dia kelebihan nutrisi => tambah AIR  //servoAir 3 // 80-130


  if (err < 0) {
    PID = PID * -1;
    if (PID > 90) {
      PID = 90; // E- -> AIR+
    }
    else if (PID < 40) {
      PID = 40;
    }
    jalankan_servo_air(PID+40);
  }
  else {
    if (PID > 90) {
      PID = 90; // E+ -> NUTRISI+
    }
    else if (PID < 40) {
      PID = 40;
    }
    jalankan_servo_nutrisi(PID);
  }

  lasterr = err;
  lasttime = millis();

}

void jalankan_servo_nutrisi(float nilai_pid) {
  int val_pid = int(nilai_pid);
  servo_nutrisi.write(val_pid);
  delay(15);
}

void jalankan_servo_air(float nilai_pid) {
  int val_pid = int(nilai_pid);
  servo_air.write(val_pid);
  delay(15);
}

