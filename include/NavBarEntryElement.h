#pragma once
#include <string>
#include <Format.h>

#include "WebElement.h"

namespace BootIface {

class NavSidebarEntryElement : public WebElement {

public:
    NavSidebarEntryElement(WebPageBase* page) : WebElement(page) {
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

    void setLabel(std::string_view label) {
        _label = label;
    }

    const std::string& getLabel() {
        return _label;
    }

    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(
            "<li id=\"{}\" style=\"{}\" class=\"{}\">"
            "<a href=\"#\">{}</a>"
            "</li>", getId(), _style, _classes, _label);
    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

private:
    std::string _style;
    std::string _classes;
    std::string _label;
};

}