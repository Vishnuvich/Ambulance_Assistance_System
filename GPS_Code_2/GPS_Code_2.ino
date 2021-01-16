//Viral Science www.viralsciencecreativity.com www.youtube.com/c/viralscience
//Realtime GPS Tracker with Nodemcu ESP8266
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>


static const int RXPin = 4, TXPin = 5;   // GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800

TinyGPSPlus gps; // The TinyGPS++ object


SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device



float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS



//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
  

void setup()
{
  Serial.begin(115200);
  Serial.println();
  ss.begin(GPSBaud);
 
}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
     
  }
}

void loop()
{
    while (ss.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read()))
        displayInfo();
  }


}

void displayInfo()
{ 
  if (gps.location.isValid() ) 
  {    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6); 
    
    spd = gps.speed.kmph();               //get speed
      Serial.println(spd);
       
       sats = gps.satellites.value();    //get number of satellites
     
        Serial.println(sats);
       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
        Serial.println(bearing);                    
  }
  
}
