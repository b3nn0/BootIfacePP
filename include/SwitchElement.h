#pragma once
#include <string>
#include <functional>
#include <Format.h>

#include "WebPage.h"
#include "WebElement.h"

namespace BootIface {

class SwitchElement : public WebElement {

public:
    SwitchElement(WebPage* page) : WebElement(page) {}

    void setLabel(std::string_view label) {
        _label = label;
    }

    const std::string& getLabel() const {
        return _label;
    }

    void setValue(bool value) {
        _checked = value;
    }

    bool getValue() const {
        return _checked;
    }

    void onChange(std::function<void(SwitchElement*, bool on)> onChange) {
        _onChange = onChange;
    }

    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(R"!!(
<div id="{0}" class="form-check form-switch {1} {2}" style="{3}">
    <label class="form-check-label" for="{0}_sw">{5}{6}</label>
    <input class="form-check-input" type="checkbox" id="{0}_sw" {4} onchange="sendEvent(this.parentNode.id, 'onchange', {{value: this.checked}})">
</div>
)!!",
            getId(), getColSpecStr(), getClassesStr(), _style, _checked ? "checked" : "", _label, getChildrenHtml(recursive));
    }

    virtual bool handleEvent(const Message& msg) override {
        _checked = msg.getEventData()["value"].as<bool>();
        if (_onChange && msg.getEventType() == "onchange") {
            _onChange(this, _checked);
            return true;
        }
        return false;
    }

protected:
    std::string _label;
    bool _checked = false;
    std::function<void(SwitchElement*, bool on)> _onChange;

};

}