/*
 Machine Countint Material By Oattt
 ปุ่มการทำงาน 'A' ไว้กำหนดจำนวนที่จะนับ 
            'B' เริ่มการทำงานนับ
            'C' Clear ค่าที่กำหนดไว้
            'D' ไม่มี
            '*' ไม่มี
            '#' เป็น .
 วิธีการใช้งาน 1.กด A 1ครั้ง จะแสดงผลให้กำหนดจำนวนนับ 
            2.ป้อนจำนวนนับ
            3.กด A อีกครั้งเพื่อ Save ค่าที่ป้อน
            4.กด B เพื่อเริ่มนับ
            5.เมื่อนับครบจำนวนที่ป้อนจะมีไฟแสดงสถานะสีเขียว เป็นอันว่านับเสร็จสิ้นแล้ว 
          **หมายเหต เมื่อนับเสร็จแล้วอยากเปลี่ยนค่านับ ให้เริ่มทำข้อ 1-3 ใหม่
 */
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const int BUZZER_PIN = 10; // buzzer
const int ROW_NUM    = 4; // four rows
const int COLUMN_NUM = 4; // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '.', 'D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};      // connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

int cursorColumn = 0;
int setupcount = 0; // count when push D (on keypad)
int count = 0;
int keep_valsensor = 0;
int sensorPin = 0;
int sensor_val = 0;
int led_red = 11;
int led_green = 12;
int led_blue = 13;
String inputString;
long countSet;
int A = 0;

void setup() {
  pinMode(led_blue, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  Serial.begin(9600);
  inputString.reserve(10);
  lcd.begin();
  lcd.clear();
  lcd.print(" Count Machine");
  lcd.setCursor(2 , 1);
  lcd.print("By : JAOOatt");
  delay(3000);
  lcd.clear();
  lcd.home();
  lcd.print("  A=Set B=Count");
  lcd.setCursor(5, 1);
  lcd.print("C=Clear");
}

void loop() {
  digitalWrite(13, 0);
  digitalWrite(12, 0);
  digitalWrite(11, 255);
  keypad_set();
  sensor_count1();
}

void keypad_set() {//-----------------------------------------------------------------
  char key = keypad.getKey();
  if (key) {
    lcd.setCursor(cursorColumn + 6, 1); // move cursor to   (cursorColumn, 0)
    lcd.print(key);     // print key at (cursorColumn, 0)
    cursorColumn++;                // move cursor to next position
  }

  if (cursorColumn == 6) {       // if reaching limit, clear LCD
    lcd.clear();
    lcd.print("   Count Setup ");
    lcd.setCursor(5, 1);
    lcd.print(":");
    lcd.blink();
    cursorColumn = 0;
  }

  if (setupcount == 2) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Save Setup");
    lcd.setCursor(0, 1);
    lcd.print("B=Count C=Clear");
    lcd.noBlink();
    setupcount = 0;
  }

  if (key != NO_KEY) {
    if (key >= '0' && key <= '9') {     // only act on numeric keys
      inputString += key;               // append new character to input string
    } else if (key == 'A') {
      if (inputString.length() > 0) {
        countSet = inputString.toInt(); // YOU GOT AN INTEGER NUMBER
        inputString = "";               // clear input
        Serial.println(countSet);
        cursorColumn = 0;
      }
    }
  }
  switch (key) {
    case 'A':
      lcd.clear();
      lcd.print("   Count Setup ");
      lcd.noDisplay();
      delay(500);
      lcd.display();
      delay(500);
      beep();           // function setup value counting.
      setupcount++;
      delay(30);
      cursorColumn = 0;
      lcd.setCursor(5, 1);
      lcd.print(":");
      lcd.blink();
      keep_valsensor = 0;
      break;

    case 'B':
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Count Working...");
      lcd.setCursor(6, 1);
      lcd.print(":");

      break;

    case 'C':
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Clear finish");
      lcd.setCursor(0, 1);
      lcd.print("Please set again");
      beep();
      delay(1000);
      lcd.clear();                      // function clear value
      lcd.setCursor(3, 0);
      lcd.print("Count Setup");
      lcd.setCursor(5, 1);
      lcd.print(":");
      lcd.blink();
      inputString = "";
      cursorColumn = 0;
      setupcount++;
      if (setupcount > 2 ) {
        setupcount = 1;
      }
      break;
  }
}//---------------------------------------------------------------------------------

void beep() {
  digitalWrite(10, HIGH);
  delay(100);
  digitalWrite(10, LOW);
  delay(100);
  digitalWrite(10, HIGH);

}

void sensor_count1() {
  sensor_val = digitalRead(sensorPin);
  //int sensor_keep = sensor_val;

  if (sensor_val == LOW && A == 0) {
    keep_valsensor++;
    digitalWrite(13, 255);
    digitalWrite(12, 0);
    digitalWrite(11, 255);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Count Working...");
    lcd.setCursor(6, 1);
    lcd.print(":");
    lcd.print(keep_valsensor);
    delay(50);
    A = 1;
  }
  if (sensor_val == HIGH) {
    A = 0;
  }
  checkFinish();
}

void checkFinish() {
  if (countSet > 1) {
    if (keep_valsensor == countSet) {
      beep();
      beep();
      setupcount = 0;
      keep_valsensor = 0;
      Serial.println(setupcount);
      digitalWrite(13, 0);
      digitalWrite(12, 255);
      digitalWrite(11, 0);
      delay(10);
      lcd.clear();
      lcd.print(" Count Success");
      lcd.setCursor(3, 1);
      lcd.print("Total: ");
      lcd.print(countSet);
      lcd.noBlink();
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("  A=Set B=Count");
      lcd.setCursor(5, 1);
      lcd.print("C=Clear");
    }
  }
}
