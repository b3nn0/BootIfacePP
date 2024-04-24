#pragma once
#include <string>
#include <Format.h>

#include "WebElement.h"

namespace BootIface {

class NavSidebarElement : public WebElement {

public:
    NavSidebarElement(WebPageBase* page) : WebElement(page) {
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

    void setHeader(std::string_view header) {
        _header = header;
    }

    const std::string& getHeader() {
        return _header;
    }



    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(
            "<nav id=\"{}\">"
            "<div class=\"sidebar-header\"><h3>{}</h3></div>"
            "<ul class=\"list-unstyled components\">"
            "{}"
            "</ul>"
            "</nav>", getId(), _header, getChildrenHtml(recursive));
    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

private:
    std::string _style;
    std::string _classes;
    std::string _header;
};

}