#include "iCalGenerator.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/date_time/local_timezone_defs.hpp>

const std::string calendarSwedishName = "Chalmers - Datum och tider för läsåret";
const std::string calendarEnglishName = "Chalmers - The academic year - dates and times";
const int utcTimeOffset = 1;

std::string dateToString(boost::gregorian::date date) {
    using namespace boost;
    date_time::dst_calc_engine<gregorian::date, posix_time::time_duration, date_time::eu_dst_trait<gregorian::date>> eu_dst;
    posix_time::ptime ptime(date, -posix_time::hours(utcTimeOffset) -
        posix_time::hours(eu_dst.local_is_dst(date, posix_time::hours(0)) == date_time::is_in_dst));
    return to_iso_string(ptime) + "Z";
}

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
        << "DTSTART:" << dateToString(event.start) << "\r\n"
        << "DTEND:" << dateToString(event.end) << "\r\n"
        << "DTSTAMP:" << stamp << "\r\n"
        << "UID:" << boost::uuids::to_string(gen()) << "\r\n"
        << "SUMMARY:" << event.summary << "\r\n"
        << "END:VEVENT\r\n";
    }

    ss << "END:VCALENDAR";
    return ss.str();
}
