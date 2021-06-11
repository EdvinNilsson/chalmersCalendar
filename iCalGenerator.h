#pragma once
#include "calendarEvent.h"

using namespace chalmersCalendar;

std::string generateICal(const std::vector<CalendarEvent>& events, Language language);
