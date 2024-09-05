#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 20, 4);
extern volatile unsigned long timer0_millis;
int buz=7;
int btn=6;
int cnt=0;

void setup() {
  // 한번만 실행되는 초기 설정 코드
  Serial.begin(9600);
  pinMode(buz, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // 반복해서 실행되는 메인 코드
  if(digitalRead(btn) == LOW) {
    cnt += 1;
    timer0_millis=0;
    if (cnt == 2) {
      cnt = 0;
    }

    Serial.println('1');
    
  }



  if (cnt == 0) {
    lcd.init();
  }
   
  
  if (cnt == 1) {
    unsigned long time1 = millis() / 1000;
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print(time1);

    if(time1%30==0){
      Serial.println(time1);
    }
    
    if (time1 == 180) {
      cnt = 0;
      
      tone(buz, 1500, 300); // 1500 Hz의 소리를 300 ms 동안 재생
      lcd.setCursor(0, 0);
      lcd.print("TIME OUT !!");
      Serial.println('2');
      delay(500);
    }
  }
}
