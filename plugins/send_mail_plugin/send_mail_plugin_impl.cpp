//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/send_mail_plugin/send_mail_plugin_impl.h>
#include <hb/thread_pool_plugin/thread_pool_plugin.h>
#include <hb/send_mail_plugin/session.h>

namespace hb{ namespace plugin {
        static uint64_t send_mail_task_id = 0;

        send_mail_plugin_impl::~send_mail_plugin_impl(){
            
        }
        void send_mail_plugin_impl::send_mail(const string &to, const string &subject, const string &content){
                auto & thread_plugin = app().get_plugin<thread_pool_plugin>();
                auto api = thread_plugin.get_api();
                auto self = shared_from_this();
                std::string task_id = "send_mail_"+to_string(send_mail_task_id++);
                log_info<< "send mail to: " << to << "thread task is: " << task_id;
                api->post(task_id, [=](){
                        session sess;
                        sess.host(self->host());
                        sess.port(self->port());
                        sess.user(self->user());
                        sess.pass(self->pass());
                        sess.to(to);
                        sess.subject(subject);
                        sess.content(content);
                        sess.mail();
                });
        }
} }