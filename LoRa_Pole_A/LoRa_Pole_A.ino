#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
byte localAddress = 0xFF;     // address of this device
byte destination = 0xBB;      // destination to send to
void LoRa_Sender(String outgoing)
{
    LoRa.beginPacket();
    LoRa.print(destination);              // add destination address
    LoRa.print(localAddress);             // add sender address
    LoRa.print(outgoing.length());        // add payload length
    LoRa.print(outgoing);                 // add payload
    Serial.println("Send data to next pole");
    LoRa.endPacket();
}

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
    int  recipient = LoRa.read();
    byte sender = LoRa.read();
    byte incominglength = LoRa.read();
    String incoming = "";
    while(LoRa.available())
    {

      incoming += (char)LoRa.read();
    }

    if (recipient != localAddress ) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }
  
    if (incoming == "ToHospital")
    {
      Serial.println("recipient :",String(recipient,HEX);
      Serial.println("Sender :",String(sender,HEX);
      Serial.println("Incoming length");
      Serial.println(incominglegth);
      Serial.println(incoming);
      LoRa_Sender("A");
    }
  }
}
