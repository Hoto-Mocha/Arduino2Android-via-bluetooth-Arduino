#include <SoftwareSerial.h>

const int FSR_PIN = A0;
const float VCC = 5;
const float R_M = 10000;

#define BT_RXD 8
#define BT_TXD 7
SoftwareSerial BTSerial(BT_RXD, BT_TXD);  // 블루투스 모듈로 통신한 상대 기기 저장

#define LED_B 10
#define LED_G 11
#define LED_R 12

void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FSR_PIN, INPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  digitalWrite(LED_R, HIGH);
}

void loop() 
{
  if (BTSerial.available()) {
    String message = BTSerial.readStringUntil('\n');
    if (message.equals("up")) {
      digitalWrite(LED_B, HIGH);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_R, LOW);
    } 
    if (message.equals("down")) {
      digitalWrite(LED_B, LOW);
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_R, LOW);
    } 
    if (message.equals("stop")) {
      digitalWrite(LED_B, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_R, HIGH);
    }
    Serial.println(message);
  }
  int fsrADC = analogRead(FSR_PIN);
  if (fsrADC != 0) {
    float V_Out = VCC*(fsrADC/(float)1023); //아날로그 신호를 출력 전압으로 변경
    float R_FSR = ((R_M*VCC)/V_Out) - R_M; //센서의 저항 구함
    // float weight = pow(R_FSR/155852.5199, (1/(-0.7028))); //저항-무게 관계식을 통해 무게를 구함
    float weight = pow(R_FSR/4429494.2076, (1/(-1.2313))); //저항-무게 관계식을 통해 무게를 구함

    Serial.println("Analog: " + String(fsrADC)); //아날로그 신호 출력
    Serial.println("V: " + String(V_Out, 4) + "    \t[V]"); //출력 전압 출력
    Serial.println("R: " + String(R_FSR, 4) + "    \t[Ω]"); //저항 출력
    Serial.println("W: " + String(weight, 4) + "    \t[g]\n"); //무게 출력

    BTSerial.println(weight);                // 무게 값을 안드로이드 앱으로 전송

    delay(1000);
  } else {
    //do nothing
  }
}