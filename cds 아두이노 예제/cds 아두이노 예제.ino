/*
 * 2021-05-03 창업연계공학설계입문 과제
 * code by 국민대학교 전자공학부 202114**
 * cds센서의 값이 bright값보다 낮아지면 LED가 점등하는 예제
 */
#define bright 60 //기준 밝기 값 (이 값보다 낮으면 led 점등)
#define led 3 //led 연결된 핀
#define cds A0 //cds 센서가 연결된 핀

int read_cds= 0;

void setup() {
pinMode(led,OUTPUT); //led 핀 설정
Serial.begin(9600); //시리얼 보드레이트 속도 9600으로 설정
}

void loop() {
read_cds = analogRead(cds); //int 변수에 cds 센서 read값 저장
Serial.println(read_cds); //센서 값 시리얼 출력

if(read_cds<bright)digitalWrite(led,HIGH); //cds센서 값이 일정 값보다 낮으면(어두어지면) led 점등
else digitalWrite(led,LOW); //아닌경우(밝은경우) led 꺼짐

delay(100); 
}
