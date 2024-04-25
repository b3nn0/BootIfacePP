#pragma once
#include <string>
#include <functional>
#include <Format.h>

#include "WebPage.h"
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

    void onClick(std::function<void(ButtonElement*)> onClick) {
        _onClick = onClick;
    }

    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(R"!!(
<button id="{}" class="btn btn-primary {} {}" style="{}" onclick="sendEvent(this.id, 'onclick', {{}})">
{}
{}
</button>)!!",
            getId(), getColSpecStr(), getClassesStr(), _style, _label, getChildrenHtml(recursive));
    }

    virtual bool handleEvent(const Message& msg) override {
        if (_onClick && msg.getEventType() == "onclick") {
            _onClick(this);
            return true;
        }
        return false;
    }

protected:
    std::string _label;
    std::function<void(ButtonElement*)> _onClick;

};

}