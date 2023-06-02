#include "calendarEvent.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>

using namespace boost;

const regex tablePattern(R"(<table width="100%" border="0" cellspacing="0" cellpadding="3">(.*)</table>)");
const regex trPattern("<tr .+?>(.+?)</tr>");
const regex tdPattern("<td>(.+?)</td>");
const regex cleanPattern("</?[bi]>");

const regex academicTermPattern("<option value=\"([0-9]{4}\\/[0-9]{4})\"( selected=\"selected\"|)>");

auto swedishExceptions = {"anmälan","registrering", "kursval", "läsperiod"};
auto englishExceptions = {"sign-up", "registration", "course selection", "study period", "application"};

std::string getTable(const std::string& input) {
    smatch what;
    if (regex_search(input, what, tablePattern, match_single_line))
        return what[1].str();
    throw std::exception();
}

std::vector<std::string> getMatches(const std::string& input, const regex& regex) {
    std::vector<std::string> matches;
    std::string::const_iterator start, end;
    start = input.begin();
    end = input.end();
    match_results<std::string::const_iterator> what;
    while (regex_search(start, end,what, regex, match_single_line)) {
        matches.push_back(what[1].str());
        start = what[0].second;
    }
    return matches;
}


std::string cleanString(std::string text) {
    text = regex_replace(text, cleanPattern, "");
    replace_all(text, ";", "\\;");
    return text;
}

std::vector<std::string> getUpcomingTerms(std::string html) {
    std::vector<std::string> res;
    std::string::const_iterator start, end;
    start = html.begin();
    end = html.end();
    match_results<std::string::const_iterator> what;
    while (regex_search(start, end,what, academicTermPattern, match_single_line)) {
        if (!what[2].str().empty()) break;
        res.push_back(what[1].str());
        start = what[0].second;
    }
    return res;
}

bool icontainsAny(const std::string& input, std::initializer_list<const char *> texts) {
    for (int i = 0; i < texts.size(); i++)
        if (algorithm::icontains(input, texts.begin()[i]))
            return true;
    return false;
}

void createEvents(std::vector<std::string> spans, std::vector<CalendarEvent>& events, Language language) {
    if (spans.size() != 5) return;

    if (spans[2] == spans[4])
        events.emplace_back(spans, SingleDay, language);
    else if (icontainsAny(spans[0], language == Swedish ? swedishExceptions : englishExceptions)) {
        auto continued = language == Swedish ? ", forts" : ", continued";
        if (algorithm::icontains(spans[0], continued))
            algorithm::replace_first(spans[0], continued, "");
        else
            events.emplace_back(spans, FirstDay, language);
        events.emplace_back(spans, LastDay, language);
    } else {
        events.emplace_back(spans, Period, language);
    }
}

CalendarEvent::CalendarEvent(std::vector<std::string> spans, EventType type, Language language) {
    summary = cleanString(spans[0]);
    switch (type) {
        case SingleDay:
            start = gregorian::from_string(spans[2]);
            end = start + gregorian::date_duration(1);
            break;
        case FirstDay:
            summary += language == Swedish ? " börjar" : " begins";
            start = gregorian::from_string(spans[2]);
            end = start + gregorian::date_duration(1);
            break;
        case LastDay:
            summary += language == Swedish ? " slutar" : " ends";
            start = gregorian::from_string(spans[4]);
            end = start + gregorian::date_duration(1);
            break;
        case Period:
            start = gregorian::from_string(spans[2]);
            end = gregorian::from_string(spans[4]) + gregorian::date_duration(1);
            break;
    }
}

void getEvents(const std::string& input, std::vector<CalendarEvent>& events, Language language) {
    auto table = getMatches(input, tablePattern).back();

    auto trs = getMatches(table, trPattern);

    for (const auto& tr : trs) {

        auto tds = getMatches(tr, tdPattern);
        createEvents(tds, events, language);
    }
}
