#include<WiFi.h>
#include<ESPmDNS.h>
char ssid[]="Engineers";
char pw[]="EmA@6264";
WiFiServer server(80) ; // to make a server node 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.disconnect();
  Serial.println("");
  Serial.println("");
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,pw);
  while(WiFi.status()!=WL_CONNECTED)
  {
   Serial.println(".");
   delay(300); 
  }
  Serial.println("");
  Serial.println("WiFi is connected ");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP()); //the IP of my esp32 module 
  while(!MDNS.begin("esp32"))
  {
    Serial.println("Error setting up MDNS responder");
    delay(1);
  }
  Serial.println("mDNS started");
  server.begin() ;    // start the server
  Serial.println("WebServer started");
  
}

void loop() {
  // put your main code here, to run repeatedly:
 WiFiClient client=server.available();   //making a client variable and checking if there is any available
 if(!client){return;}  //checking the presence of a client and returning until finding one 
 while(!client.available()){delay(1);}  //checking until the client sends data 
 Serial.println(client.readString());
 client.stop(); //stops the client
}
