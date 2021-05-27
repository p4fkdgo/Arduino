/*
 * 국민대학교 창업연계공학설계입문 기말과제
 * 2021-05-27
 * 
 * 연결되어있는 기능은 다음과 같습니다:
  *1.LED 밝기 조절 가능 
  *-명령어-
  *led on : LED 킴 
  *led on : LED 꺼짐
  *led bright (0~255의 숫자) : 숫자로 led 밝기조절
 *2.cds 센서
 *3.초음파 센서를 이용해 거리측정
 *4.블루투스를 통해 led 제어와 cds, 거리 측정값을 출력함
 *
 */
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#define rs 4
#define bright 5
#define en 6
#define TRIG 7 
#define ECHO 8
#define led 9
#define d4 10
#define d5 11
#define d6 12
#define d7 13

#define notice_delay 1000 //센서 데이터를 출력하는 기간

int lcd_bright = 90;
unsigned long old_t = 0;
String message_buf = "";
String message = "";

SoftwareSerial mySerial(3, 2);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float cds_volt(){
  float volt = analogRead(A0)*0.0049; //(5/1024 = 0.0049)
  return volt;
}

float sona(){
  float distance = 0;
  
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    float duration = pulseIn (ECHO, HIGH);
    distance = duration * 17 / 1000; 
    old_t = millis();
  
  return distance;
}

void control(String m){
  m.replace(" ",""); //문자열 공백 제거
  
  //led 명령인 경우
  if(m.indexOf("led")!=-1&&message.indexOf("csy")==-1){  
    if(m.indexOf("on")!=-1) {
      analogWrite(led,255);
      mySerial.println("control message : LED 켜짐");
    }
    else if(m.indexOf("off")!=-1) {
      analogWrite(led,0);
      mySerial.println("control message : led 꺼짐");
    }
    else if(m.indexOf("bright")!=-1) {
      String br = m.substring(m.indexOf("bright")+6,m.length());
      int b = br.toInt();
      analogWrite(led,b);
      mySerial.print("control message : led 밝기 : ");mySerial.println(b);
    }
  }
  
  //lcd 명령인 경우
  else if(m.indexOf("led")==-1&&message.indexOf("csy")!=-1){ 
    if(m.indexOf("bright")!=-1) {
      String br = m.substring(m.indexOf("bright")+6,m.length());
      lcd_bright = br.toInt();
      analogWrite(bright,lcd_bright);
      mySerial.print("control message : lcd 밝기 : ");mySerial.println(lcd_bright);
    }
    }
  
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(bright,OUTPUT);
  pinMode(led,OUTPUT);
  Serial.begin(57600);
  mySerial.begin(9600);
  
  analogWrite(bright,90);
  lcd.begin(16, 2);
}

void loop() {
  //문자열 수신시 string에 문자열 저장
  if (mySerial.available()) {
    //message = mySerial.readStringUntil('\n');

    char c = mySerial.read();
    if(c!=0b00001010)message_buf = message_buf+(String)c; //0b00001010 == \n
    else { message = message_buf; message_buf="";}
  }
  
  //문자열 입력이 있는 경우 명령어 확인 및 문자열 lcd 표시
  else{
    if(message.length()>0){
      if(message.indexOf("led")==-1&&message.indexOf("csy")==-1){
        for(int i=0;i<16;i++){ 
          lcd.setCursor(i, 0);
          lcd.print(" ");
        }
      lcd.setCursor(0, 0);
      lcd.print(message);
      Serial.println(message);
      } else if(message.indexOf("led")!=-1||message.indexOf("csy")!=-1) control(message);
      
    message="";
    }
    
//문자열 입력이 없는 경우 센서 데이터 송신 작업 수행
    if(millis()-old_t>notice_delay){
      String output = "cds 센서 전압값 : "+(String)cds_volt()+"V   ||  "+"초음파 센서 : "+(String)sona()+"CM";
      mySerial.println(output);
      lcd.setCursor(0, 1);
      lcd.print((String)cds_volt()+"V"+"  "+(String)sona()+"CM     ");
      old_t = millis();
    }
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
