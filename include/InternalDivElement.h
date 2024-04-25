#pragma once
#include <string>

#include "WebElement.h"
#include "DivElement.h"

namespace BootIface {

class InternalDivElement : public DivElement {

public:
    InternalDivElement(WebPageBase* page, std::string_view id) : DivElement(page) {
        setBreakAndCols(BreakPoint::None, 0);
        _id = id;
    }

    virtual std::string getId() const override {
        return _id;
    }


private:
    std::string _id;
};

}