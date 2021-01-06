#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>

void setup() 
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("LoRa_Pole");
  LoRa.setPins(15, 16, 4);
  if (!LoRa.begin(473E6))
  {
    Serial.println("Starting LoRa failed!");
    while(1);
  }
}

void loop() 
{
  int packetsize = LoRa.parsePacket();
  if(packetsize)
  {
    String incoming = "";
    while(LoRa.available())
    {
      incoming += (char)LoRa.read();
    }
    if (incoming == "ToHospital")
    {
      Serial.println("Working");
    }
  }
}
