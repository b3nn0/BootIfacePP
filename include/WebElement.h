#pragma once

#include <string>
#include <memory>
#include <vector>

#include "WebPageBase.h"
#include "Message.h"


namespace BootIface {

class WebElement : public std::enable_shared_from_this<WebElement> {
public:
    enum class BreakPoint : uint8_t {
        XSmall, Small, Medium, Large, XLarge, XXLarge, None
    };

    WebElement(WebPageBase* page) 
    : _page(page) {}

    virtual ~WebElement() = default;


    virtual WebElement* addChild(std::shared_ptr<WebElement> child) {
        child->_parent = this;
        _children.push_back(child);
        if (_page->hasClientsConnected()) {
            Message msg(Message::Action::ADD_CHILD, child->getHtml(true));
            msg.setElementId(getId());
            _page->pushMessage(msg);
        }
        return this;
    }

    
    virtual std::string getHtml(bool recursive) const = 0;


    virtual std::string getId() const {
        return "_" + std::to_string(reinterpret_cast<long>(this));
    }

    virtual std::shared_ptr<WebElement> getElementById(std::string_view id) {
        if (id == getId())
            return shared_from_this();
        for (auto& ele : _children) {
            std::shared_ptr<WebElement> result = ele->getElementById(id);
            if (result != nullptr)
                return result;
        }
        return nullptr;
    }

    virtual bool handleEvent(const Message& msg) = 0;

    virtual std::string getChildrenHtml(bool recursive) const {
        std::string result;
        for (auto elem : _children) {
            if (recursive) {
                result += elem->getHtml(recursive);
            } else {
                result += "<div placeholder=\"" + elem->getId() + "\"/>";
            }
        }
        return result;
    }

    virtual void push() {
        Message msg(Message::Action::UPDATE, getHtml(false));
        msg.setElementId(getId());
        _page->pushMessage(msg);
    }

    void setBreakAndCols(BreakPoint b, uint8_t cols) {
        _breakPoint = b;
        _cols = cols;
    }

    void setCols(uint8_t cols) {
        _cols = cols;
    }

    void setBreakPoint(BreakPoint b) {
        _breakPoint = b;
    }

protected:
    WebPageBase* _page;
    std::string getColSpecStr() const {
        if (_cols == 0 || _breakPoint == BreakPoint::None)
            return "";
        switch (_breakPoint) {
        case BreakPoint::XSmall:
            return "col-xs-" + std::to_string(_cols);
            break;
        case BreakPoint::Small:
            return "col-sm-" + std::to_string(_cols);
            break;
        case BreakPoint::Medium:
            return "col-md-" + std::to_string(_cols);
            break;
        case BreakPoint::Large:
            return "col-lg-" + std::to_string(_cols);
            break;
        case BreakPoint::XLarge:
            return "col-xl-" + std::to_string(_cols);
            break;
        case BreakPoint::XXLarge:
            return "col-xxl-" + std::to_string(_cols);
            break;
        }
        return "";
    }

private:
    WebElement* _parent = nullptr;
    std::vector<std::shared_ptr<WebElement>> _children;
    uint8_t _cols = 4;
    BreakPoint _breakPoint = BreakPoint::Small;
};

}