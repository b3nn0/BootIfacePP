#include <string>
#include <functional>

#include <WebSocketServerBase.h>

#include <seasocks/Server.h>
#include <seasocks/WebSocket.h>
#include <seasocks/PrintfLogger.h>
#include <seasocks/PageHandler.h>
#include <seasocks/Request.h>
#include <seasocks/Response.h>
#include <seasocks/ResponseWriter.h>
#include <seasocks/util/RootPageHandler.h>
#include <seasocks/util/PathHandler.h>


namespace BootIface {

class SeasocksServer : public WebSocketServerBase, public seasocks::WebSocket::Handler {
public:
    SeasocksServer();
    void startServer(int port);

    virtual void addWsRoute(std::string_view route, std::function<void(WebSocket*)> connectCb) override;
    virtual void addStaticContent(std::string_view route, std::string_view content, bool gzipped) override;


    virtual void onConnect(seasocks::WebSocket* sock) override;
    virtual void onDisconnect(seasocks::WebSocket* sock) override;

private:
    std::unique_ptr<seasocks::Server> _server;
    std::vector<seasocks::WebSocket*> _connections;
};



class SeasocksStaticResponse : public seasocks::Response {
public:
    SeasocksStaticResponse(std::string_view s, bool isGzipped) {
        _response = s;
        _gzipped = isGzipped;
    }

    virtual void handle(std::shared_ptr<seasocks::ResponseWriter> writer) {
        writer->begin(seasocks::ResponseCode::Ok);
        if (_gzipped) {
            writer->header("Content-Encoding", "gzip");
        }
        writer->payload(_response.c_str(), _response.size());
        writer->finish(false);
    }

    // Called when the connection this response is being sent on is closed.
    virtual void cancel() {
    }
    virtual ~SeasocksStaticResponse() = default;
private:
    std::string _response;
    bool _gzipped;
};


class SeasockStaticPageHandler : public seasocks::CrackedUriPageHandler {
public:
    SeasockStaticPageHandler(std::string_view s, bool isGzipped) 
    : _resp(std::make_shared<SeasocksStaticResponse>(s, isGzipped)) {}

    virtual std::shared_ptr<seasocks::Response> handle(const seasocks::CrackedUri& uri, const seasocks::Request& request) override {
        if (uri.path().size() == 0)
            return _resp;
        return nullptr;
    }
private:

    std::shared_ptr<SeasocksStaticResponse> _resp;
};


class SeasocksWebSocket : public WebSocket {
public:
    SeasocksWebSocket(seasocks::WebSocket* sock) {
        _sock = sock;
    }
    virtual void setCallbacks(std::function<void(WebSocket*, std::string_view)> onMessage, std::function<void(WebSocket*)> onDisconnect) override {
        _onMessage = onMessage;
        _onDisconnect = onDisconnect;
    }

    virtual void sendMessage(const std::string& message, bool isBinary) override {
        if (isBinary)
            _sock->send(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
        else
            _sock->send(message.data());
    }

    void onData(std::string_view data) {
        if (_onMessage)
            _onMessage(this, data);        
    }

    void onDisconnect() {
        if (_onDisconnect)
            _onDisconnect(this);
    }

    virtual ~SeasocksWebSocket() = default;
private:
    seasocks::WebSocket* _sock;
    std::function<void(WebSocket*, std::string_view)>  _onMessage;
    std::function<void(WebSocket*)> _onDisconnect;
};

class SeasockWebSocketHandler : public seasocks::WebSocket::Handler {
public:
    SeasockWebSocketHandler(std::function<void(WebSocket*)> connectCb) {
        _connectCb = connectCb;
    }
    virtual void onConnect(seasocks::WebSocket *socket) override {
        auto s = std::make_shared<SeasocksWebSocket>(socket);
        _conns[socket] = s;
        _connectCb(s.get());
    }

    virtual void onData(seasocks::WebSocket* sock, const char* data) {
        auto s = _conns[sock];
        if (s == nullptr) {
            return;
        }
        s->onData(std::string_view(data));
    }

    virtual void onData(seasocks::WebSocket* sock, const uint8_t* data, size_t size) {
        auto s = _conns[sock];
        if (s == nullptr) {
            return;
        }
        s->onData(std::string_view(reinterpret_cast<const char*>(data), size));
    }

    virtual void onDisconnect(seasocks::WebSocket *sock) {
        auto s = _conns[sock];
        if (s == nullptr)
            return;
        s->onDisconnect();
        _conns.erase(sock);
    }
   
private:
    std::function<void(WebSocket*)>  _connectCb;
    std::unordered_map<seasocks::WebSocket*, std::shared_ptr<SeasocksWebSocket>> _conns;
};


SeasocksServer::SeasocksServer() {
    auto logger = std::make_shared<seasocks::PrintfLogger>();
    _server = std::make_unique<seasocks::Server>(logger);
}

void SeasocksServer::startServer(int port) {
    _server->startListening(port);
    _server->loop();
}


void SeasocksServer::addWsRoute(std::string_view route, std::function<void(WebSocket*)> connectCb) {
    _server->addWebSocketHandler(route.data(), std::make_shared<SeasockWebSocketHandler>(connectCb));
}

void SeasocksServer::addStaticContent(std::string_view route, std::string_view content, bool gzipped) {
    auto root = std::make_shared<seasocks::RootPageHandler>();
    //auto handler = std::make_shared<seasocks::PathHandler>(std::string(route));
    auto contentHandler = std::make_shared<SeasockStaticPageHandler>(content, gzipped);
    //handler->add(contentHandler);
    //root->add(handler);
    root->add(contentHandler);

    _server->addPageHandler(root);
}


void SeasocksServer::onDisconnect(seasocks::WebSocket* sock) {

}
void SeasocksServer::onConnect(seasocks::WebSocket* sock) {

}

}