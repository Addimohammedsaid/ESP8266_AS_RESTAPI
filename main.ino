#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>


ESP8266WebServer server(80);

#include "credentials.h"
#include "network.h"
#include "router.h"
#include "server.h"

void setup(void)
{     
    pinMode(2,OUTPUT);     
  
    Serial.begin(115200);    
    initNetwork();
    initServer();
}

void loop(void)
{
    server.handleClient();
}
