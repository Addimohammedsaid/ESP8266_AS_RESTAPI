/******************************************************/
/*                Preprocesseur
  /******************************************************/
void initNetwork() {

  /* Put IP Address details */
  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  Serial.println(WiFi.localIP());
}
