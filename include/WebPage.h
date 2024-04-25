#pragma once

#include <string>
#include <memory>
#include <set>

#include "WebPageBase.h"
#include "WebSocketServerBase.h"
#include "InternalDivElement.h"
#include "WebElement.h"
#include "Message.h"



namespace BootIface {

class WebPage : public WebPageBase {

public:

    WebPage() {
        _rootContainer = std::make_shared<InternalDivElement>(this, "_rootContainer");
    }

    void serveOn(WebSocketServerBase& server, std::string rootPath) {
        server.addStaticContent(rootPath, bootiface_index_html, true);
        server.addStaticContent(rootPath + "bootstrap.min.js", bootiface_bootstrap_min_js, true);
        server.addStaticContent(rootPath + "bootstrap.min.css", bootiface_bootstrap_min_css, true);
        std::string wspath = std::string(rootPath) + "BootIface";
        server.addWsRoute(wspath, [this](WebSocket* socket) {
            _connections.insert(socket);
            socket->setCallbacks([this](auto sock, auto data) {
                handleFrontendEvent(data);
            }, [this](auto sock) {
                _connections.erase(sock);
            });
            handleNewClient(socket);
        });
    }

    std::string_view getTitle() {
        return _title;
    }

    WebPage* setTitle(std::string_view title) {
        _title = title;
        return this;
    }

    std::shared_ptr<WebElement> getRootContainer() {
        return _rootContainer;
    }

    WebElement* getElementById(std::string_view id) {
        if (_rootContainer == nullptr)
            return nullptr;
        return _rootContainer->getElementById(id);
    }


    virtual bool hasClientsConnected() const {
        return _connections.size() > 0;
    }

    virtual void pushMessage(const Message& msg) {
        if (_connections.size() == 0)
            return;
        std::string json = msg.asJson();
        for (WebSocket* sock : _connections)
            sock->sendMessage(json, false);
    }


private:
    void handleNewClient(WebSocket* sock) {
        std::string html = _rootContainer->getHtml(true);
        Message msg(Message::Action::UPDATE, html);
        msg.setElementId("_rootContainer");
        std::string json = msg.asJson();
        sock->sendMessage(json, false);
    }
    void handleFrontendEvent(std::string_view data) {
        Message msg(data);
        auto element = getElementById(msg.getElementId());
        if (element != nullptr)
            element->handleEvent(msg);
    }


    std::string _title;
    std::shared_ptr<WebElement> _rootContainer;
    std::set<WebSocket*> _connections;
};

}