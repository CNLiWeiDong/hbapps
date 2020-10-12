#include <hb/grid_trad_plugin/new_order.h>
#include <hb/time/time.h>
#include <hb/grid_trad_plugin/grid_trad_error.h>

namespace hb{ namespace plugin {
        price_result_type new_order::query_price(const bool& is_buy){
            log_info<<"[fn] new_order::query_price";
            auto &trad_plugin = app().get_plugin<trad_api_plugin>();
            auto trad_api = trad_plugin.get_api();
            auto price = trad_api->get_price();
            if(!price.status || price.close_price<=0) {
                grid_trad_exception e;
                e.msg("query_price status error: %f",price.close_price);
                hb_throw(e);
            }
            return price;
        }
        void new_order::add_sale_order(const double& sale_price,const double& sale_num, const tactic_config_type &tactic){
            log_info<<"[fn] new_order::add_sale_order";
            auto &trad_plugin = app().get_plugin<trad_api_plugin>();
            auto trad_api = trad_plugin.get_api();
            auto &db_plugin = app().get_plugin<grid_db_plugin>();
            auto db_api = db_plugin.get_api();

            string order_id = trad_api->new_sale_order(sale_num, sale_price);
            sale_tx_type tx;
            tx.seg_id = tactic.seg_id;
            tx.seg_price = tactic.sale_price;
            tx.order_id = order_id;
            tx.sale_number = sale_num;
            tx.sale_price = sale_price;
            db_api->add_new_sale_tx(tx);
        }
        void new_order::add_buy_order(const double& buy_price,const double& buy_amount, const tactic_config_type &tactic){
            log_info<<"[fn] new_order::add_buy_order";
            auto &trad_plugin = app().get_plugin<trad_api_plugin>();
            auto trad_api = trad_plugin.get_api();
            auto &db_plugin = app().get_plugin<grid_db_plugin>();
            auto db_api = db_plugin.get_api();

            const double buy_number = buy_amount/buy_price;
            string order_id = trad_api->new_buy_order(buy_number, buy_price);
            buy_tx_type tx;
            tx.seg_id = tactic.seg_id;
            tx.seg_price = tactic.buy_price;
            tx.order_id = order_id;
            tx.buy_number = buy_number;
            tx.buy_price = buy_price;
            db_api->add_new_buy_tx(tx);
        }
        const double new_order::get_sold_number(const string& seg_id,const sale_tx_array_type& sold_list){
            double num = 0;
            for(auto &it : sold_list){
                if(it.seg_id==seg_id)
                    num+=it.valid_sale_number;
            }
            return num;
        }
        const double new_order::get_free_amount(const string& seg_id,const sale_tx_array_type& sold_list){
            double amount = 0;
            for(auto &it : sold_list){
                if(it.seg_id==seg_id)
                    amount+=it.free_buy_amount;
            }
            return amount;
        }
        bool new_order::deal(std::set<std::string> dealing_segs){
            log_info<<"[fn] new_order::deal";
            auto cur_timestamp = hb::time::timestamp();
            if(cur_timestamp-last_sale_time_<sale_sleep_seconds_ && 
                    cur_timestamp-last_buy_time_<buy_sleep_seconds_)
                return false;
            auto price = query_price(false);
            auto &db_plugin = app().get_plugin<grid_db_plugin>();
            auto db_api = db_plugin.get_api();
            auto sold_list = db_api->get_delivered_sale_array();
            // do sale 
            // 附近成交原则 从高到低卖出。下次更容易买回，底部的更容易卖出。
            if(hb::time::timestamp()-last_sale_time_>=sale_sleep_seconds_) {
                int sale_counts = 0;
                for (auto it = tactics_.rbegin(); it!=tactics_.rend(); it++) {
                    if(dealing_segs.find(it->first)!=dealing_segs.end())
                        continue;
                    if(price.max_buy_price<it->second.sale_price)
                        continue;
                    double sold_num = get_sold_number(it->first,sold_list);
                    // log_info<<"new_order do sale: tactics:"<<it->second.seg_id<<" sold_num:"<<sold_num;
                    if(sold_num>=it->second.sale_number)
                        continue;
                    double new_sale_num = it->second.sale_number-sold_num;
                    // log_info<<"new_order do sale: tactics:"<<it->second.seg_id<<" new_sale_num:"<<new_sale_num;
                    if(new_sale_num<min_number_num_)
                        continue;
                    if(price.max_buy_num<=0)
                        break;
                    price.max_buy_num -= new_sale_num;
                    const double sale_price = price.max_buy_price-price.max_buy_price*improve_deliver_premium_;
                    add_sale_order(sale_price, new_sale_num, it->second);
                    last_sale_time_ = hb::time::timestamp();
                    if(++sale_counts>=every_max_sale_counts_)
                        break;
                }
            }
            // do buy 
            // 附近成交原则，从低到高买入，下次更容易卖出,高部的更容易买入。
            if(hb::time::timestamp()-last_buy_time_>=buy_sleep_seconds_) {
                int buy_counts = 0;
                for (auto it = tactics_.begin(); it!=tactics_.end(); it++) {
                    if(dealing_segs.find(it->first)!=dealing_segs.end())
                        continue;
                    if(price.max_sale_price>it->second.buy_price)
                        continue;
                    double free_amount = get_free_amount(it->first,sold_list);
                    const double buy_price = price.max_sale_price+price.max_sale_price*improve_deliver_premium_;
                    // log_info<<"new_order do buy: tactics:"<<it->second.seg_id<<" free_amount:"<<free_amount<<" buy_price:"<<buy_price;
                    if(free_amount/buy_price<min_number_num_) //min_amount_num_
                        continue;
                    if(price.max_sale_num<=0)
                        break;
                    price.max_sale_num -= free_amount/buy_price;
                    add_buy_order(buy_price, free_amount, it->second);
                    last_buy_time_ = hb::time::timestamp();
                    if(++buy_counts>=every_max_buy_counts_)
                        break;
                }
            }
            return false;
        }
} }