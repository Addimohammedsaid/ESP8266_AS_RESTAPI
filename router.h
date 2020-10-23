/******************************************************/
/*                Preprocesseur
  /******************************************************/
void router();
void handleNotFound();
void setCrossOrigin();
void sendCrossOriginHeader();
JsonObject getBody();
void get_pin();
void set_pin();
void getSettings();

/******************************************************/
/*                Define Routes
  /******************************************************/
void router()
{
  // Home Route
  server.on("/", HTTP_GET, []() {
    server.send(200, F("text/html"), F("Welcome to the REST Web Server"));
  });

  // Pins Route
  server.on(F("/pin"), HTTP_GET, get_pin);
  server.on(F("/pin"), HTTP_POST, set_pin);


  // ESP Settings Route
  server.on(F("/settings"), HTTP_GET, getSettings);
  //Access by browser
  server.on(F("/settings"), HTTP_OPTIONS, sendCrossOriginHeader);
}

/******************************************************/
/*                Mange not found url
  /******************************************************/
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

/******************************************************/
/*                CROSS ORIGIN
  /******************************************************/
void setCrossOrigin()
{
  server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  server.sendHeader(F("Access-Control-Max-Age"), F("600"));
  server.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
  server.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
};

/******************************************************/
/*                CROSS ORIGIN
  /******************************************************/
void sendCrossOriginHeader()
{
  Serial.println(F("sendCORSHeader"));
  server.sendHeader(F("access-control-allow-credentials"), F("false"));
  setCrossOrigin();
  server.send(204);
}
/******************************************************/
/*                Pins Route
  /******************************************************/
void get_pin()
{
  // set Cross Origin
  setCrossOrigin();

  // Allocate a temporary JsonDocument with 512 byte
  DynamicJsonDocument doc(512);

  for (int i=1;i<10;i++)
  {    
    doc["pin "+String(i)] = digitalRead(i);
  }

  Serial.print(F("Stream..."));

  String buf;
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);

  Serial.print(F("done."));
}

void set_pin()
{
  // set Cross Origin
  setCrossOrigin();

  DynamicJsonDocument res(512);

  String postBody = server.arg("plain");
  Serial.println(postBody);

  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, postBody);

  if (error) {
    // if the file didn't open, print an error:
    Serial.print(F("Error parsing JSON "));
    Serial.println(error.c_str());

    String msg = error.c_str();

    server.send(400, F("text/html"), "Error in parsin json body! <br>" + msg);
  }
  else
  {
    JsonObject postObj = doc.as<JsonObject>();    

   if (postObj.containsKey("state") && server.method() == HTTP_POST)
    {                       
      const int pin = postObj["pin"];

      const int state = postObj["state"];

      Serial.println(pin);
      Serial.println(state);
      
      digitalWrite(pin, state);

      res["status"] = "OK";

      Serial.print(F("Stream..."));
      String buf;
      serializeJson(res, buf);

      server.send(201, F("application/json"), buf);
      Serial.print(F("done."));

    } else {
      res["status"] = "ERROR";
      res["message"] = F("No data found, or incorrect!");

      Serial.print(F("Stream..."));
      String buf;
      serializeJson(res, buf);

      server.send(400, F("application/json"), buf);
      Serial.print(F("done."));

    }
  }
}


/******************************************************/
/*                Settings Route
  /******************************************************/
void getSettings() {
  // add cross
  setCrossOrigin();
  // Allocate a temporary JsonDocument with 512 byte
  DynamicJsonDocument doc(512);

  doc["ip"] = WiFi.localIP().toString();
  doc["gw"] = WiFi.gatewayIP().toString();
  doc["nm"] = WiFi.subnetMask().toString();
  doc["signalStrengh"] = WiFi.RSSI();
  doc["chipId"] = ESP.getChipId();
  doc["flashChipId"] = ESP.getFlashChipId();
  doc["flashChipSize"] = ESP.getFlashChipSize();
  doc["flashChipRealSize"] = ESP.getFlashChipRealSize();
  doc["freeHeap"] = ESP.getFreeHeap();


  Serial.print(F("Stream..."));
  String buf;
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);
  Serial.print(F("done."));
}


/******************************************************/
/*                get Post
  /******************************************************/
JsonObject getBody()
{
  String postBody = server.arg("plain");
  Serial.println(postBody);

  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, postBody);

  if (error) {
    // if the file didn't open, print an error:
    Serial.print(F("Error parsing JSON "));
    Serial.println(error.c_str());

    String msg = error.c_str();

    server.send(400, F("text/html"), "Error in parsin json body! <br>" + msg);
  }

  else
    return doc.as<JsonObject>();
}
