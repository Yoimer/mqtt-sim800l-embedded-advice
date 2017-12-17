  int led = 13;
  unsigned int Counter = 0;
  unsigned long datalength,CheckSum,RLength;
  unsigned short topiclength;
  unsigned char topic[30];
  char str[250];
  unsigned char encodedByte;
  int X; 
  unsigned short MQTTProtocolNameLength;
  unsigned short MQTTClientIDLength;
  unsigned short MQTTUsernameLength;
  unsigned short MQTTPasswordLength;
       
 
  const char MQTTHost[30] = "io.adafruit.com";
  const char MQTTPort[10] = "1883";
  const char MQTTClientID[20] = "ABCDEF";
  const char MQTTTopic[30] = "raviypujar/feeds/switchfeed";
  const char MQTTProtocolName[10] = "MQTT";
  const char MQTTLVL = 0x03;
  const char MQTTFlags = 0xC2;
  const unsigned int MQTTKeepAlive = 60;
  const char MQTTUsername[30] = "raviypujar";
  const char MQTTPassword[35] = "993df7b916494d19b430d53cfcbe677d";
  const char MQTTQOS = 0x00;
  const char MQTTPacketID = 0x0001;
  void setup()
  {
    pinMode(led, OUTPUT); 
    Serial.begin(9600);    
    Serial.println("Arduino MQTT Tutorial, Valetron Systems @www.raviyp.com ");
    delay(3000); 

  }
  
  void SendConnectPacket(void)
  {
    Serial.print("\r\nAT+CIPSEND\r\n");
    delay(3000);
    
    Serial.write(0x10);
    
    MQTTProtocolNameLength = strlen(MQTTProtocolName);
    MQTTClientIDLength = strlen(MQTTClientID);
    MQTTUsernameLength = strlen(MQTTUsername);
    MQTTPasswordLength = strlen(MQTTPassword);
    datalength = MQTTProtocolNameLength+2+4+MQTTClientIDLength+2+MQTTUsernameLength+2+MQTTPasswordLength+2;
    
    X=datalength;
    do
    {
        encodedByte = X%128;
        X = X/128;
        // if there are more data to encode, set the top bit of this byte
        if ( X > 0 ) { encodedByte |= 128; }
        
         Serial.write(encodedByte);
    }
    while ( X > 0 );
    
    
    Serial.write(MQTTProtocolNameLength >> 8);
    Serial.write(MQTTProtocolNameLength & 0xFF);
    Serial.print(MQTTProtocolName);
    
    Serial.write(MQTTLVL); // LVL
    Serial.write(MQTTFlags); // Flags
    Serial.write(MQTTKeepAlive >> 8);
    Serial.write(MQTTKeepAlive & 0xFF);
    
    
    Serial.write(MQTTClientIDLength >> 8);
    Serial.write(MQTTClientIDLength & 0xFF);
    Serial.print(MQTTClientID);
    
    
    Serial.write(MQTTUsernameLength >> 8);
    Serial.write(MQTTUsernameLength & 0xFF);
    Serial.print(MQTTUsername);
    
    
    Serial.write(MQTTPasswordLength >> 8);
    Serial.write(MQTTPasswordLength & 0xFF);
    Serial.print(MQTTPassword);
    
    Serial.write(0x1A);
}
void SendPublishPacket(void)
{
      Serial.print("\r\nAT+CIPSEND\r\n");
      delay(3000);
      
      memset(str,0,250);

      topiclength = sprintf((char*)topic,MQTTTopic);
      datalength = sprintf((char*)str,"%s%u",topic,Counter);      
      delay(1000);
      Serial.write(0x30);
      X=datalength+2;
      do
      {
          encodedByte = X%128;
          X = X/128;
          // if there are more data to encode, set the top bit of this byte
          if ( X > 0 ) { encodedByte |= 128; }          
           Serial.write(encodedByte);
      }
      while ( X > 0 );
      
      Serial.write(topiclength >> 8);
      Serial.write(topiclength & 0xFF);
      Serial.print(str);

      Serial.write(0x1A);
}
void SendSubscribePacket(void)
{
      Serial.print("\r\nAT+CIPSEND\r\n");
      delay(3000);

      memset(str,0,250);
      topiclength = strlen(MQTTTopic);						
      datalength = 2+2+topiclength+1;
      delay(1000);

      Serial.write(0x82);
      X=datalength;
      do
      {
          encodedByte = X%128;
          X = X/128;
          // if there are more data to encode, set the top bit of this byte
          if ( X > 0 ) { encodedByte |= 128; }         
          Serial.write(encodedByte);
      }
      while ( X > 0 );
      Serial.write(MQTTPacketID >> 8);
      Serial.write(MQTTPacketID & 0xFF);
      Serial.write(topiclength >> 8);
      Serial.write(topiclength & 0xFF);
      Serial.print(MQTTTopic);
      Serial.write(MQTTQOS);
      
      Serial.write(0x1A);
      
      
}
void loop()
{
  
    Serial.print("AT+CIPSHUT\r\n");
    delay(2000);  
    Serial.print("AT+CSTT=\"www\",\"\",\"\"\r\n");
    delay(1000);
    Serial.print("AT+CIPMODE=0\r\n");    
    delay(1000);
    Serial.print("AT+CIICR\r\n"); 
    delay(9000);
    //Serial.print("AT+CIFSR\r\n");
    //delay(2000);
    Serial.print("AT+CIPSTART=\"TCP\",\"io.adafruit.com\",\"1883\"\r\n");
    delay(6000); 
    SendConnectPacket();
    delay(5000);
    SendSubscribePacket();
    delay(5000);
    while(1)
    {
          if(Serial.available()>0)
          {
             str[0] = Serial.read();
             Serial.write(str[0]);
             
             if(str[0] == 'N')
                 digitalWrite(led, HIGH);
             if(str[0] == 'F')
                 digitalWrite(led, LOW);
          }
          //delay(5000);        
          //SendPublishPacket();
          //Counter++;
        
    }
}
