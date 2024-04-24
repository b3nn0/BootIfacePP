#pragma once
#include <string>
#include "Format.h"

#include "WebElement.h"

namespace BootIface {

class DivElement : public WebElement {

public:
    DivElement(WebPageBase* page) : WebElement(page) {
        setCols(0);
    }

    void setClasses(std::string_view classes) {
        _classes = classes;
    }

    void setStyle(std::string_view style) {
        _style = style;
    }

    const std::string& getStyle() const {
        return _style;
    }



    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(
            "<div id=\"{}\" class=\"{} {}\" style=\"{}\">"
            "{}"
            "</div>", getId(), getColSpecStr(), _classes, _style, getChildrenHtml(recursive));
    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

protected:
    std::string _style;
    std::string _classes;
};

}