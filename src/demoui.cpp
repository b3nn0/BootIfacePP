#include <WebPage.h>
#include <CardElement.h>
#include <ButtonElement.h>
#include <RowElement.h>
#include <NavBarElement.h>
#include <NavBarEntryElement.h>

#include <Format.h>

using namespace BootIface;

std::shared_ptr<WebPage> createDemoUi() {
    auto p = std::make_shared<WebPage>();

    // Navigation
    auto nav = std::make_shared<NavSidebarElement>(p.get());
    nav->setHeader("Demo");
    for (int i = 0; i < 10; i++) {
        auto entry = std::make_shared<NavSidebarEntryElement>(p.get());
        entry->setLabel("Foo " + std::to_string(i));
        nav->addChild(entry);
    }
    p->getRootElement()->addChild(nav);


    // content

    auto root = std::make_shared<DivElement>(p.get());
    auto boxRow = std::make_shared<RowElement>(p.get());
    auto cardRow = std::make_shared<RowElement>(p.get());
    root->addChild(boxRow);
    root->addChild(cardRow);


    auto lulcard = std::make_shared<CardElement>(p.get());
    lulcard->setHeader("lulbuttons");
    lulcard->setTitle("hehehe");
    lulcard->setBreakAndCols(WebElement::BreakPoint::Small, 6);
    cardRow->addChild(lulcard);

    auto changer = std::make_shared<CardElement>(p.get());
    auto changerbtn = std::make_shared<ButtonElement>(p.get());
    changerbtn->setLabel("foo");
    changer->setHeader("changer");
    changer->addChild(changerbtn);
    changer->setBreakAndCols(WebElement::BreakPoint::Small, 6);
    cardRow->addChild(changer);


    for (int i = 0; i < 3; i++) {
        auto button = std::make_shared<ButtonElement>(p.get());
        boxRow->addChild(button);
        button->setBreakAndCols(WebElement::BreakPoint::Small, 4);
        button->setLabel("Click me");
        button->onClick([p, lulcard, boxRow, button, changer](ButtonElement* elem) {
            static int color = 0;
            color += 10;
            if (elem->getStyle() == "color:red")
                elem->setStyle("color:white");
            else
                elem->setStyle("color:red");
            elem->push();

            auto b = std::make_shared<ButtonElement>(p.get());
            b->setBreakAndCols(WebElement::BreakPoint::Small, 4);
            b->setLabel("lul");
            b->setStyle(fmt::format("color:rgb({}, 0, 0)", color));
            lulcard->addChild(b);

            changer->setHeader(changer->getHeader() + "a");
            changer->push();

            //p.updateElement(box.get());
        });
    }

    p->getRootElement()->addChild(root);

    return p;
}