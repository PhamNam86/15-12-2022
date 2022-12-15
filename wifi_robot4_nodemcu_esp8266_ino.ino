#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char* ssid = "Node_MCU CAR";
//const char* host = "NodeMCU";

ESP8266WebServer server(80);

void setup(){
  
  Serial.begin(115200);

  // Connecting WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  // Starting WEB-server
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
     
}
void loop(){
  
  server.handleClient();
  //String command = server.arg("State");
  //if (command == "A") smart();
  delay(10);
}
void HTTP_handleRoot(void) 
{
if( server.hasArg("State") ){
      Serial.println(server.arg("State"));
  }
  server.send (200,"text/html","" );
  delay(1);
}