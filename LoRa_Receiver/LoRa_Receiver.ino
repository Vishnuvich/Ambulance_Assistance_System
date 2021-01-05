#include <SPI.h>
#include <LoRa.h>

float latitude,longitude;
int distance;
char charreceived[32];
//Pole location
const float fixedlat = 30.236641; 
const float fixedlong = -97.821457;


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
  LoRa.setPins(15,16,4);
  if (!LoRa.begin(473E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
   int a = 0;
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet
    while (LoRa.available()) {
       char receiv = LoRa.read();//receives one character at a time(iteration)
       //storing each char to an array
       charreceived[a]=receiv;
       a++;
    }
    
    //Array to string
    String received = charreceived;
    //Serial.println(received);
    //splitting received string and converting to float
    latitude =received.substring(0,8).toFloat();
    longitude = received.substring(9).toFloat();
    Serial.print("Latitude : ");
    Serial.println(latitude,6);
    Serial.print("Longitude :");
    Serial.println(longitude,6);
    //Distance calculation, returns distance in meter
    distance = TinyGPSPlus::LoRa.distanceBetween(latitude,longitude,fixedlat,fixedlong);
      if (distance < 100 ) 
        {
          Serial.println(distance);
          Serial.println("Ambulance passed the pollling station");
        }
    
    
      
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
