/*
 * this code has been developed by Ayandeep Dutta
 */
#include <EEPROM.h>

#define rf 5
#define lf 6
#define rb 7
#define lb 8

boolean is_moving=false;
int b_level;

void setup() {
  //Serial.begin(115200);

  pinMode(12,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  
  pinMode(A1,INPUT);
  pinMode(rf,OUTPUT);
  pinMode(rb,OUTPUT);
  pinMode(lf,OUTPUT);
  pinMode(lb,OUTPUT);

  b_level=EEPROM.read(0)*4;
}

void set_level(){
  //notify user of initiation
  for (int i=0;i<3;i++){
    digitalWrite(13, 1);
    delay(500);
    digitalWrite(13, 0);
    delay(500);
  }
  int sb=0, sd=0, v=1;

  //set brightness value
  for (int i=0;i<10;i++){
    sb+=analogRead(A1);
    delay(100);
    digitalWrite(13, v);
    v=(v==1)?0:1;
  }
  sb=sb/40;
  

  for (int i=0;i<3;i++){
    digitalWrite(13, 1);
    delay(500);
    digitalWrite(13, 0);
    delay(500);
  }
  
  //set darkness value
  for (int i=0;i<10;i++){
    sd+=analogRead(A1);
    delay(100);
    digitalWrite(13, v);
    v=(v==1)?0:1;
  }
  sd=sd/40;
  
  int val=(sd+sb)/2;
  
  EEPROM.write(0, val);//write to byte 0 of eeprom
  b_level=val*4;
}

void go(){
  analogWrite(rf, 128);
  analogWrite(lf, 128);
  
  digitalWrite(rb, 0);
  digitalWrite(lb, 0);
  
  is_moving = true;
  
}

void halt(){
  digitalWrite(rb, HIGH);  
  digitalWrite(lb, HIGH);
  delay(50);

  digitalWrite(rb, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(lf, LOW);
  is_moving = false;
  
}

void loop() {
  
  if(digitalRead(12)==LOW){ halt();set_level(); }
  
  int val=analogRead(A1);
  
  if (val<b_level){//dark
    if(!is_moving){go();}
  }
  else{//bright
    if(is_moving){halt();}
  }
}
