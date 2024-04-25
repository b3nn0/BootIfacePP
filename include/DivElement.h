#pragma once
#include <string>
#include "Format.h"

#include "WebElement.h"

namespace BootIface {

class DivElement : public WebElement {

public:
    DivElement(WebPageBase* page, bool fullWidth = false) : WebElement(page) {
        if (fullWidth)
            setBreakAndCols(BreakPoint::None, 0);
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