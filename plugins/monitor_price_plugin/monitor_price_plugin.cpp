#include <hb/monitor_price_plugin/monitor_price_plugin.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp> 
#include <stdlib.h>
#include <set>
#include <hb/monitor_price_plugin/monitor_price_error.h>

namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _monitor_price_plugin = app().register_plugin<monitor_price_plugin>();
        monitor_price_plugin::monitor_price_plugin(){

        }
        monitor_price_plugin::~monitor_price_plugin(){

        }
        void monitor_price_plugin::set_program_options(options_description& cli, options_description& cfg) {
                cfg.add_options()
                        ("monitor-intervals-seconds", boost::program_options::value<int>()->default_value(20), "the intervals seconds of update all targets info.")
                        ("monitor-delay-update-seconds", boost::program_options::value<int>()->default_value(5), "the delay update seconds after query all targets info.")
                        ("monitor-sendmsg-intervals", boost::program_options::value<int>()->default_value(5*60), "the intervals seconds of send message.")
                        ("monitor-senderror-intervals", boost::program_options::value<int>()->default_value(1*60*60), "the intervals seconds of send error msg.")
                        ("monitor-max-request-error", boost::program_options::value<int>()->default_value(5), "the max continuous request response error times.")
                        ("monitor-target-config", boost::program_options::value<string>()->default_value("monitor.ini"), "monitor targets config file name.")
                        ("monitor-request-error-mail-to", boost::program_options::value<string>()->default_value("357879926@qq.com"), "monitor targets request error, send mail to.");

        }
        void monitor_price_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"monitor_price_plugin::plugin_initialize";
                my = make_shared<monitor_price_plugin_impl>();
                my->intervals_seconds(options.at( "monitor-intervals-seconds" ).as<int>());
                my->sendmsg_seconds(options.at( "monitor-sendmsg-intervals" ).as<int>());
                my->senderror_seconds(options.at( "monitor-senderror-intervals" ).as<int>());
                my->delay_update_seconds(options.at( "monitor-delay-update-seconds" ).as<int>());
                my->max_request_error_time(options.at( "monitor-max-request-error" ).as<int>());
                my->request_error_mail_to(options.at( "monitor-request-error-mail-to" ).as<string>());
                bfs::path config_file = options.at( "monitor-target-config" ).as<string>();
                if( config_file.is_relative()) {
                    config_file = app().config_dir()/config_file;
                }
                if (!boost::filesystem::exists(config_file))
                {
                    log_error<<"no monitor target config file to load!";
                    return;
                }
                boost::property_tree::ptree config_pt;  
                boost::property_tree::ini_parser::read_ini(config_file.string(), config_pt);  
                std::set<string> target_ids;
                const int max_targets_num = 100000;
                // add all targets
                for(int i=1; i<=max_targets_num; i++) {
                    auto target = config_pt.get_child_optional("target"+to_string(i));
                    if(!target)
                        break;
                    target_type one_target = {
                        .id = target->get<string>("id"),
                        .server_type = target->get<string>("server_type"),
                        .host = target->get<string>("host"),
                        .port = target->get<string>("port"),
                        .target = target->get<string>("target"),
                        .min_week_day = target->get<int>("min_week_day"),
                        .max_week_day = target->get<int>("max_week_day"),
                        .min_day_minutes = target->get<int>("min_day_minutes"),
                        .max_day_minutes = target->get<int>("max_day_minutes"),
                        .format_value = target->get<string>("format_value"),
                        .format_date = target->get<string>("format_date", ""),
                        .format_contents = target->get<string>("format_contents", ""),
                        .value = 0,
                        .status = target_status::target_status_begin,
                        .update_time = 0,
                        .active = target->get<bool>("active"),
                        .request_error_times = 0,
                        .last_send_error_time = 0
                    };
                    target_ids.insert(one_target.id);
                    LOG_INFO("add target:%s,%s,%s,%s",one_target.id.c_str(),
                        one_target.host.c_str(),
                        one_target.format_value.c_str(),
                        one_target.format_date.c_str());
                    my->add_target(one_target.id, one_target);
                }
                // add all singles
                for(int i=1; i<=max_targets_num; i++) {
                    auto target = config_pt.get_child_optional("single"+to_string(i));
                    if(!target)
                        break;
                    const string id = target->get<string>("id");
                    if(target_ids.find(id)==target_ids.end()) {
                        hb::plugin::monitor_price_exception e;
                        e.msg("single %s is not existed in targets!", id);
                        hb_throw(e);
                    }
                    singles_type single = {
                        id: id,
                        min: target->get<double>("min"),
                        max: target->get<double>("max"),
                        phone: target->get<string>("phone"),
                        mail: target->get<string>("mail"),
                        send_msg_time: 0,
                        active: target->get<bool>("active")
                    };
                    LOG_INFO("add single:%s,%f,%f,%s,%s",id.c_str(),single.min,single.max,single.phone.c_str(),single.mail.c_str());
                    my->add_single(single);
                }  
                // add all pairs
                for(int i=1; i<=max_targets_num; i++) {
                    auto target = config_pt.get_child_optional("pairs"+to_string(i));
                    if(!target)
                        break;
                    const string id1 = target->get<string>("id1");
                    const string id2 = target->get<string>("id2");
                    if(target_ids.find(id1)==target_ids.end() || target_ids.find(id2)==target_ids.end()) {
                        hb::plugin::monitor_price_exception e;
                        e.msg("pairs %s,%s is not existed in targets!", id1, id2);
                        hb_throw(e);
                    }
                    pairs_type p = {
                        .id1 = id1,
                        .id2 = id2,
                        .min = target->get<double>("min"),
                        .max = target->get<double>("max"),
                        .phone = target->get<string>("phone"),
                        .mail = target->get<string>("mail"),
                        .send_msg_time = 0,
                        .active = target->get<bool>("active")
                    };
                    LOG_INFO("add pair:%s,%s,%f,%f,%s",id1.c_str(),id2.c_str(),p.min,p.max,p.mail.c_str());
                    my->add_pairs(p);                    
                }
        }
        void monitor_price_plugin::plugin_startup() {
                log_info<<"monitor_price_plugin::plugin_startup";
                my->start();
        }
        void monitor_price_plugin::plugin_shutdown() {
                log_info<<"monitor_price_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }




/*
                // std::set<string> target_ids;
                if(options.count("monitor-all-targets-list") > 0)
                {
                    auto targets = options.at("monitor-all-targets-list").as<std::vector<std::string>>();
                    for(auto& arg : targets)
                    {
                        vector<string> props;
                        // boost::split(props, arg, boost::is_any_of("\t"));
                        boost::split_regex( props, arg, boost::regex( "\\|\\=\\|" ) );
                        if(props.size()<6){
                            hb_throw("monitor-all-targets-list config is error: %s", arg.c_str());
                        }
                        vector<string> week_vals;
                        boost::split(week_vals, props[5], boost::is_any_of("~:"));
                        if(week_vals.size()<2){
                            hb_throw("monitor-all-targets-list min and max week config is error: %s", props[5].c_str());
                        }
                        vector<string> minutes_vals;
                        boost::split(minutes_vals, props[6], boost::is_any_of("~:"));
                        if(minutes_vals.size()<2){
                            hb_throw("monitor-all-targets-list min and max hours config is error: %s", props[6].c_str());
                        }
                        target_type one_target = {
                            .id = props[0],
                            .server_type = props[1],
                            .host = props[2],
                            .port = props[3],
                            .target = props[4],
                            .min_week_day = std::atoi(week_vals[0].c_str()),
                            .max_week_day = std::atoi(week_vals[1].c_str()),
                            .min_day_minutes = std::atoi(minutes_vals[0].c_str()),
                            .max_day_minutes = std::atoi(minutes_vals[1].c_str()),
                            .format = props[7],
                            .value = 0,
                            .status = target_status::target_status_begin,
                            .update_time = 0
                        };
                        target_ids.insert(props[0]);
                        LOG_INFO("add target:%s,%s,%s,%s,%s,%s,%s,%s",props[0].c_str(),props[1].c_str(),props[2].c_str(),props[3].c_str(),props[4].c_str(),props[5].c_str(),props[6].c_str(),props[7].c_str());
                        my->add_target(props[0], one_target);
                    }
                }
                */
                // if(options.count("monitor-singles-list") > 0)
                // {
                //     auto singles = options.at("monitor-singles-list").as<std::vector<std::string>>();
                //     for(auto& arg : singles)
                //     {
                //         vector<string> props;
                //         boost::split(props, arg, boost::is_any_of("\t,"));
                //         if(props.size()<3){
                //             hb_throw("monitor-singles-list config is error: %s", arg.c_str());
                //         }
                //         if(target_ids.find(props[0])==target_ids.end()) {
                //             hb_throw("single %s is not existed in targets!", props[0].c_str());
                //         }
                //         vector<string> vals;
                //         boost::split(vals, props[1], boost::is_any_of("~:"));
                //         if(vals.size()<2){
                //             hb_throw("monitor-singles-list min and max value config is error: %s", props[1].c_str());
                //         }
                //         singles_type single = {
                //             id: props[0],
                //             min: std::atof(vals[0].c_str()),
                //             max: std::atof(vals[1].c_str()),
                //             phone: props[2],
                //             mail: props[3],
                //             send_msg_time: 0
                //         };
                //         LOG_INFO("add single:%s,%s,%s,%s",props[0].c_str(),vals[0].c_str(),vals[1].c_str(),props[2].c_str());
                //         my->add_single(single);
                //     }
                // }
                // if(options.count("monitor-pairs-list") > 0)
                // {
                //     auto mpairs = options.at("monitor-pairs-list").as<std::vector<std::string>>();
                //     for(auto& arg : mpairs)
                //     {
                //         vector<string> props;
                //         boost::split(props, arg, boost::is_any_of("\t,"));
                //         if(props.size()<4){
                //             hb_throw("monitor-pairs-list config is error: %s", arg.c_str());
                //         }
                //         if(target_ids.find(props[0])==target_ids.end() || target_ids.find(props[1])==target_ids.end()) {
                //             hb_throw("pairs %s,%s is not existed in targets!", props[0].c_str(), props[1].c_str());
                //         }
                //         vector<string> vals;
                //         boost::split(vals, props[2], boost::is_any_of("~:"));
                //         if(vals.size()<2){
                //             hb_throw("monitor-pairs-list min and max value config is error: %s", props[1].c_str());
                //         }
                //         pairs_type p = {
                //             .id1 = props[0],
                //             .id2 = props[1],
                //             .min = std::atof(vals[0].c_str()),
                //             .max = std::atof(vals[1].c_str()),
                //             .phone = props[3],
                //             .mail = props[4],
                //             .send_msg_time = 0
                //         };
                //         LOG_INFO("add pair:%s,%s,%s,%s,%s",props[0].c_str(),props[1].c_str(),vals[0].c_str(),vals[1].c_str(),props[3].c_str());
                //         my->add_pairs(p);
                //     }
                // }