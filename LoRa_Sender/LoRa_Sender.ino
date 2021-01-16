#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//For GPS Pins
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;

//Variables

float lati,longi;
String longitude, latitude;


// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

//Display Function

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    lati = (gps.location.lat());//Latitude float
    latitude = String(lati,6);//Latitude string
    Serial.print(lati,6); //printing lat in serial monitor
    Serial.print(F(","));
    longi = gps.location.lng();// Longitude float
    longitude = String(longi,6);//Longitude string
    Serial.print(longi,6);//printing long in serial monitor
    Serial.println();
    Serial.println(latitude+longitude); //String concatenation and printing
    LoRa.beginPacket();
    LoRa.print(latitude+longitude);
    Serial.println("Send!");
    LoRa.endPacket();
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}


void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(15,16,4);

  if (!LoRa.begin(473E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

}

void loop() {

    while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
      if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    delay(3000);
  }
  }
//   const char *gpsStream = "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n";
//   while(*gpsStream)
//    if (gps.encode(*gpsStream++)){
//      displayInfo();
//    }
//    delay(3000);
//}
