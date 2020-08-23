#include <FirebaseArduino.h>


#include  <ESP8266WiFi.h>

#define FIREBASE_HOST ""
#define WIFI_SSID "rhtsn" // Change the name of your WIFI
#define WIFI_PASSWORD "rhtsn1012" // Change the password of your WIFI

byte sensorInterrupt = 0;  // 0 = digital pin 2
#define FlowMeterPulsePerSecondPerLiter 6.9
//#define FlowMeterPulsePerSecondPerLiter 7.5

volatile float PreviousPulseTime;
volatile float PulseDuration;
float LPM;
float averageLPM;
float CumulativeLPM;
float averageCount;
int LPMdecimal;

void setup() 
{
  Serial.begin(115200); 
   WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");}
    Serial.println ("");
  Serial.println ("WiFi Connected!");
  Firebase.begin(FIREBASE_HOST);
  PreviousPulseTime = 0;
  PulseDuration     = 0;
  LPM               = 0;
  averageLPM        = 0;
  averageCount      = 0;
  CumulativeLPM     = 0;
  LPMdecimal        = 2;
  attachInterrupt(sensorInterrupt, pulseCounter, CHANGE);
  }

void loop() 
  {

  CalculateFlow1();

  }

void CalculateFlow1()
  {
  CumulativeLPM = CumulativeLPM + (500/(PulseDuration*FlowMeterPulsePerSecondPerLiter)); 
  averageCount ++; averageLPM = CumulativeLPM/averageCount;
  if (averageCount >10000) 
    { 
    LPM = averageLPM;
    Serial.println(LPM);
    Firebase.setFloat ("flowrate",LPM);
    
    averageCount=0;   CumulativeLPM = 0; 
    }  
  }
  
void pulseCounter()
  {
  PulseDuration = millis() - PreviousPulseTime;
  PreviousPulseTime = millis(); 
  }





  
