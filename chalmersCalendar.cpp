#include "chalmersCalendar.h"
#include "webRequest.h"
#include "iCalGenerator.h"

const std::string server = "www.student.chalmers.se";
const std::string path = "/sp/academic_year_list";

using namespace chalmersCalendar;

std::string getCookie(Language language) {
    switch (language) {
        case Swedish: return "lang=\"sv\"";
        case English: return "lang=\"en\"";
    }
}

std::string chalmersCalendar::getICal(Language language) {
    std::string html = getHTML(server, path, getCookie(language), "");
    std::vector<CalendarEvent> events;
    getEvents(html, events, language);
    for (const auto &term : getUpcomingTerms(html)) {
        html = getHTML(server, path, getCookie(language), "search_ac_year=" + term);
        getEvents(html, events, language);
    }
    return generateICal(events, language);
}
