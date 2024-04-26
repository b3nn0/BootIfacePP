#pragma once
#include <string>
#include "Format.h"

#include "WebElement.h"

namespace BootIface {

class DivElement : public WebElement {

public:
    DivElement(WebPageBase* page) : WebElement(page) {
    }

    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(
            "<div id=\"{}\" class=\"{} {}\" style=\"{}\">"
            "{}"
            "</div>", getId(), getColSpecStr(), getClassesStr(), _style, getChildrenHtml(recursive));
    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

};

}