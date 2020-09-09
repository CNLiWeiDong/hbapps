//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/monitor_price_plugin/monitor_pairs.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <math.h>
#include <fc/http/http.h>
#include <fc/https/https.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <sstream>
#include <fc/time/time.h>

namespace hb{ namespace plugin {
        
        void monitor_pairs::deal(const shared_ptr<monitor_targets> &targets){
            log_info<<"begin monitor_pair";
            auto &all_targets_list = targets->all_tagets();
            for(auto &it : pairs_list_) {
                if(!it.active)
                    continue;
                auto &target1 = all_targets_list[it.id1];
                auto &target2 = all_targets_list[it.id2];
                if(target1.status!=target_status::target_status_ok || target2.status!=target_status::target_status_ok){
                    log_info<<"【pair " << target1.id<< "~" << target2.id << "】status is wrong "<< target1.status << "~" << target2.status;
                    continue;
                }
                if(target1.date!=target2.date){
                    log_info<<"【pair " << target1.id<< "~" << target2.id << "】date is wrong "<< target1.date << "~" << target2.date;
                    continue;
                }
                // auto val = std::fabs(target2.value-target1.value);
                auto val = target2.value-target1.value;
                std::ostringstream contents1;
                contents1<<"(";
                for(auto &it: target1.contents){
                    contents1<<" "<<it<<" ";
                }
                contents1<<")";
                std::ostringstream contents2;
                contents2<<"(";
                for(auto &it: target2.contents){
                    contents2<<" "<<it<<" ";
                }
                contents2<<")";
                if(it.min<val && val<it.max){
                    log_info<< "【pair " << target1.id<< "~" << target2.id << "】normal " << val << contents1.str() << contents2.str();
                }else{
                    log_info<< "【pair " << target1.id<< "~" << target2.id << "】attent " << val << contents1.str() << contents2.str();
                    //snd msg small
                    auto cur_seconds = fc::timestamp();
                    if(cur_seconds - it.send_msg_time >=sendmsg_seconds_){
                        it.send_msg_time = cur_seconds;
                        log_info<< "【pair send msg " << target1.id <<  "~" << target2.id << " 】 value is " << val;
                        //snd msg
                        auto &mail_plugin = app().get_plugin<send_mail_plugin>();
                        auto api = mail_plugin.get_api();
                        std::ostringstream subject;
                        subject<<"[pair "<<target1.id<<"~"<<target2.id<<"]" << val << contents1.str() << contents2.str();
                        api->send_mail(it.mail, subject.str(), subject.str()+"\n\rPlease pay attention to the pair !!!");
                    }
                }
            }
        }
} }