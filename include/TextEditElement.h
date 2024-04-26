#pragma once
#include <string>
#include <functional>
#include <Format.h>

#include "WebPage.h"
#include "WebElement.h"

namespace BootIface {

class TextEditElement : public WebElement {

public:
    TextEditElement(WebPage* page) : WebElement(page) {}

    void setLabel(std::string_view label) {
        _label = label;
    }

    const std::string& getLabel() const {
        return _label;
    }

    void setValue(std::string_view value) {
        _value = value;
    }

    const std::string& getValue() const {
        return _value;
    }

    void setHint(std::string_view hint) {
        _hint = hint;
    }

    const std::string& getHint() const {
        return _hint;
    }

    void onChange(std::function<void(TextEditElement*, const std::string& value)> onChange) {
        _onChange = onChange;
    }

    void onInput(std::function<void(TextEditElement*, const std::string& value)> onInput) {
        _onInput = onInput;
    }

    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(R"!!(
<div id="{0}" class="{1} {2}" style="{3}">
    <label for="{0}_fld" class="form-label">{4}</label>
    <input type="text" class="form-control" id="{0}_fld" value="{5}" placeholder="{6}" 
        onchange="sendEvent(this.parentNode.id, 'onchange', {{value: this.value}})"
        oninput="sendEvent(this.parentNode.id, 'oninput', {{value: this.value}})">{7}</input>
</div>
)!!",
            getId(), getColSpecStr(), getClassesStr(), _style, _label, _value, _hint, getChildrenHtml(recursive));
    }

    virtual bool handleEvent(const Message& msg) override {
        _value = msg.getEventData()["value"].as<const char*>();
        if (_onChange && msg.getEventType() == "onchange") {
            _onChange(this, _value);
            return true;
        } else if (_onInput && msg.getEventType() == "oninput") {
            _onInput(this, _value);
            return true;
        }
        return false;
    }

protected:
    std::string _label;
    std::string _value;
    std::string _hint;
    std::function<void(TextEditElement*, const std::string& value)> _onChange;
    std::function<void(TextEditElement*, const std::string& value)> _onInput;

};

}