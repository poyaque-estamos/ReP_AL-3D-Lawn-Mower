#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11);  // RX, TX

//Amp Sensor Variables.
int RawValueAmp= 0;
int RawValueVolt = 0;
int Raining = 5;
int Charging = 0;


// Transmission of the raw sensor values to the Arduino MEGA
float AmpsTX; 
float VoltsTX;
bool Test;


void setup(){ 
 Serial.begin(9600);
 mySerial.begin(1200);
 Test = 0;
 pinMode(A3, INPUT);

 if (Test == 1) {
  RawValueAmp = 510;
  RawValueVolt = 500;
 }
}

void Calculate_Volt_Amp() {


// Calculate Amp Value from Charging Pins
 int mVperAmp = 185;
 int ACSoffset = 2500; 
 double VoltageAmp = 0;
 double Amps = 0;
 VoltageAmp = (RawValueAmp / 1024.0) * 5000; // Gets you mV
 AmpsTX =  ((VoltageAmp - ACSoffset) / mVperAmp);



// Calculate Voltage Sensor Value from Battery
 
 float vout = 0.0;
 float R1 = 30000;      // 30000 Mower 2    Mower 1 30000
 float R2 = 7300;       // 7300 Mower 2     Mower 1 7500
 vout = (RawValueVolt * 5.0) / 1024.0; // see text
 VoltsTX = vout / (R2/(R1+R2)); 

}

 
void TX_Volts()  {  
  mySerial.print(VoltsTX);
  mySerial.println("\j");
  Serial.print("VoltsTX = ");
  Serial.print(VoltsTX);
  Serial.print("|");
}

void TX_Charge()  {
  mySerial.print(Charging);
  mySerial.println("\q");
}

void TX_Raining()  {
  mySerial.print(Raining);
  mySerial.println("\w");
}

void loop(){

 //Read Amp and Volt Raw Number Sensor Values 
 if (Test == 0) { 
 RawValueAmp = analogRead(A1);
 RawValueVolt = analogRead(A2);
 Raining = analogRead(A3);
 }

  if (Test == 1 )   {
 RawValueAmp = RawValueAmp + 1;
 if (RawValueAmp > 550) RawValueAmp = 500;
 RawValueVolt = RawValueVolt + 1;
 if (RawValueVolt > 550) RawValueVolt = 500;
  }


 Serial.print("VoltsTX Raw = ");
 Serial.print(RawValueVolt);
 Serial.print("|");
 Serial.print("AmpsTX Raw = ");
 Serial.print (RawValueAmp);
 Serial.print("|");


 Calculate_Volt_Amp();

 if (AmpsTX < 0.4) Charging = 0;
 if (AmpsTX > 0.4) Charging = 4;
 Serial.print("Charging = ");  
 Serial.print(Charging);
 Serial.print("|");


 Serial.print("AmpsTX = ");
 Serial.print (AmpsTX);
 Serial.print("|");


 Serial.print("Rain Sensor Raw = ");
 Serial.print (Raining);
 Serial.print("|");

if (Raining < 100) Raining = 0;
if (Raining >= 100) Raining = 1;

Serial.print( "Raining = ");
Serial.print(Raining);
Serial.print("|");


 Serial.println("");
 
 
 TX_Volts();
 delay(5);
 TX_Charge();
 delay(5);
 TX_Raining();

    
} 
