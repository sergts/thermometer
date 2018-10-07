#include<SoftwareSerial.h>

/* Create object named bt of the class SoftwareSerial */ 
SoftwareSerial bt(8,9); /* (Rx,Tx) */  

const int SENSOR_PIN = A0;
const int COLD_ZONE = 0;
const int COMFORT_ZONE = 1;
const int HOT_ZONE = 2; 
const int COLD_ZONE_LIMIT = 18;
const int HOT_ZONE_LIMIT = 26;
const int R_LED_PIN = 4;
const int G_LED_PIN = 5; 
const int B_LED_PIN = 6;
const int SAMPLES = 128;


void setup() {
  Serial.begin(9600);
  bt.begin(9600); /* Define baud rate for software serial communication */
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);
  digitalWrite(G_LED_PIN, HIGH);
}

int t = 0;
int tempZone = COMFORT_ZONE;
float sensorTotal = 0.0;
int totalSamples = 0;

void loop() {
  
  for (int i = 0; i < SAMPLES; i++) {
    sensorTotal += analogRead(SENSOR_PIN);
    totalSamples++;
  }

  if(t % 5 == 0) {
    float sensorVal = sensorTotal / totalSamples;
    sensorTotal = 0.0;
    totalSamples = 0;
     //convert to voltage
    float voltage = (sensorVal / 1024.0) * 5;
    //convert to temperature in C degrees
    float temperature = (voltage - .465) * 100;
    Serial.println(temperature);
    
    bt.println(temperature);
    
    if(temperature > HOT_ZONE_LIMIT && tempZone != HOT_ZONE) {
      tempZone = HOT_ZONE;
      digitalWrite(R_LED_PIN, HIGH);
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(B_LED_PIN, LOW);
    } else if (temperature < COLD_ZONE_LIMIT && tempZone != COLD_ZONE) {
      tempZone = COLD_ZONE;
      digitalWrite(R_LED_PIN, LOW);
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(B_LED_PIN, HIGH);
    } else if(temperature >= COLD_ZONE_LIMIT && temperature <= HOT_ZONE_LIMIT && tempZone != COMFORT_ZONE) {
      tempZone = COMFORT_ZONE;
      digitalWrite(R_LED_PIN, LOW);
      digitalWrite(G_LED_PIN, HIGH);
      digitalWrite(B_LED_PIN, LOW);
    }
  }
  t++;
  t %= 100000;
  delay(1000);
}
