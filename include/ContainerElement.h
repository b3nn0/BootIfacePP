#pragma once
#include <string>
#include "Format.h"

#include "WebElement.h"
#include "DivElement.h"

namespace BootIface {

class ContainerElement : public DivElement {

public:
    ContainerElement(WebPageBase* page, bool fullWidth = false) : DivElement(page) {
        setBreakAndCols(BreakPoint::None, 0);
        if (fullWidth)
            addClass("container-fluid");
        else
            addClass("container");
            
    }

    virtual bool handleEvent(const Message& msg) override {
        return false;
    }

};

}