#pragma once

#include <string>
#include <memory>

#include "Message.h"


extern const std::string_view bootiface_index_html;
extern const size_t bootiface_index_html_orig_len;
extern const std::string_view bootiface_bootstrap_min_css;
extern const size_t bootiface_bootstrap_min_css_orig_len;
extern const std::string_view bootiface_bootstrap_min_js;
extern const size_t bootiface_bootstrap_min_js_orig_len;

namespace BootIface {

class WebPageBase {

public:
    virtual void pushMessage(const Message& msg) = 0;
    virtual bool hasClientsConnected() const = 0;
};

}