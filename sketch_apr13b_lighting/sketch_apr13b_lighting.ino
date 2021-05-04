/*
 * 기숙사 전등 제어 
 * *로터리 엔코더로 전등 밝기 제어*
 * Atmega328P, MOSFET
 * 2021.4.13
 * 용예훈
 */

#define a 2
#define b 3
#define SW 4
int nowC;
int oldC;
int counter = 0;
unsigned long lastButtonPress = 0;    
boolean state =0;
void setup() {
  // put your setup code here, to run once:
pinMode(a,INPUT);
pinMode(b,INPUT_PULLUP);
pinMode(9,OUTPUT);
pinMode(SW, INPUT);     
oldC = digitalRead(a);
Serial.begin(9600);
}

void loop() {
  int btnState = digitalRead(SW);

  
  if (btnState == LOW) {
    
    if (millis() - lastButtonPress > 50) {  
      Serial.println("버튼 눌림!");  
      state = !state;
    }

  
    lastButtonPress = millis();
  }
  if(!state) {
analogWrite(9,0);
  }
  
  if(state) {
  nowC = digitalRead(a);

  if(nowC!=oldC&&nowC==1)
  {
    
    if(digitalRead(b)!=nowC){
      counter = counter -4;
    }
    else{
      counter = counter +4;
    }
    
    Serial.println(counter);
  }
  if(counter<0) counter=0;
      if(counter>255) counter=255;
      oldC = nowC;
      analogWrite(9,counter);
      
  }
  

}
