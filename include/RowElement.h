#pragma once
#include <string>
#include <Format.h>

#include "WebElement.h"

namespace BootIface {

class RowElement : public WebElement {

public:
    RowElement(WebPageBase* page) : WebElement(page) {
    }


    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(
            "<div id=\"{}\" class=\"row\" style=\"{}\">"
            "{}"
            "</div>", getId(), _style, getChildrenHtml(recursive));
    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

private:
};

}