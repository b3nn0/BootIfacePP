#pragma once
#include <string>
#include "Format.h"

#include "WebElement.h"

namespace BootIface {

class CardElement : public WebElement {

public:
    CardElement(WebPageBase* page) : WebElement(page) {}


    void setHeader(std::string_view header) {
        _header = header;
    }

    const std::string& getHeader() const {
        return _header;
    }

    void setTitle(std::string_view title) {
        _title = title;
    }



    virtual std::string getHtml(bool recursive) const override {
        std::string headerStr = "";
        return fmt::format(
            "<div id=\"{}\" class=\"{}\">"
            "<div class=\"card\" style=\"{}\">"
            "<h5 class=\"card-header\">{}</h5>"
            "<div class=\"card-body\">"
            "<h5 class=\"card-title\">{}</h5>"
            "<div childcontainer>"
            "{}"
            "</div></div></div></div>", getId(), getColSpecStr(), _style, _header, _title, getChildrenHtml(recursive)
            );
    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

private:
    std::string _header;
    std::string _title;

};

}