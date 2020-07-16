#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <ESPAsyncWebServer.h>

class WebServer
{
    public:
        WebServer() = default;
        ~WebServer() = default;

        void start();

    private:

        AsyncWebServer m_webServer{AsyncWebServer(80)};
};


#endif // __WEBSERVER_H__