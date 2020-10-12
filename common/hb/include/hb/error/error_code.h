#pragma once

#include <map>
#include <string>

namespace hb { namespace error {
    using namespace std;

    enum error_code {
        success_no = 0,
        assert_condition_no,
        grid_db_plugin_no,
        grid_trad_plugin_no,
        http_server_plugin_no,
        log_server_plugin_no,
        log_plugin_no,
        monitor_log_plugin_no,
        monitor_mail_plugin_no,
        monitor_price_plugin_no,
        mysql_plugin_no,
        send_mail_plugin_no,
        thread_pool_plugin_no,
        ws_server_plugin_no,
        hb_sync_no,
        trad_api_plugin_no
    };
    const map<error_code,string> error_title = {
        make_pair(success_no, "no error!"),
        make_pair(assert_condition_no, "assert condition error!"),
        make_pair(grid_db_plugin_no, "grid_db plugin error!"),
        make_pair(grid_trad_plugin_no, "grid_trad plugin error!"),
        make_pair(http_server_plugin_no, "http_server plugin error!"),
        make_pair(log_server_plugin_no, "log_server plugin error!"),
        make_pair(log_plugin_no, "log_plugin plugin error!"),
        make_pair(monitor_price_plugin_no, "monitor_price plugin error!"),
        make_pair(mysql_plugin_no, "mysql_plugin plugin error!"),
        make_pair(send_mail_plugin_no, "send_mail plugin error!"),
        make_pair(thread_pool_plugin_no, "thread_pool plugin error!"),
        make_pair(ws_server_plugin_no, "ws_server plugin error!"),
        make_pair(hb_sync_no, "hb_sync_no error!"),
        make_pair(monitor_log_plugin_no, "monitor_log plugin error!"),
        make_pair(monitor_mail_plugin_no, "monitor_mail plugin error!"),
        make_pair(trad_api_plugin_no, "trad_api plugin error!")
        
    };

    inline string get_title(const error_code& key) {
        auto cit = error_title.find(key);
        if (cit != error_title.end()) {
            return cit->second;
        }
        return "";
    }
} }