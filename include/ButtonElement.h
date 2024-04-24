#pragma once
#include <string>
#include <functional>
#include <Format.h>

#include "WebElement.h"

namespace BootIface {

class ButtonElement : public WebElement {

public:
    ButtonElement(WebPage* page) : WebElement(page) {}

    void setLabel(std::string_view label) {
        _label = label;
    }

    const std::string& getLabel() const {
        return _label;
    }

    void setStyle(std::string_view style) {
        _style = style;
    }

    const std::string& getStyle() const {
        return _style;
    }

    void onClick(std::function<void(ButtonElement*)> onClick) {
        _onClick = onClick;
    }

    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(
            "<button id=\"{}\" class=\"btn btn-primary {}\" style=\"{}\" onclick=\"sendEvent(this.id, 'onclick', {{}})\">"
            "{}"
            "{}"
            "</button>", getId(), getColSpecStr(), _style, _label, getChildrenHtml(recursive));
    }

    virtual bool handleEvent(const Message& msg) override {
        if (_onClick && msg.getEventType() == "onclick") {
            _onClick(this);
            return true;
        }
        return false;
    }

private:
    std::string _label;
    std::string _style;
    std::function<void(ButtonElement*)> _onClick;

};

}