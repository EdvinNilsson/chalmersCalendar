#include "chalmersCalendar.h"
#include "webRequest.h"
#include "iCalGenerator.h"

const std::string server = "student.portal.chalmers.se";
const std::string swedish_path = "/sv/chalmersstudier/Sidor/Lasarstider.aspx";
const std::string english_path = "/en/chalmersstudies/Pages/TheAcademicYear.aspx";

using namespace chalmersCalendar;

const std::string &getPath(Language language) {
    switch (language) {
        case Swedish: return swedish_path;
        case English: return english_path;
        default: return swedish_path;
    }
}

std::string chalmersCalendar::getICal(Language language) {
    std::string html = getHTML(server, getPath(language));
    std::vector<CalendarEvent> events;
    getEvents(html, events, language);
    for (const auto &term : getUpcomingTerms(html)) {
        html = getHTML(server, getPath(language) + "?year=" + term);
        getEvents(html, events, language);
    }
    return generateICal(events, language);
}
