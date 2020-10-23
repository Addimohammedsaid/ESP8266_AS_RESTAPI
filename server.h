/******************************************************/
/*                Init Server               
/******************************************************/
void initServer(){
  // Set server routing
  router();
    
  // Set not found response
  server.onNotFound(handleNotFound);
  
  // Start server
  server.begin();
  
  Serial.println("HTTP server started");

}
