#define ENA_m2 10       // Enable/speed motor Front Left
#define ENB_m2 11       // Enable/speed motor Back Left
#define IN_21  8        // L298N #2 in 1 motor Front Left
#define IN_22  9        // L298N #2 in 2 motor Front Left
#define IN_23  12       // L298N #2 in 3 motor Back Left
#define IN_24  13       // L298N #2 in 4 motor Back Left

int command;            //Int to store app command state.
int speedCar = 1000;     // 50 - 255.
int speed_Coeff = 3;

int state = 0;

// defining the pins
const int trigPin1 = 4; //Left
const int echoPin1 = 7;
const int trigPin2 = 16; //Front
const int echoPin2 = 17;
const int trigPin3 = 15; //Right
const int echoPin3 = 18;

int irpin = 8;

// defining variables
float duration1, distanceleft;
float duration2, distancefront;
float duration3, distanceright;
int a=0;


int led = 5;


int measurePin = A0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

const int congtachanhtrinh = 3;  // Dây ra công tăc hành trình dây màu xanh nối chân 3   
//const int led =  13;  
int giatri = 0;  
#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial SIM900(12, 13); //SIM800L Tx & Rx is connected to Arduino #3 & #2
// Variable to store text message
String textMessage;
String message;
// Create a variable to store Lamp state
String lampState = "HIGH";

// Relay connected to pin 12
const int relay = 12;
void setup(){  
  
    Serial.begin(115200);

    pinMode(ENA_m2, OUTPUT);
    pinMode(ENB_m2, OUTPUT);
    pinMode(IN_21, OUTPUT);
    pinMode(IN_22, OUTPUT);
    pinMode(IN_23, OUTPUT);
    pinMode(IN_24, OUTPUT);

    digitalWrite(IN_21, LOW);
    digitalWrite(IN_22, LOW);
    digitalWrite(IN_23, LOW);
    digitalWrite(IN_24, LOW);

  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);// Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  
pinMode(irpin, INPUT);

pinMode(led, OUTPUT);
digitalWrite(led, LOW);
  
pinMode(ledPower,OUTPUT);

pinMode(congtachanhtrinh, INPUT);

// Set relay as OUTPUT
  pinMode(relay, OUTPUT);

  // By default the relay is off
  digitalWrite(relay, HIGH);
  
  // Initializing serial commmunication
  //Serial.begin(115200); 
  SIM900.begin(115200);

  // Give time to your GSM shield log on to network
  delay(5000);
  Serial.println("SIM start...");
 /*
  SIM900.println("AT"); //Handshaking with SIM900
  delay(100);
  SIM900.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  delay(100);
  SIM900.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  delay(100);
 SIM900.println("AT+CREG?"); //Check whether it has registered in the network
  delay(100);
*/
  // AT command to set SIM900 to SMS mode
  SIM900.println("AT"); //Handshaking with SIM900
  delay(100);
  SIM900.println("AT+CMGF=1"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.println("AT+CNMI=2,2,0,0,0");
  delay(100);

} 

void goAhead(){ 
      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);
      
      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);

  }

void goBack(){ 

      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);

  }

void goRight(){ 

      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);


  }

void goLeft(){
      
      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
      
        
  }

void goAheadRight(){

      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
 
  }

void goAheadLeft(){

      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, HIGH);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, HIGH);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);
 
  }

void goBackRight(){ 

      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);

  }

void goBackLeft(){ 
  
      digitalWrite(IN_21, HIGH);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);


      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, HIGH);
      analogWrite(ENB_m2, speedCar);

  }

void stopRobot(){  

      digitalWrite(IN_21, LOW);
      digitalWrite(IN_22, LOW);
      analogWrite(ENA_m2, speedCar);

      digitalWrite(IN_23, LOW);
      digitalWrite(IN_24, LOW);
      analogWrite(ENB_m2, speedCar);

      analogWrite(led, 0);
  
  }

