#include "iCalGenerator.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

const std::string calendarSwedishName = "Chalmers - Datum och tider för läsåret";
const std::string calendarEnglishName = "Chalmers - The academic year - dates and times";

std::string generateICal(const std::vector<CalendarEvent>& events, Language language) {
    std::stringstream ss;
    ss
    << "BEGIN:VCALENDAR\r\n"
    << "VERSION:2.0\r\n"
    << "PRODID:-//Chalmers\\\\\\, //Chalmers//EN\r\n"
    << "METHOD:PUBLISH\r\n"
    << "X-WR-CALNAME:" << (language == Swedish ? calendarSwedishName : calendarEnglishName) << "\r\n"
    << "CALSCALE:GREGORIAN\r\n";

    std::string stamp = boost::posix_time::to_iso_string(boost::posix_time::second_clock::universal_time());

    boost::mt19937 ran;
    boost::uuids::basic_random_generator<boost::mt19937> gen(&ran);

    for (const auto& event : events) {
        ss
        << "BEGIN:VEVENT\r\n"
        << "DTSTART:" << boost::gregorian::to_iso_string(event.start) << "\r\n"
        << "DTEND:" << boost::gregorian::to_iso_string(event.end) << "\r\n"
        << "DTSTAMP:" << stamp << "\r\n"
        << "UID:" << boost::uuids::to_string(gen()) << "\r\n"
        << "SUMMARY:" << event.summary << "\r\n"
        << "END:VEVENT\r\n";
    }

    ss << "END:VCALENDAR";
    return ss.str();
}
