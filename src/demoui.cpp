#include <WebPage.h>
#include <CardElement.h>
#include <ButtonElement.h>
#include <RowElement.h>
#include <HeaderElement.h>
#include <TextElement.h>
#include <TabBarElement.h>
#include <TabBarItemElement.h>
#include <SwitchElement.h>
#include <TextEditElement.h>
#include <ContainerElement.h>

#include <Format.h>

using namespace BootIface;

std::shared_ptr<WebPage> createDemoUi() {
    auto p = std::make_shared<WebPage>();


    
    // Page header
    auto header = std::make_shared<HeaderElement>(p.get());
    header->setTitle("Demo Page");
    header->addClass("shadow");
    p->getRootContainer()->addChild(header);

    // Navigation
    auto nav = std::make_shared<TabBarElement>(p.get());
    nav->setCentered(true);
    nav->setBreakAndCols(WebElement::BreakPoint::None, 0);
    for (int i = 0; i < 10; i++) {
        auto tab = std::make_shared<TabBarItemElement>(p.get());
        auto content = std::make_shared<ContainerElement>(p.get(), false);
        tab->setContent(content);
        tab->setLabel("Foo " + std::to_string(i));
        auto text = std::make_shared<TextElement>(p.get());
        text->setText("tab #" + std::to_string(i));
        content->addChild(text);
        nav->addTab(tab);
    }
    //header->addChild(nav);
    p->getRootContainer()->addChild(nav);

    // Navigation
    /*auto nav = std::make_shared<NavBarElement>(p.get());
    nav->setHeader("Demo");
    for (int i = 0; i < 10; i++) {
        auto entry = std::make_shared<NavBarEntryElement>(p.get());
        entry->setLabel("Foo " + std::to_string(i));
        nav->addChild(entry);
    }
    p->getRootContainer()->addChild(nav);*/


    // content

    auto root = std::make_shared<ContainerElement>(p.get(), true);
    root->addClass("container");
    root->setBreakAndCols(WebElement::BreakPoint::None, 0);
    auto boxRow = std::make_shared<RowElement>(p.get());
    auto cardRow = std::make_shared<RowElement>(p.get());
    root->addChild(boxRow);
    root->addChild(cardRow);


    auto lulcard = std::make_shared<CardElement>(p.get());
    lulcard->setHeader("funnycard");
    lulcard->setTitle("hehehe");
    lulcard->setBreakAndCols(WebElement::BreakPoint::Small, 6);
    cardRow->addChild(lulcard);

    auto sw = std::make_shared<SwitchElement>(p.get());
    sw->setLabel("switcheroo");
    sw->setValue(true);
    sw->onChange([nav, p](auto sw, auto val) {
        auto txt = std::make_shared<TextElement>(p.get());
        txt->setText(std::to_string(val));
        nav->getTab(1)->getContent()->addChild(txt);
    });
    lulcard->addChild(sw);

    auto txt = std::make_shared<TextEditElement>(p.get());
    txt->setHint("hint");
    txt->setLabel("label");
    txt->onChange([](auto elem, auto value) {
        elem->setValue(value + value);
        elem->push();
    });
    lulcard->addChild(txt);

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

    p->getRootContainer()->addChild(root);

    return p;
}