#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
LiquidCrystal_I2C lcd(0x27,16,2);
dht tempmodule;
int Min = 34;
int Max = 35;
int TempResult;
float voltage;
float R1 = 1000000;
float R2 = 333333.3333;
int powerCutDetected = 0;
//OUTPUTS
int MainLight = 2;
int DeskLight = 3;
int Fan = 4;
int BackLights = 5;
int Buzzer = 6;

//INPUTS
int Door = 7;
int Temp = 8;
int Light = 9;
int PIR = 10;
int Movbypass = 11;

void setup() {
  Serial.begin(9600);
  pinMode(MainLight,OUTPUT);
  pinMode(DeskLight,OUTPUT);
  pinMode(Fan,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(BackLights,OUTPUT);
  pinMode(Door,INPUT);
  pinMode(Temp,INPUT);
  pinMode(Light,INPUT);
  pinMode(PIR,INPUT);
  pinMode(Movbypass,INPUT);
  digitalWrite(MainLight,HIGH);
  digitalWrite(DeskLight,HIGH);
  digitalWrite(Fan,HIGH);
  digitalWrite(Buzzer,LOW);
  digitalWrite(BackLights,HIGH);
  Buzzerout();
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Welcome!!!");
  lcd.setCursor(3,1);
  lcd.print("Navida");
  delay(5000);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("System Active");
  delay(2000);
  lcd.init();
  lcd.noBacklight();
}


void loop() {
}

//Temperature 
int Temperature(){
  int val;
  tempmodule.read11(Temp);
  Serial.print("Temperature: ");
  Serial.print(tempmodule.temperature);
  Serial.print("\t Humidity: ");
  Serial.println(tempmodule.humidity);
  if (tempmodule.temperature > Max){
    val = 1;
  }
  if (tempmodule.temperature < Min){
    val = 0;
  }
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(7,0);
  lcd.print(tempmodule.temperature);
  lcd.setCursor(0,1);
  lcd.print("Humidity:");
  lcd.setCursor(10,1);
  lcd.print(tempmodule.humidity);
  lcd.init(); 
  delay(500);
  return val;
}

//Door
int DoorCheck(){
  int val;
  val = digitalRead(Door);
  if (val == 0){
    Serial.println("Door Open");
  }
  else{
    Serial.println("Door Close");
  }
  return val;
}

//Buzzer
int Buzzerout(){
  tone(Buzzer,4000); 
  delay(500);
  noTone(Buzzer);
  delay(500);
  tone(Buzzer,4000); 
  delay(500);
  noTone(Buzzer);
}

void Buzzeroff() {
  noTone(Buzzer);
}

//Power
int Powercut() {
  float value = analogRead(A0);
  voltage = value * (5.0 / 1023) * ((R1 + R2) / R2) - 0.22;

  if (voltage != 0 && !powerCutDetected) {
    powerCutDetected = 1; 
    Buzzerout();
    Serial.println("Power Failure");
    Serial.println("Check Wifi");
    int check = Led(); // Optional: Check light status and adjust BackLights if needed
    if (check == 1) {
      digitalWrite(BackLights, LOW);
    } else {
      digitalWrite(BackLights, HIGH);
    }
  } else if (voltage == 0 && powerCutDetected) {
    digitalWrite(BackLights, HIGH);
    powerCutDetected = 0;
    Buzzeroff();
  }
}

//Light
int Led(){
  int val;
  val = digitalRead(Light);
  Serial.println(val);
  if (val == 1){
    Serial.println("Dark");
  }
  else{
    Serial.println("Light");
  }
  return val;
}
