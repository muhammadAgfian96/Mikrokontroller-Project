// SLAVE UNTUK
#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(2, 3); // RX, TX
int LED = 13; // the on-board LED
int Data; // the data received

void setup() {
  Bluetooth.begin(38400);
  Serial.begin(38400);
  Serial.println("Waiting for command...");
  Bluetooth.println("Send 1 to turn on the LED. Send 0 to turn Off");
  pinMode(LED,OUTPUT);

}

void loop() {
  if (Bluetooth.available()){ //wait for data received
    Data=Bluetooth.read();
    Serial.println(Data);
  }
  
    Bluetooth.write("Halo dari SLAVE");
delay(100);
}
