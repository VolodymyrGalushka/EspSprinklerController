#include "WebServer.h"

#ifdef ESP8266

#include <FS.h>

#else

#include <SPIFFS.h>

#endif

void WebServer::start() 
{
  
    m_webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false);
    });

    m_webServer.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });



  m_webServer.onNotFound([](AsyncWebServerRequest *request){ request->send(404); });
  // m_webServer.onFileUpload(onUpload);
  m_webServer.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){});

  m_webServer.begin();

}
