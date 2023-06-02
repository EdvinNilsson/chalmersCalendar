cmake_minimum_required(VERSION 3.16)
project(chalmersCalendar)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS locale date_time REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(chalmersCalendar chalmersCalendar.cpp chalmersCalendar.h calendarEvent.cpp calendarEvent.h iCalGenerator.cpp iCalGenerator.h webRequest.cpp webRequest.h language.h)

target_link_libraries(chalmersCalendar Boost::locale Boost::date_time OpenSSL::SSL OpenSSL::Crypto)