void Auto(){
  analogWrite(led, 255);
  delay(100);

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distanceleft = duration1 * 0.034 / 2;
  Serial.print("Distance1: ");
  Serial.println(distanceleft);

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distancefront = duration2 * 0.034 / 2;
  Serial.print("Distance2: ");
  Serial.println(distancefront);

  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distanceright = duration3 * 0.034 / 2;
  Serial.print("Distance3: ");
  Serial.println(distanceright);
  int s = digitalRead(irpin);

  if(s==HIGH) //goAhead
  { 
    goBack();
    delay(1000);
    a=1;
    }
  if ((a==0)&&(s==LOW)&&(distanceleft <= 20 && distancefront > 20 && distanceright <= 20) || (a==0)&&(s==LOW)&&(distanceleft > 20 && distancefront > 20 && distanceright > 20))
  {//goBack
    goAhead();
  }
  if ((a==1)&&(s==LOW)||(s==LOW)&&(distanceleft <= 20 && distancefront <= 20 && distanceright > 20)||(s==LOW)&&(distanceleft <= 20 && distancefront <= 20 && distanceright > 20)||(s==LOW)&& (distanceleft <= 20 && distancefront > 20 && distanceright > 20)||(distanceleft <= 20 && distancefront > 20 && distanceright > 20))
  {//goRight
    goRight();
    delay(100);
    a=0;
  }
  if ((s==LOW)&&(distanceleft > 20 && distancefront <= 20 && distanceright <= 20) ||(s==LOW)&& (distanceleft > 20 && distancefront > 20 && distanceright <= 20) ||(s==LOW)&& (distanceleft > 20 && distancefront <= 20 && distanceright > 20) )
  {//goLeft
    goLeft();
  } 
  //delay(100);

  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 170 * calcVoltage - 0.1;


  Serial.println("Raw Signal Value (0-1023):");
  Serial.println(voMeasured);

  Serial.println("Voltage:");
  Serial.println(calcVoltage);

  Serial.println("Dust Density:");
  Serial.println(dustDensity); // unit: (muy) ug/m3

  if(dustDensity > 210)
  {
    Serial.println("HIGH");
    sendSMS(message);
    }
  else{
    Serial.println("LOW");
    }
  
  //delay(100);
  giatri = digitalRead(congtachanhtrinh); // Đọc tín hiệu từ chân số 3
  Serial.print("GIA TRI NHAN DUOC: ");
 Serial.println(giatri);
 // giá trị nhận được từ công tắc hành trình bằng 1 thì led sáng 
 // còn giá trị nhận được từ công tắc hành trình bằng 0 thì led tắt
  if (giatri == HIGH) {
    //digitalWrite(led, HIGH);
    Serial.println("go Ahead");
  } 
  else {
    //digitalWrite(led, LOW);
    Serial.println("go Back");
  }
  delay(100);// delay 1 giây
 if(SIM900.available()>0){
    textMessage = SIM900.readString();
    Serial.print(textMessage);    
    delay(10);
  } 
  if(textMessage.indexOf("ON")>=0){
    // Turn on relay and save current state
    digitalWrite(relay, LOW);
    lampState = "on";
    Serial.println("Relay set to ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0){
    // Turn off relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "off"; 
    Serial.println("Relay set to OFF");
    textMessage = ""; 
  }
  if(textMessage.indexOf("STATE")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
}
void highest()
{
  analogWrite(led, 255);
  }
void medium()
{
  analogWrite(led, 200);
  }
void lowest()
{
  analogWrite(led, 150);
  }
  void stop_mos()
{
  analogWrite(led,0);
  }
void Circle()
{
  goRight();
  delay(5000);
  goAhead();
  delay(1000);
  }
/*void zigzag()
{
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;
  int distance = round(distance_cm);
  
  //Serial.print("distance: ");
  Serial.print(distance);
  //Serial.println(" cm");
  //delay(200);
  
  int SensorValue = digitalRead(19);
  
  //goAhead();
  
  //Serial.print("SensorPin Value: ");
  //Serial.println(SensorValue);
  //delay(300);
  
   if ((distance <= 5) || (SensorValue == 1)){ // LOW MEANS Object Detected
      goBack();
      //delay(3000);
      //goRight();
      //delay(5000);
  }
  
  
  }*/

void loop(){

  if (Serial.available() > 0){ 
  command = Serial.read();
  
      if (command == 'F') state = 2;
      else if (command == 'B') state = 3;
      else if (command == 'L') state = 4;
      else if (command == 'R') state = 5;
      //else if (command == 'I') goAheadRight();
      else if (command == 'G') goAheadLeft();
      else if (command == 'J') goBackRight();
      else if (command == 'H') goBackLeft();
      else if (command == '0') speedCar = 400;
      else if (command == '1') speedCar = 470;
      else if (command == '2') speedCar = 540;
      else if (command == '3') speedCar = 610;
      else if (command == '4') speedCar = 680;
      else if (command == '5') speedCar = 750;
      else if (command == '6') speedCar = 820;
      else if (command == '7') speedCar = 890;
      else if (command == '8') speedCar = 960;
      else if (command == '9') speedCar = 1023;
      else if (command == 'S') state = 6;

      else if (command == 'A') state = 1;
      
      else if (command == 'P') state = 0;
      
      else if (command == 'O') state = 7;
      else if (command == 'M') state = 8;
      else if (command == 'C') state = 9;
      else if (command == 'I') state = 10;
      else if (command == 'U') state = 11;
      }

if(state == 1)
{
  Auto();
  }
  else if(state == 2)
  {
    goAhead();
    }
    else if(state == 3)
  {
    goBack();
    }
    else if(state == 4)
  {
    goLeft();
    }
    else if(state == 5)
  {
    goRight();
    }
    else if(state == 6)
  {
    stopRobot();
    }
    else if(state == 7)
  {
    highest();
    }
    else if(state == 8)
  {
    medium();
    }
    else if(state == 9)
  {
    lowest();
    }
    else if(state == 10)
  {
    Circle();
    }
    else if(state == 11)
  {
    //zigzag();
    }
  else
  {
      stop_mos();
    }

}


// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM900.println("AT+CMGF=1"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+84868475996\""); 
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.write(26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}