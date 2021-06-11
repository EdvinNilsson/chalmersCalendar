#pragma once
#include <string>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/regex.hpp>
#include "language.h"

using namespace chalmersCalendar;

enum EventType {SingleDay, FirstDay, LastDay, Period};

class CalendarEvent {
public:
    std::string summary;
    boost::gregorian::date start;
    boost::gregorian::date end;
    CalendarEvent(std::vector<std::string> spans, EventType type, Language language);
};

std::vector<std::string> getUpcomingTerms(std::string html);

void getEvents(const std::string& input, std::vector<CalendarEvent>& events, Language language);
