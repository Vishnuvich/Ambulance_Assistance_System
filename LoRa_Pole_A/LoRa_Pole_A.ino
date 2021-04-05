#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>

float latitude,longitude;
int distance, Alert_return_value;
String received_data, outgoing;
byte localAddress = 0xBB;     // address of this device
byte destination = 0xAA;      // destination to send to
byte nxt_destination = 0xCC;  // destination address of next pole
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends
//Pole location
const float fixedlat =30.236641;// 10.113079;// 
const float fixedlong =-97.821457;// 76.351358;//


/*--------------------------------------------------Recepient address check-----------------------------------------*/
void address_Check(int recipient)
{
    // if the recipient isn't this device or broadcast,
  if (recipient != localAddress ) {
    Serial.println("This message is not for me.");
    //return;                             // skip rest of function
  }
}

/*-------------------------------------------------- Length check----------------------------------*/

void length_Check(byte incomingLength, String incoming)
{
    if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    //return;                             // skip rest of function
  }
}

/*------------------------------------------- LoRa_Sender----------------------------------*/

void LoRa_Sender(String outgoing, byte destination, String Mssg)
{
    LoRa.beginPacket();
    LoRa.write(destination);              // add destination address
    LoRa.write(localAddress);             // add sender address
    LoRa.write(outgoing.length());        // add payload length
    LoRa.print(outgoing);                 // add payload
    Serial.println(Mssg);
    LoRa.endPacket();
}

/*--------------------------------------------------LoRa Receive Function----------------------------------------------------------*/

String LoRa_Receive()
{
  String received = "";
  // read packet
     while (LoRa.available()) 
    {
        received += (char)LoRa.read();//receives data
        delay(0);
    }
    return received;
}
/*-------------------------------------- LoRa_Alert Receiving Function -----------------*/

int onReceive_Alert()
{   
    int  recipient = LoRa.read();
    Serial.println("recipient :"+String(recipient,HEX));
    byte sender = LoRa.read();
    byte incomingLength = LoRa.read();
    String incoming = "";
    while(LoRa.available())
    {

      incoming += (char)LoRa.read();
      delay(0);
    }

    length_Check(incomingLength, incoming); //checks incoming length
    
    address_Check(recipient);
  
    if (incoming == "To")
    {
      Serial.println("recipient :"+String(recipient,HEX));
      Serial.println("Sender :"+String(sender,HEX));
      Serial.println("Incoming length");
      Serial.println(incomingLength);
      Serial.println(incoming);
      for (int i=0; i<3; i++)
      {
       LoRa_Sender("A",destination,"Ack Send to 0xAA"); //Send Ack back 
       delay(500);
      }
       return true;
    }
}


/*--------------------------------------------------LoRa Receiving Ack Function----------------------------------------------------------*/

void onReceive(int packetSize) {
  if (packetSize == 0) 
  return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) 
  {
    incoming += (char)LoRa.read();
  }

  address_Check(recipient);
  length_Check(incomingLength, incoming);
  
  // if message is for this device, or broadcast, print details:
  
  if (incoming == "A")
  {
    while(1)
        {
      Serial.println("Works");
      delay(2000);
        }
  }
}

/*--------------------------------Fuction for distance checking and extracting lat and long values--------------------------------*/

int distance_Check(String received_data)
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
      if (distance < 123 ) //give the difference in distance here
        {
          Serial.println(distance);
          Serial.println("Ambulance passed the pollling station");
          return true;/* returns true if the condition is satisfied ie, when the ambulance is within the specified range 
                       of station or we can say that the ambulance crossed the polling station*/
        }
}

/*-------------------------------------- Arduino  default functions ------------------------*/

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
   Alert_return_value = onReceive_Alert();
  while(Alert_return_value == true){

    // Give code for speaker here

  int packetSize = LoRa.parsePacket();// try to parse packet
  if (packetSize) 
  {
   
         received_data = LoRa_Receive(); //recieves location data from ambulance
    Serial.println(received_data);
  //}

    /*Checking the distance between ambulance and polling station,if it is less than specified value the function returns "true"*/
    if (distance_Check(received_data)== true)
    {
      while(1)
     {  
     if(millis() - lastSendTime > interval)//Checking the interval for sending data to next pole
     {
     LoRa_Sender("To", nxt_destination, "Alert to nxt  pole 0xCC");//Alerting next pole(Sending to next pole)
     lastSendTime = millis();
     }
     onReceive(LoRa.parsePacket());//Check for acknowledgement
    delay(0);
    }
    }

  }
    
   }
  }
}
