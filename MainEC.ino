#include <dht.h>
#include <EEPROM.h>
#include "GravityTDS.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"

unsigned long minute = 60000;
unsigned long stime = 0;
char SerialInput = 'x';


#define samplingInterval 1500
#define DHTPIN 11
#define WaterLevelPIN  7
#define TdsSensorPin A0
#define FAN1live 6
#define FAN1gnd 3
#define FAN2live 9
#define FAN2gnd 10


SoftwareSerial EEBlue(4, 5); // TX | RX on module 
RTC_DS1307 RTC;
dht DHT;
GravityTDS gravityTds;

float tdsValue = 0;
int Liquid_level=0;

void setup(){
  
    Serial.begin(9600);
    EEBlue.begin(9600);
    Wire.begin();                   //I2C Start
    RTC.begin();                    // RTC Start
    
    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      RTC.adjust(DateTime(__DATE__, __TIME__)); // sets the RTC to the date & time this sketch was compiled
    }
    RTC.adjust(DateTime(__DATE__, __TIME__));
      pinMode(WaterLevelPIN,INPUT);  // Set Water Level PIN
      gravityTds.setPin(TdsSensorPin); // set TDS sendor PIN
      gravityTds.setAref(5.0);  //reference voltage for TDS
      gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
      gravityTds.begin();  //initialization for TDS
}

void loop(){
  
    //int chk = DHT.read11(DHTPIN); // Check Temp DHT11
    gravityTds.setTemperature(DHT.temperature);  // set the temperature and execute temperature compensation on TDS
    gravityTds.update();  //sample and calculate TDS 
    tdsValue = gravityTds.getTdsValue();  // Get the value for TDS
    Liquid_level=digitalRead(WaterLevelPIN); // Get liqiod Level read 
    DateTime now = RTC.now();   // Get RTC time 


 if (Serial.available() > 0) {
     
    SerialInput = Serial.read();
      Serial.print("received: ");
      Serial.println(SerialInput);
      
    switch (SerialInput){
      case 'c':
                      analogWrite(FAN1live, 150);
                      analogWrite(FAN1gnd, 0);
                      analogWrite(FAN2live, 0);
                      analogWrite(FAN2gnd, 150);
                      Serial.print("Enable FAN 1, Time :");
                      stime=millis();
                      Serial.println(stime);
                      delay(20000);   // 5 sec delay before stop
                      analogWrite(FAN1live, 0);
                      analogWrite(FAN1gnd, 0);
                      analogWrite(FAN2live, 0);
                      analogWrite(FAN2gnd, 0);
                      Serial.print("Stop Fan1, Time: ");
                      stime=millis();
                      Serial.println(stime);                 
                      delay(5000);
       break;
             case 'x':
                      analogWrite(FAN1live, 0);
                      analogWrite(FAN1gnd, 0);
                      analogWrite(FAN2live, 0);
                      analogWrite(FAN2gnd, 0);
                      Serial.print("Enable FAN 1, Time :");
                      stime=millis();
                      Serial.println(stime);
                      delay(2000);   // 5 sec delay before stop
                      analogWrite(FAN1live, 0);
                      analogWrite(FAN1gnd, 0);
                      analogWrite(FAN2live, 0);
                      analogWrite(FAN2gnd, 0);
                      Serial.print("Stop Fan1, Time: ");
                      stime=millis();
                      Serial.println(stime);                 
                      delay(5000);
       break;
   }
 }

static unsigned long stime = millis();
if (millis()-stime > samplingInterval) { 
    int chk = DHT.read11(DHTPIN);
    EEBlue.print(now.year(), DEC);
    EEBlue.print('/');
    EEBlue.print(now.month(), DEC);
    EEBlue.print('/');
    EEBlue.print(now.day(), DEC);
    EEBlue.print(' ');
    EEBlue.print(now.hour(), DEC);
    EEBlue.print(':');
    EEBlue.print(now.minute(), DEC);
    EEBlue.print(':');
    EEBlue.print(now.second(), DEC);
    EEBlue.println(); 
    EEBlue.print("Liquid_level= ");
    EEBlue.println(Liquid_level,DEC);
    EEBlue.print(tdsValue,0);
    EEBlue.println("ppm");
    EEBlue.print("Temperature = ");
    EEBlue.println(DHT.temperature);
    EEBlue.print("Humidity = ");
    EEBlue.println(DHT.humidity);
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();   
    Serial.print ("Liquid_level= ");
    Serial.println(Liquid_level,DEC);
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    Serial.print("Temperature = ");
    Serial.println(DHT.temperature);
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity);
    //delay(1500);
    stime=millis();
 }
}
