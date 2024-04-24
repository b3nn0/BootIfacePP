#pragma once
#include <string>

#include "WebElement.h"
#include "DivElement.h"

namespace BootIface {

class InternalDivElement : public DivElement {

public:
    InternalDivElement(WebPageBase* page, std::string_view id, std::string_view classes) : DivElement(page) {
        setCols(0);
        _id = id;
        _classes = classes;
    }

    virtual std::string getId() const override {
        return _id;
    }


private:
    std::string _id;
};

}