#pragma once
#include <string>
#include <Format.h>

#include "WebPageBase.h"
#include "WebElement.h"

namespace BootIface {

class TabBarItemElement : public WebElement {
public:
    TabBarItemElement(WebPageBase* page) : WebElement(page) {
    }

    void setLabel(std::string_view label) {
        _label = label;
    }

    const std::string& getLabel() const {
        return _label;
    }

    void setContent(std::shared_ptr<WebElement> content) {
        _content = content;
        _content->addClass("tab-pane");
    }

    std::shared_ptr<WebElement> getContent() {
        return _content;
    }

    void show() {
        _shown = true;
        _content->addClass("active");
    }

    bool isShown() {
        return _shown;
    }


    virtual std::string getHtml(bool recursive) const override {
        std::string target = _content ? _content->getId() : "";
        return fmt::format(R"!!(
<li class="nav-item" role="presentation">
    <button id="{}" class="nav-link {} {}" style="{}" data-bs-toggle="tab" data-bs-target="#{}" type="button" role="tab">{}</button>
</li>
)!!",
            getId(), _shown ? "active" : "", getClassesStr(), getStyle(), target, getLabel());

    }


    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

protected:
    std::string _label;
    bool _shown = false;
    std::shared_ptr<WebElement> _content;
};

}