#pragma once
#include <string>
#include <vector>
#include <memory>
#include <Format.h>

#include "WebPageBase.h"
#include "WebElement.h"
#include "TabBarItemElement.h"

namespace BootIface {

class TabBarElement : public WebElement {
public:
    TabBarElement(WebPageBase* page) : WebElement(page) {
    }

    void addTab(std::shared_ptr<TabBarItemElement> tab) {
        if (_tabs.size() == 0)
            tab->show();
        _tabs.push_back(tab);
        addChild(tab);
    }

    void setCentered(bool centered) {
        _centered = centered;
    }

    std::shared_ptr<TabBarItemElement> getTab(size_t index) {
        if (index < _tabs.size())
            return _tabs[index];
        return nullptr;
    }

    virtual std::string getHtml(bool recursive) const override {
        return fmt::format(R"!!(
<div id="{}">
    <ul class="nav nav-tabs {} {} {}" role="tablist" style="{}">
    {}
    </ul>
    <div class="tab-content">
    {}
    </div>
</div>)!!",
            getId(), _centered ? "justify-content-center" : "", getColSpecStr(), getClassesStr(), getStyle(), getChildrenHtml(recursive), formatTabContents(recursive));

    }


private:
    std::string formatTabContents(bool recursive) const {
        std::string result;
        for (auto& tab : _tabs) {
            auto content = tab->getContent();
            if (content)
                result += content->getHtml(recursive);
        }
        return result;
    }
    std::vector<std::shared_ptr<TabBarItemElement>> _tabs;
    bool _centered = false;;

};

}