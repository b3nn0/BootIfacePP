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
        ArduinoJson::deserializeJson(_doc, text.data());
        if (_doc.containsKey("id"))
            _elemId = _doc["id"].as<std::string>();
        if (_doc.containsKey("eventType"))
            _eventType = _doc["eventType"].as<std::string>();
        if (_doc.containsKey("eventData"))
            _eventData = _doc["eventData"].as<ArduinoJson::JsonObject>();
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

    const ArduinoJson::JsonObject& getEventData() const {
        return _eventData;
    }

    std::string_view getEventType() const {
        return _eventType;
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
    ArduinoJson::JsonDocument _doc;
    ArduinoJson::JsonObject _eventData;
    Action _action;
    std::string _elemId = "";
    std::string _data;
    std::string _eventType;
    
};

}