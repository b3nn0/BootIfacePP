#include <memory>
#include <unordered_map>
#include <ESPAsyncWebServer.h>
#include <WebSocketServerBase.h>

namespace BootIface {

class EspAsyncWebSocket : public WebSocket {
public:
    EspAsyncWebSocket(AsyncWebSocketClient* client) {
        _client = client;
    }
    virtual void setCallbacks(std::function<void(WebSocket*, std::string_view)> onMessage, std::function<void(WebSocket*)> onDisconnect) override {
        _onMessage = onMessage;
        _onDisconnect = onDisconnect;
    }
    virtual void sendMessage(const std::string& message, bool isBinary) override {
        _client->text(message.c_str(), message.length());
    }

    void callOnDisconnect() {
        if (_onDisconnect)
            _onDisconnect(this);
    }

    void callOnData(uint8_t* data, size_t len) {
        if (_onMessage) {
            _onMessage(this, std::string_view((const char*) data, len));
        }
    }

    virtual ~EspAsyncWebSocket() = default;

private:
    AsyncWebSocketClient* _client;
    std::function<void(WebSocket*, std::string_view)>  _onMessage;
    std::function<void(WebSocket*)> _onDisconnect;
};

class EspAsyncWebSocketServer : public WebSocketServerBase {
public:
    EspAsyncWebSocketServer(AsyncWebServer* server)
    : _server(server) {}
    /**
     * TODO: only one ws route supported by this adapter
    */
    virtual void addWsRoute(std::string_view route, std::function<void(WebSocket*)> connectCb) {
        _ws = std::make_unique<AsyncWebSocket>(route.data());
        _ws->onEvent([this, connectCb](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
            if (type == WS_EVT_CONNECT) {
                auto sock = std::make_shared<EspAsyncWebSocket>(client);
                _conns[client] = sock;
                connectCb(sock.get());
            } else if (type == WS_EVT_DISCONNECT) {
                auto sock = _conns[client];
                if (sock == nullptr)
                    return;
                _conns.erase(client);
                sock->callOnDisconnect();
            } else if (type == WS_EVT_DATA) {
                auto sock = _conns[client];
                if (sock == nullptr)
                    return;
                sock->callOnData(data, len);
            }
        });
        _server->addHandler(_ws.get());
    }
    virtual void addStaticContent(std::string_view route, std::string_view content, bool gzipped) {
        _server->on(route.data(), HTTP_GET, [gzipped, content, route](AsyncWebServerRequest *request) {
            auto response = request->beginResponse_P(200, "text/html", (uint8_t*)content.data(), content.length());
            if (gzipped) {
                response->addHeader("Content-Encoding", "gzip");
            }
            request->send(response);
        });
    }


    virtual ~EspAsyncWebSocketServer() = default;

private:
    AsyncWebServer* _server;
    std::unique_ptr<AsyncWebSocket> _ws;
    std::unordered_map<AsyncWebSocketClient*, std::shared_ptr<EspAsyncWebSocket>> _conns;
};

}