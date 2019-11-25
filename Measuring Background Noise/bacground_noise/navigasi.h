// for JoyStick
#define btn_x A6
#define btn_y A7
#define btn_ok 23
int vertikal, horizontal;
bool up, down, ok, kiri, kanan;
int point = 0, point_hzl = 0;


//--- NAVIGASI ---
void joyStick(int MAX_MENU, int MAX_hzl=0) {

  vertikal = analogRead(btn_x);
  horizontal = analogRead(btn_y);
  ok = digitalRead(btn_ok);

  if (ok == LOW) {Serial.print("LOW_ ok \t");}
  else if (ok == HIGH) {Serial.print("HIGH ok \t");}

  if (vertikal < 150) {
    point--; delay(200);
    if (point <= 0) {point = 0;}
  }
  else if (vertikal > 950) {
    point++; delay(200);
    if (point >= MAX_MENU) {point = MAX_MENU;}
  }

  if (horizontal < 150) {
    point_hzl++; delay(200);
    
    if (point_hzl >= MAX_hzl) {
      point_hzl = MAX_hzl;
      }
  }
  else if (horizontal > 950) {
    point_hzl--; delay(200);
    if (point_hzl <= 0) {point_hzl = 0;}
  }
  
  Serial.print("Premium Joy Kepake - ");  
        
  String out ="";
  out+=String(point)+"\t";
  out+=String(horizontal)+"\t";
  out+=String(point_hzl)+"\t";
  out+=String(MAX_hzl)+"\t";
  Serial.println(out);
}

int XjoyStick(int MAX, int sebelumnya) {
  int new_point = sebelumnya;
  
  vertikal = analogRead(btn_x);
  ok = digitalRead(btn_ok);

  if (vertikal < 150) {
    new_point --; delay(200);
    if (new_point  <= 0) {new_point = 0;}
  }
  else if (vertikal > 950) {
    new_point++; delay(200);
    if (new_point >= MAX) {new_point = MAX;}
  }
  Serial.println("XJoy Kepake");
  return new_point;
}
