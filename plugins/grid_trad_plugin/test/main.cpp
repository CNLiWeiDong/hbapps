#define BOOST_TEST_MODULE grid_trad_test

#include <boost/filesystem/path.hpp> 
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include "appbase/application.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/thread/thread.hpp>
#include <fc/logging/logging.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

uint64_t t_get_seconds(const string& date_str) 
{ 
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1)); 
    boost::posix_time::time_duration time_from_epoch = boost::posix_time::time_from_string(date_str)- epoch;
    return time_from_epoch.total_seconds(); 
}
int64_t t_cur_seconds() 
{ 
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1)); 
    boost::posix_time::time_duration time_from_epoch = boost::posix_time::second_clock::local_time() - epoch;
    return time_from_epoch.total_seconds(); 
}

BOOST_AUTO_TEST_SUITE(grid_trad_test)


BOOST_AUTO_TEST_CASE(tt) try {
    auto a = t_get_seconds("2020-05-20 23:50:59");
    auto b = t_cur_seconds();
    log_info<<a;
    log_info<<b;
    log_info<<b-a;
} catch (...) {
    log_throw("tt");
    BOOST_FAIL("tt error!");
};
BOOST_AUTO_TEST_SUITE_END()