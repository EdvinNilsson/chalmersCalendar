cmake_minimum_required(VERSION 3.16)
project(chalmersCalendar)

set(CMAKE_CXX_STANDARD 20)

add_library(chalmersCalendar chalmersCalendar.cpp chalmersCalendar.h calendarEvent.cpp calendarEvent.h iCalGenerator.cpp iCalGenerator.h webRequest.cpp webRequest.h language.h)

target_link_libraries(chalmersCalendar boost_date_time)
