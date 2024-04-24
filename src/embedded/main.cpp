#include <memory>

#include <EspAsyncWebSocketServer.h>
#include <WebPage.h>

#include <WiFi.h>

using namespace BootIface;


extern std::shared_ptr<WebPage> createDemoUi();


std::shared_ptr<WebPage> ui;
AsyncWebServer server(80);
EspAsyncWebSocketServer serverAdapter(&server);


void setup() {
    Serial.begin(115200);


    WiFi.softAP("bootiface");
    ui = createDemoUi();
    ui->serveOn(serverAdapter, "/");
    server.begin();
}


void loop() {
}