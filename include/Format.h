// libfmt has name clash with ArduinoJson.. import this instead
// Hopefully we can get rid of this once most toolchains support std::format

#undef B1
#include <fmt/core.h>
#define B1 1