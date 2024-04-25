#pragma once

#include <string>
#include <memory>
#include <vector>
#include <numeric>
#include <unordered_set>

#include "WebPageBase.h"
#include "Message.h"


namespace BootIface {

class WebElement {
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

    virtual WebElement* getElementById(std::string_view id) {
        if (id == getId())
            return this;
        for (auto& ele : _children) {
            WebElement* result = ele->getElementById(id);
            if (result != nullptr)
                return result;
        }
        return nullptr;
    }

    virtual bool handleEvent(const Message& msg) {
        return false;
    };

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

    virtual void push(bool recursive=false) {
        Message msg(Message::Action::UPDATE, getHtml(recursive));
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

    virtual void setStyle(std::string_view style) {
        _style = style;
    }

    virtual const std::string& getStyle() const {
        return _style;
    }

    virtual void addClass(std::string_view cls) {
        _classes.insert(std::string(cls));
    }

    virtual void removeClass(const std::string& cls) {
        _classes.erase(cls);
    }

    virtual const std::unordered_set<std::string>& getClasses() const {
        return _classes;
    }

    virtual std::string getClassesStr() const {
        return std::accumulate(_classes.begin(), _classes.end(), std::string(), [](const std::string& s1, const std::string& s2) { return s1 + " " + s2; });
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

protected:
    std::unordered_set<std::string> _classes;
    std::string _style;

private:
    WebElement* _parent = nullptr;
    std::vector<std::shared_ptr<WebElement>> _children;
    uint8_t _cols = 4;
    BreakPoint _breakPoint = BreakPoint::Small;


};

}