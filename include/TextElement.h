#pragma once
#include <string>
#include "Format.h"

#include "WebElement.h"

namespace BootIface {

class TextElement : public WebElement {

public:
    TextElement(WebPageBase* page) : WebElement(page) {
    }

    void setText(std::string_view text) {
        _text = text;
    }

    const std::string& getText() const {
        return _text;
    }



    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(R"!!(
<span id="{}" class="{} {}" style="{}">
{}
{}
</span>)!!",
            getId(), getColSpecStr(), getClassesStr(), _style, _text, getChildrenHtml(recursive));
    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

protected:
    std::string _text;
};

}