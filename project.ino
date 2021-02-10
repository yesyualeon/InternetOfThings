#include <ESP8266_Lib.h>

#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd1(7,6,5,4,3,2); // pins for LCD Connection
 
//lcd blynk
WidgetLCD lcd(V1);

#define buzzer 12 // buzzer pin
#define led 11 //led pin 
#define x A0 // x_out pin of Accelerometer
#define y A1 // y_out pin of Accelerometer
#define z A2 // z_out pin of Accelerometer

// auth blynk
char auth[] = "C-p5YRXwiaD3vC2CXzhNMlZLPqIVc_rq";

// pengaturan wifi
char ssid[] = "SD A2";
char pass[] = "55141oke";

SoftwareSerial EspSerial(9,10); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

//variables//
int xsample=0;
int ysample=0;
int zsample=0;
long start;
int buz=0;
 
//Macros//
#define samples 50
#define maxVal 20 // max change limit
#define minVal -20 // min change limit
#define buzTime 5000 // buzzer on time

void setup()
{
// Debug console
Serial.begin(9600);

// Set ESP8266 baud rate
EspSerial.begin(ESP8266_BAUD);
delay(10);

// Blynk.begin(auth, wifi, ssid, pass);
// You can also specify server:
Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 8080); //khusus indihomo
//Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

lcd1.begin(16,2);//initializing lcd1
Serial.begin(9600); // initializing serial
delay(1000);
lcd.print(0,0,"EarthQuake ");
lcd.print(0,1,"Detector ");
lcd1.print("EarthQuake ");
lcd1.setCursor(0,1);
lcd1.print("Detector ");
delay(2000);
lcd.clear();
lcd.print(0,0,"Calibrating.....");
lcd.print(0,1,"Please wait...");
lcd1.clear();
lcd1.print("Calibrating.....");
lcd1.setCursor(0,1);
lcd1.print("Please wait...");
pinMode(buzzer, OUTPUT);
pinMode(led, OUTPUT);
buz=0;
digitalWrite(buzzer, buz);
digitalWrite(led, buz);
for(int i=0;i<samples;i++) // taking samples for calibration
{
xsample+=analogRead(x);
ysample+=analogRead(y);
zsample+=analogRead(z);
}
 
xsample/=samples; // taking avg for x
ysample/=samples; // taking avg for y
zsample/=samples; // taking avg for z
 
delay(3000);
lcd.clear();
lcd.print(0,0,"Calibrated");
lcd1.clear();
lcd1.print("Calibrated");
delay(1000);
lcd.clear();
lcd.print(0,0,"Device Ready");
lcd1.clear();
lcd1.print("Device Ready");
delay(1000);
lcd.clear();
lcd1.clear();
}


void loop()
{
   Blynk.run(); 
int value1=analogRead(x); // reading x out
int value2=analogRead(y); //reading y out
int value3=analogRead(z); //reading z out
 
int xValue=xsample-value1; // finding change in x
int yValue=ysample-value2; // finding change in y
int zValue=zsample-value3; // finding change in z
float gX = xValue/67.584;
float gY = yValue/67.584;
float gZ = zValue/67.584;
float Xsquared = pow(gX,2);
float Ysquared = pow(gY,2);
float Zsquared = pow(gZ,2);
float sum=Xsquared+Ysquared+Zsquared;
float DG=sqrt(sum);
float MR=sqrt(sum);


lcd1.setCursor(0,1);
lcd1.print(MR);
//delay
/* comparing change with predefined limits*/
if(xValue < minVal || xValue > maxVal || yValue < minVal || yValue > maxVal || zValue < minVal || zValue > maxVal)
{
  if(buz == 0)
  start=millis(); // timer start
  buz=1; // buzzer / led flag activated
}
 
else if(buz == 1) // buzzer flag activated then alerting earthquake
{
  lcd.print(0,0,"Earthquake Alert ");
  lcd1.setCursor(0,0);
  lcd1.print("Earthquake Alert ");
  lcd1.print(MR);
  //delay(100);

  if (MR < 0.017)
  {
    lcd.print(0,1,"Gempa Micro");
    lcd.print(1,0,"1 SR");
    //delay(100);
    Blynk.notify("Gempa Micro");
  }

  else if (MR >= 0.017 || MR < 0.0785)
  {
    lcd.print(0,1,"Gempa Minor");
    lcd.print(1,0,"2 SR");
    //delay(100);
    lcd1.print("Earthquake Alert ");
    Blynk.notify("Gempa Minor");
   }

  else if (MR >= 0.0785 || MR < 0.14)
  {
    lcd.print(0,1,"Gempa Minor");
    lcd.print(1,0,"3 SR");
    //delay(100);
    lcd1.print("Earthquake Alert ");
    Blynk.notify("Gempa Minor");
   }

  else if (MR >= 0.14 || MR < 0.39)
  {
    lcd.print(0,1,"Gempa Ringan");
    lcd.print(1,0,"4 SR");
    //delay(100);
    lcd1.print("Earthquake Alert ");
    Blynk.notify("Gempa Ringan");
   }

   else if (MR >= 0.39 || MR < 0.92)
  {
    lcd.print(0,1,"Gempa Sedang");
    lcd.print(1,0,"5 SR");
    //delay(100);
    lcd1.print("Earthquake Alert ");
    Blynk.notify("Gempa, Tidak Berpotensi Tsunami");
   }

  else if (MR >= 0.92 || MR < 1.8)
  {
    lcd.print(0,1,"Gempa Besar");
    lcd.print(1,0,"6 SR");
    //delay(100);
    lcd1.print("Earthquake Alert ");
    Blynk.notify("Gempa, Berpotensi Tsunami!");
   }

  else if (MR >= 1.8)
  {
    lcd.print(0,1,"Gempa Besar");
    lcd1.print("Earthquake Alert ");
    Blynk.notify("Gempa, Sangat Berpotensi Tsunami!");
    if (MR >= 1.8 || MR < 3.4)
    lcd.print(1,0,"7 SR");
    else if (MR >= 3.4 || MR < 6.5)
    lcd.print(1,0,"8 SR");
    else if (MR >= 6.5 || MR < 12.4)
    lcd.print(1,0,"9 SR");
    else if (MR >= 12.4)
    lcd.print(1,0,"10 SR");
  }

if(millis()>= start+buzTime)

//spesifik
lcd.clear();
lcd1.clear();
buz=0;
}
 
else
{
lcd.clear();
lcd.print(0,0, "Aman Terkendali");
lcd1.setCursor(0,0);
lcd1.print("Getaran tanah");
}

Blynk.virtualWrite (V3, MR);
digitalWrite(buzzer, buz); // buzzer on and off command
digitalWrite(led, buz); // led on and off command
Serial.println("x = ");
Serial.println(xValue);
Serial.println("y = ");
Serial.println(yValue);
Serial.println("z = ");
Serial.println(zValue);
Serial.println(MR); 
}
