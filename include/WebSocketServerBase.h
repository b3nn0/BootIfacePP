#pragma once

#include <string>
#include <functional>
#include <memory>

namespace BootIface {

class WebSocket {
public:
    virtual void setCallbacks(std::function<void(WebSocket*, std::string_view)> onMessage, std::function<void(WebSocket*)> onDisconnect) = 0;
    virtual void sendMessage(const std::string& message, bool isBinary) = 0;

    virtual ~WebSocket() = default;
};

class WebSocketServerBase {
public:
    virtual void addWsRoute(std::string_view route, std::function<void(WebSocket*)> connectCb) = 0;
    virtual void addStaticContent(std::string_view route, std::string_view content, bool gzipped) = 0;
    virtual ~WebSocketServerBase() = default;
};

}