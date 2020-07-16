#include "WebServer.h"

#ifdef ESP8266

#include <FS.h>

#else

#include <SPIFFS.h>

#endif



String request_handler(const String& var)
{
  Serial.println(var);
  if(var == "STATE")
  {
    if(digitalRead(ledPin))
    {
      ledState = "ON";
    }
    else
    {
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}

void WebServer::start() 
{
    m_webServer.on("/index", HTTP_ANY, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.htm");
  });
  
}
