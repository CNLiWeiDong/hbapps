//
// Created by 李卫东 on 2019-02-26.
//
#pragma once

#include <memory>
#include <thread>
#include <functional>
#include <string>
#include <appbase/application.hpp>
#include <hb/logging_plugin/logging_plugin.h>
#include <hb/grid_db_plugin/grid_db_plugin_impl.h>

using namespace std;

namespace hb{ namespace plugin {
        using namespace appbase;
        class grid_db_plugin_api{
        public:
            grid_db_plugin_api(shared_ptr<grid_db_plugin_impl> impl):impl_(impl){}
            sale_tx_array_type get_delivered_sale_array() {
                return impl_->get_delivered_sale_array();
            }
            sale_tx_array_type get_new_sale_array(){
                return impl_->get_new_sale_array();
            }
            buy_tx_array_type get_new_buy_array(){
                return impl_->get_new_buy_array();
            }
            void deliver_sale_tx(const sale_tx_type &tx){
                return impl_->deliver_sale_tx(tx);
            }
            void deliver_buy_tx(const buy_tx_type &tx){
                return impl_->deliver_buy_tx(tx);
            }
            void add_new_sale_tx(const sale_tx_type &tx){
                return impl_->add_new_sale_tx(tx);
            }
            void add_new_buy_tx(const buy_tx_type &tx){
                return impl_->add_new_buy_tx(tx);
            }
        private:
            shared_ptr<grid_db_plugin_impl> impl_;
        };
}}