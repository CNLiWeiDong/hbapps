#include <hb/grid_trad_plugin/delvier_order.h>
#include <sstream>
#include <hb/time/time.h>

namespace hb{ namespace plugin {
        void delvier_order::complete_sale_order(const order_result_type& order_result, const sale_tx_type& tx) {
            log_info<<"[fn] delvier_order::complete_sale_order";
            auto &db_plugin = app().get_plugin<grid_db_plugin>();
            auto db_api = db_plugin.get_api();
            auto &mail_plugin = app().get_plugin<send_mail_plugin>();
            auto mail_api = mail_plugin.get_api();
            
            sale_tx_type delvier_tx = std::move(tx);
            delvier_tx.deal_price = order_result.price;
            delvier_tx.deal_number = order_result.number;
            delvier_tx.deal_fee = 0;
            delvier_tx.history_fee = order_result.fee;
            delvier_tx.deal_amount = order_result.amount;
            delvier_tx.used_buy_amount = 0;
            delvier_tx.free_buy_amount = delvier_tx.deal_amount;
            if(!fee_is_free_) {
                delvier_tx.deal_fee = order_result.fee;
                delvier_tx.free_buy_amount -= delvier_tx.deal_fee; 
            }
            delvier_tx.valid_sale_number = delvier_tx.free_buy_amount/delvier_tx.deal_price;
            delvier_tx.status = db_tx_order_status(order_result.status);
            delvier_tx.use_status = db_tx_use_status::db_tx_unused;
            log_info<<"deliver_sale_tx:" << delvier_tx.order_id << "|" << delvier_tx.deal_price << "|"  << delvier_tx.deal_number << "|" << order_result.status;
            db_api->deliver_sale_tx(delvier_tx);
        }
        void delvier_order::deal_selling_order(const sale_tx_array_type& txs){
            log_info<<"[fn] delvier_order::deal_selling_order";
            auto &trad_plugin = app().get_plugin<trad_api_plugin>();
            auto trad_api = trad_plugin.get_api();
            for(auto &tx : txs) {
                order_result_type order_result = trad_api->get_order_status(tx.order_id);
                if(order_result.status!=trad_order_status::order_processing){
                    complete_sale_order(order_result,tx);
                    continue;
                }
                // cancell tx
                if(hb::time::timestamp()-tx.create_time>=order_cancel_seconds_){
                    trad_api->cancell_order(tx.order_id);
                }
            }
        }
        void delvier_order::complete_buy_order(const order_result_type& order_result, const buy_tx_type& tx){
            log_info<<"[fn] delvier_order::complete_buy_order";
            auto &db_plugin = app().get_plugin<grid_db_plugin>();
            auto db_api = db_plugin.get_api();
            auto &mail_plugin = app().get_plugin<send_mail_plugin>();
            auto mail_api = mail_plugin.get_api();
            
            buy_tx_type delvier_tx = std::move(tx);
            delvier_tx.deal_price = order_result.price;
            delvier_tx.deal_number = order_result.number;
            delvier_tx.deal_fee = 0;
            delvier_tx.history_fee = order_result.fee;
            if(!fee_is_free_) {
                delvier_tx.deal_fee = order_result.fee;
                 
            }
            delvier_tx.deal_amount = order_result.amount;
            delvier_tx.status = db_tx_order_status(order_result.status);
            log_info<<"deliver_buy_tx:" << delvier_tx.order_id << "|" << delvier_tx.deal_price << "|"  << delvier_tx.deal_number << "|" << order_result.status;
            db_api->deliver_buy_tx(delvier_tx);
        }
        void delvier_order::deal_buying_order(const buy_tx_array_type& txs){
            log_info<<"[fn] delvier_order::deal_buying_order";
            auto &trad_plugin = app().get_plugin<trad_api_plugin>();
            auto trad_api = trad_plugin.get_api();
            for(auto &tx : txs) {
                order_result_type order_result = trad_api->get_order_status(tx.order_id);
                if(order_result.status!=trad_order_status::order_processing){
                    complete_buy_order(order_result,tx);
                    continue;
                }
                // cancell tx
                if(hb::time::timestamp()-tx.create_time>=order_cancel_seconds_){
                    trad_api->cancell_order(tx.order_id);
                }
            }
        }
        std::set<std::string> delvier_order::deal(){
            log_info<<"[fn] delvier_order::deal";
            auto &db_plugin = app().get_plugin<grid_db_plugin>();
            auto db_api = db_plugin.get_api();
            auto sellings = db_api->get_new_sale_array();
            auto buyings = db_api->get_new_buy_array();
            if(sellings.size()>0 || buyings.size()>0) {
                hb_try
                    deal_selling_order(sellings);
                    deal_buying_order(buyings);
                hb_catch([](const auto &e){
                    log_throw("deal selling or buying error.", e);
                })
            }
            std::set<std::string> dealing_segs;
            for(auto &it : sellings) {
                if(dealing_segs.find(it.seg_id)==dealing_segs.end()) {
                    dealing_segs.insert(it.seg_id);
                }
            }
            for(auto &it : buyings) {
                if(dealing_segs.find(it.seg_id)==dealing_segs.end()) {
                    dealing_segs.insert(it.seg_id);
                }
            }
            return dealing_segs;
        }
} }