#pragma once

#include <string>
#include <ArduinoJson.hpp>

namespace BootIface {


class Message {
public:
    enum class Action {
        ADD_CHILD,
        UPDATE,
        JS,
        EVENT
    };

    Message(Action action, const std::string& data) {
        _action = action;
        _data = data;
    }

    Message(std::string_view text) {
        ArduinoJson::JsonDocument doc;
        ArduinoJson::deserializeJson(doc, text.data());
        if (doc.containsKey("id"))
            _elemId = doc["id"].as<std::string>();
        if (doc.containsKey("eventType"))
            _eventType = doc["eventType"].as<std::string>();
        if (doc.containsKey("eventData"))
            _eventData = doc["eventData"].as<ArduinoJson::JsonObject>();
    }

    Action getAction() const {
        return _action;
    }

    void setAction(Action action) {
        _action = action;
    }

    void setElementId(std::string_view id) {
        _elemId = id;
    }

    const std::string& getElementId() const {
        return _elemId;
    }

    void setData(const std::string& data) {
        _data = data;
    }

    std::string_view getData() const {
        return _data;
    }

    std::string_view getEventType() const {
        return _eventType;
    }

    const ArduinoJson::JsonObject& getEventData() {
        return _eventData;
    }

    std::string asJson() const {
        ArduinoJson::JsonDocument doc;
        if (_action == Action::ADD_CHILD)
            doc["action"] = "addChild";
        else if (_action == Action::UPDATE)
            doc["action"] = "update";
        else if (_action == Action::JS)
            doc["action"] = "js";
        doc["data"] = _data;
        doc["id"] = _elemId;
        std::string result;
        ArduinoJson::serializeJson(doc, result);
        return result;
    }
    
private:
    Action _action;
    std::string _elemId = "";
    std::string _data;
    std::string _eventType;
    ArduinoJson::JsonObject _eventData;
    
};

}