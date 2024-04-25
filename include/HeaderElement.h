#pragma once
#include <string>
#include "Format.h"

#include "WebElement.h"

namespace BootIface {

class HeaderElement : public WebElement {

public:
    HeaderElement(WebPageBase* page) : WebElement(page) {
        setBreakAndCols(BreakPoint::None, 0);
    }

    void setTitle(std::string_view title) {
        _title = title;
    }



    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(R"!!(
<header id="{}" class="navbar sticky-top bg-light flex-md-nowrap p-0 {}" style="{}">
<span class="navbar-brand col-md-3 col-lg-2 me-0 px-3">{}</span>
{}
</header>)!!",
            getId(), getClassesStr(), _style, _title, getChildrenHtml(recursive));
    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

protected:
    std::string _title;

};

}