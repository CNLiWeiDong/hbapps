#pragma once

#include <map>
#include <string>

namespace fc { namespace error {
    using namespace std;

    enum error_code {
        assert_condition_no = 1
    };
    const map<error_code,string> error_msg = {
        make_pair(asset_condition_no, "assert condition error!")
    };
} }