#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>

float latitude,longitude;
int distance;
char charreceived[32];
String received, received_data;
//Pole location
const float fixedlat = 10.113079;//30.236641; 
const float fixedlong = 76.351358;//-97.821457;


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
/*--------------------------LoRa Receive Function----------------------------*/

String LoRa_Receive()
{
  int a = 0;
  // read packet
      while (LoRa.available()) 
    {
       char receiv = LoRa.read();//receives one character at a time(iteration)
       //storing each char to an array
       charreceived[a]=receiv;
       a++;
    }
    //Array to string
    received = charreceived; //here the variable "received" is a string type
    return received;
}

/*--------------------------------Fuction for distance checking and extracting lat and long values--------------------------*/

void distance_Check(String received_data)
{
    //splitting received string and converting to float
    latitude =received_data.substring(0,9).toFloat();
    longitude = received_data.substring(9).toFloat();
    Serial.print("Latitude : ");
    Serial.println(latitude,6);
    Serial.print("Longitude :");
    Serial.println(longitude,6);
    //Distance calculation, returns distance in meter
    distance = TinyGPSPlus::distanceBetween(latitude,longitude,fixedlat,fixedlong);
      Serial.println(distance);
      if (distance < 100 ) 
        {
          Serial.println(distance);
          Serial.println("Ambulance passed the pollling station");
        }
}


void loop() {
   
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    received_data = LoRa_Receive();//calling user defined lora receive function which returns the received string
    Serial.println(received_data);
    
    distance_Check(received_data);
      
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
