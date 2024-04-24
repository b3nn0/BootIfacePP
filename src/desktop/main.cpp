#include <memory>


#include "SeasocksServer.h"
#include "WebPage.h"

using namespace BootIface;

extern std::shared_ptr<WebPage> createDemoUi();

int main(int argc, char* argv[]) {
    SeasocksServer server;
    
    auto p = createDemoUi();

    p->serveOn(server, "/");

    server.startServer(9090);
    
}