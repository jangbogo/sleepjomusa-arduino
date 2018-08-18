 #include <SoftwareSerial.h> 
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial BTSerial(6, 7);

byte buffer[256]; // 데이터 수신 버퍼
int bufferPosition; // 버퍼에 기록할 위치

int redLed1 = 11;  
int blueLed1 = 9;
int greenLed1 = 10;
// 각각의 LED와 연결된 핀번호

boolean redState = 0;
boolean blueState = 0;
boolean greenState = 0;
// 각 부품을 제어하기 위한 상태 값(이 값을 조절하는 걸로 부품을 제어한다.)

int redLight = 0;
int greenLight = 0;
int blueLight = 0;
// 각 색상의 값을 저장 한다.

void setup () {
  pinMode(redLed1, OUTPUT);
  pinMode(blueLed1, OUTPUT);
  pinMode(greenLed1, OUTPUT);

  BTSerial.begin(9600);
  Serial.begin(9600);
  bufferPosition = 0;
}

void loop () {
  if (BTSerial.available()) {
    byte data = BTSerial.read();
    buffer[bufferPosition++] = data; 
    Serial.println(data);

    if (data == 'q') { // Red 조절
      if (bufferPosition == 4) {
        redLight = (buffer[0]-48)*100 + (buffer[1]-48)*10 + (buffer[2]-48);
        lightOn();
        redState = 1;
      } 
      else if (bufferPosition == 3) {
        redLight = (buffer[0]-48)*10 + (buffer[1]-48);
        lightOn();
        redState = 1;
      } 
      else if (bufferPosition == 2) {
        redLight = buffer[0] - 48;
        lightOn();

        if (redLight == 0) {
          redState = 0;
        } 
        else {
          redState = 1;
        }
      }
    }

    if (data == 'w') { // Blue 조절
      if (bufferPosition == 4) {
        blueLight = (buffer[0]-48)*100 + (buffer[1]-48)*10 + (buffer[2]-48);
        lightOn();
        blueState = 1;
      } 
      else if (bufferPosition == 3) {
        blueLight = (buffer[0]-48)*10 + (buffer[1]-48);
        lightOn();
        blueState = 1;
      } 
      else if (bufferPosition == 2) {
        blueLight = buffer[0] - 48;
        lightOn();

        if (blueLight == 0) {
          blueState = 0;
        } 
        else {
          blueState = 1;
        }
      }
    }

    if (data == 'e') { // Green 조절
      if (bufferPosition == 4) {
        greenLight = (buffer[0]-48)*100 + (buffer[1]-48)*10 + (buffer[2]-48);
        lightOn();
        greenState = 1;
      } 
      else if (bufferPosition == 3) {
        greenLight = (buffer[0]-48)*10 + (buffer[1]-48);
        lightOn();
        greenState = 1;
      } 
      else if (bufferPosition == 2) {
        greenLight = buffer[0] - 48;
        lightOn();

        if (greenLight == 0) {
          greenState = 0;
        } 
        else {
          greenState = 1;
        }
      }
    }

    if(data == 't'){ //온습도 센서 제어
     int h = dht.readHumidity();         // 변수 선언 (h는 습도)
     int t = dht.readTemperature();     // 변수 선언 (t는 온도)

     //블루투스 전송 코드
     BTSerial.print("Humidity: ");          // Humidity를 출력
    BTSerial.print(h);                           // h(습도 값) 출력
    BTSerial.print(" %\t");                   //  %를 출력
    BTSerial.print("Temperature: ");    // Temperature를 출력
    BTSerial.print(t);                            // t(온도 값) 출력
    BTSerial.println(" C");

    //확인용 시리얼 모니터 출력 코드
    Serial.print("Humidity: ");          // Humidity를 출력
    Serial.print(h);                           // h(습도 값) 출력
    Serial.print(" %\t");                   //  %를 출력
    Serial.print("Temperature: ");    // Temperature를 출력
    Serial.print(t);                            // t(온도 값) 출력
    Serial.println(" C");  
    }

    if (data == '\n' || data == 'x') // 명령이 다 들어왔을 경우 버퍼 위치를 초기화 합니다.
    {
      bufferPosition = 0;
    }
  }
}

void lightOn() {  // 3개의 LED 밝기를 조절 하는 함수
  analogWrite(redLed1, redLight);
  analogWrite(blueLed1, blueLight);
  analogWrite(greenLed1, greenLight);
}

void setState(boolean red, boolean blue, boolean green) { // 3개의 LED의 상태를 저장하는 함수
  redState = red;
  greenState = blue;
  blueState = green;
}

void setLight(int red, int blue, int green) { // 입력 받은 LED의 밝기를 저장 하는 함수
  redLight = red;
  blueLight = blue;
  greenLight = green;
}
