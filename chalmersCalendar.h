#pragma once
#include <string>
#include "language.h"

namespace chalmersCalendar {
    std::string getICal(Language language = Swedish);
}
