//
// Created by 李卫东 on 2019-02-26.
//

#include <hb/grid_db_plugin/grid_db_plugin_impl.h>
#include <hb/log_plugin/log_plugin.h>
#include <hb/mysql_plugin/mysql_plugin.h>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <hb/time/time.h>
#include <hb/grid_db_plugin/grid_db_error.h>
#include <hb/error/exception.h>

namespace hb { namespace plugin {
    // uint64_t get_date_seconds(const string& date_str) 
    // { 
    //     boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1)); 
    //     boost::posix_time::time_duration time_from_epoch = boost::posix_time::time_from_string(date_str)- epoch;
    //     return time_from_epoch.total_seconds(); 
    // }
    std::shared_ptr<hb::mysql_plugin::my_connection> grid_db_plugin_impl::get_connection() {
        auto &plugin = app().get_plugin<mysql_plugin>();
        auto api = plugin.get_api();
        auto con = api->connect(db_name_.c_str(), db_host_.c_str(), db_user_.c_str(), db_passwd_.c_str(), db_port_);
        return con;
    }
    void grid_db_plugin_impl::close_connection(const std::shared_ptr<hb::mysql_plugin::my_connection> &con){
        auto &plugin = app().get_plugin<mysql_plugin>();
        auto api = plugin.get_api();
        api->close(con);
    }
    sale_tx_array_type grid_db_plugin_impl::query_sale_tx(const string &sql_str){
        auto con = get_connection();
        auto query = con->query(sql_str);
        auto res = query.store();
        if(!res){
            close_connection(con);
            grid_db_exception e;
            e.msg("query_sale_tx result error! %s",sql_str);
            hb_throw(e);
        }
        sale_tx_array_type arr;
        for(auto &row : res){
            sale_tx_type item = {
                id: (uint32_t)atoi(row["id"]),
                seg_id: row["seg_id"].c_str(),
                seg_price: atof(row["seg_price"]),
                order_id: row["order_id"].c_str(),
                sale_number: atof(row["sale_number"]),
                sale_price: atof(row["sale_price"]),
                deal_number: atof(row["deal_number"]),
                deal_price: atof(row["deal_price"]),
                deal_fee: atof(row["deal_fee"]),
                deal_amount: atof(row["deal_amount"]),
                used_buy_amount: atof(row["used_buy_amount"]),
                free_buy_amount: atof(row["free_buy_amount"]),
                valid_sale_number: atof(row["valid_sale_number"]),
                create_time: hb::time::timestamp(row["create_time"].c_str()),
                update_time: hb::time::timestamp(row["update_time"].c_str()),
                status: db_tx_order_status(atoi(row["status"])),
                use_status: db_tx_use_status(atoi(row["use_status"]))
            };
            arr.push_back(std::move(item));
        }
        log_info<<sql_str<<" result:"<<res.size();
        close_connection(con);
        return arr;
    }
           

    sale_tx_array_type grid_db_plugin_impl::get_delivered_sale_array(){
        // desc 优先最新卖出的先买回，因为最新卖出的在当前价格附近，更容易下次卖出
        boost::format sql_fmt("select * from sale_tx where status in (%d,%d) and use_status in (%d,%d) order by id desc");
        sql_fmt%db_tx_order_status::db_tx_complet_closed;
        sql_fmt%db_tx_order_status::db_tx_portion_closed;
        sql_fmt%db_tx_use_status::db_tx_unused;
        sql_fmt%db_tx_use_status::db_tx_portion_used;
        return query_sale_tx(sql_fmt.str());
    }
    sale_tx_array_type grid_db_plugin_impl::get_new_sale_array(){
        // desc 优先处理最新订单。保证不被老订单卡死
        boost::format sql_fmt("select * from sale_tx where status=%d and use_status=%d order by id desc");
        sql_fmt%db_tx_order_status::db_tx_processing;
        sql_fmt%db_tx_use_status::db_tx_unused;
        return query_sale_tx(sql_fmt.str());
    }
    buy_tx_array_type grid_db_plugin_impl::get_new_buy_array(){
        // desc 优先处理最新订单。保证不被老订单卡死
        auto con = get_connection();
        boost::format sql_fmt("select * from buy_tx where status=%d order by id desc");
        sql_fmt%db_tx_order_status::db_tx_processing;
        auto query = con->query(sql_fmt.str());
        log_info<<"get_new_buy_array sql:"<<query.str();
        auto res = query.store();
        if(!res){
            close_connection(con);
            grid_db_exception e;
            e.msg("get_new_buy_array result error!");
            hb_throw(e);
        }
        buy_tx_array_type arr;
        for(auto &row : res){
            buy_tx_type item = {
                id: (uint32_t)atoi(row["id"]),
                seg_id: row["seg_id"].c_str(),
                seg_price: atof(row["seg_price"]),
                order_id: row["order_id"].c_str(),
                buy_number: atof(row["buy_number"]),
                buy_price: atof(row["buy_price"]),
                deal_number: atof(row["deal_number"]),
                deal_price: atof(row["deal_price"]),
                deal_fee: atof(row["deal_fee"]),
                deal_amount: atof(row["deal_amount"]),
                create_time: hb::time::timestamp(row["create_time"].c_str()),
                update_time: hb::time::timestamp(row["update_time"].c_str()),
                status: db_tx_order_status(atoi(row["status"]))
            };
            arr.push_back(std::move(item));
        }
        log_info<<"get_new_buy_array result:"<<res.size();
        close_connection(con);
        return arr;
    }

    void grid_db_plugin_impl::add_new_sale_tx(const sale_tx_type &tx){
        auto con = get_connection();
        auto query = con->query();
        query<<"insert into sale_tx (seg_id,seg_price,order_id,sale_number,sale_price) values (";
        query<<"%0q:seg_id,";
        query<<"%1q:seg_price,";
        query<<"%2q:order_id,";
        query<<"%3:sale_number,";
        query<<"%4:sale_price";
        query<<")";
        query.parse();
        query.template_defaults["seg_id"]  = tx.seg_id.c_str();
        query.template_defaults["seg_price"]  = tx.seg_price;
        query.template_defaults["order_id"] = tx.order_id.c_str();
        query.template_defaults["sale_number"] = tx.sale_number;
        query.template_defaults["sale_price"] = tx.sale_price;
        log_info<<"add_new_sale_tx sql:"<<query.str();
        auto insert_res = query.execute();
        close_connection(con);
        if(!insert_res){
            grid_db_exception e;
            e.msg("add_new_sale_tx error!");
            hb_throw(e);
        }
    }
    void grid_db_plugin_impl::add_new_buy_tx(const buy_tx_type &tx){
        auto con = get_connection();
        auto query = con->query();
        query<<"insert into buy_tx (seg_id,seg_price,order_id,buy_number,buy_price) values (";
        query<<"%0q:seg_id,";
        query<<"%1q:seg_price,";
        query<<"%2q:order_id,";
        query<<"%3:buy_number,";
        query<<"%4:buy_price";
        query<<")";
        query.parse();
        query.template_defaults["seg_id"]   = tx.seg_id.c_str();
        query.template_defaults["seg_price"]  = tx.seg_price;
        query.template_defaults["order_id"] = tx.order_id.c_str();
        query.template_defaults["buy_number"] = tx.buy_number;
        query.template_defaults["buy_price"] = tx.buy_price;
        log_info<<"add_new_buy_tx sql:"<<query.str();
        auto insert_res = query.execute();
        close_connection(con);
        if(!insert_res){
            grid_db_exception e;
            e.msg("add_buy_tx error!");
            hb_throw(e);
        }
    }
    void update_sale_tx(mysqlpp::Query &query, const sale_tx_type &tx){
        query<<"update sale_tx set ";
        query<<"deal_number = %0:deal_number,";
        query<<"deal_price = %1:deal_price,";
        query<<"deal_fee = %2:deal_fee,";
        query<<"deal_amount = %3:deal_amount,";
        query<<"used_buy_amount = %4:used_buy_amount,";
        query<<"free_buy_amount = %5:free_buy_amount,";
        query<<"valid_sale_number = %6:valid_sale_number,";
        query<<"status = %7:status,";
        query<<"use_status = %8:use_status";
        query<<" WHERE id=%9:id";
        query.parse();
        query.template_defaults["deal_number"]   = tx.deal_number;
        query.template_defaults["deal_price"] = tx.deal_price;
        query.template_defaults["deal_fee"] = tx.deal_fee;
        query.template_defaults["deal_amount"] = tx.deal_amount;
        query.template_defaults["used_buy_amount"] = tx.used_buy_amount;
        query.template_defaults["free_buy_amount"] = tx.free_buy_amount;
        query.template_defaults["valid_sale_number"] = tx.valid_sale_number;
        query.template_defaults["status"] = tx.status;
        query.template_defaults["use_status"] = tx.use_status;
        query.template_defaults["id"] = tx.id;
    }
    void insert_sale_history(mysqlpp::Query &query, const sale_tx_type &tx){
        query<<"insert into sale_history (sale_tx_id,seg_id,seg_price,order_id,deal_number,deal_price,deal_fee,deal_amount,status) values (";
        query<<"%0q:sale_tx_id,";
        query<<"%1q:seg_id,";
        query<<"%2q:seg_price,";
        query<<"%3q:order_id,";
        query<<"%4:deal_number,";
        query<<"%5:deal_price,";
        query<<"%6:deal_fee,";
        query<<"%7:deal_amount,";
        query<<"%8:status";
        query<<")";
        query.parse();
        query.template_defaults["sale_tx_id"]  = tx.id;
        query.template_defaults["seg_id"]  = tx.seg_id.c_str();
        query.template_defaults["seg_price"]  = tx.seg_price;
        query.template_defaults["order_id"] = tx.order_id.c_str();
        query.template_defaults["deal_number"] = tx.deal_number;
        query.template_defaults["deal_price"] = tx.deal_price;
        query.template_defaults["deal_fee"] = tx.history_fee;
        query.template_defaults["deal_amount"] = tx.deal_amount;
        query.template_defaults["status"] = tx.status;
    }
    void grid_db_plugin_impl::deliver_sale_tx(const sale_tx_type &tx){
        auto con = get_connection();
        auto self = shared_from_this();
        auto close = [=](){
            self->close_connection(con);
        };
        mysqlpp::Transaction t(*con);
        hb_try
            auto query = con->query();
            update_sale_tx(query,tx);
            log_info<<"update_sale_tx sql:"<<query.str();
            auto update_res = query.execute();
            if(!update_res){
                grid_db_exception e;
                e.msg("deliver_sale_tx update sale_tx error!");
                hb_throw(e);
            }
            query.reset();
            insert_sale_history(query,tx);
            log_info<<"insert_sale_history sql:"<<query.str();
            auto insert_res = query.execute();
            if(!insert_res){
                grid_db_exception e;
                e.msg("deliver_sale_tx insert into sale_history error!");
                hb_throw(e);
            }
            t.commit();
        hb_catch([&](const auto &err){
            t.rollback();
            close();
            log_throw("deliver_sale_tx", err);
            grid_db_exception e;
            e.msg("deliver_sale_tx commit dababase error!");
            hb_throw(e);
        })
        close();
    }
    void update_buy_tx(mysqlpp::Query &query, const buy_tx_type &tx){
        query<<"update buy_tx set ";
        query<<"deal_number = %0:deal_number,";
        query<<"deal_price = %1:deal_price,";
        query<<"deal_fee = %2:deal_fee,";
        query<<"deal_amount = %3:deal_amount,";
        query<<"status = %4:status";
        query<<" WHERE id=%5:id";
        query.parse();

        query.template_defaults["deal_number"]   = tx.deal_number;
        query.template_defaults["deal_price"] = tx.deal_price;
        query.template_defaults["deal_fee"] = tx.deal_fee;
        query.template_defaults["deal_amount"] = tx.deal_amount;
        query.template_defaults["status"] = tx.status;
        query.template_defaults["id"] = tx.id;
    }
    void insert_buy_history(mysqlpp::Query &query, const buy_tx_type &tx){
        query<<"insert into buy_history (buy_tx_id,seg_id,seg_price,order_id,deal_number,deal_price,deal_fee,deal_amount,status) values (";
        query<<"%0q:buy_tx_id,";
        query<<"%1q:seg_id,";
        query<<"%2q:seg_price,";
        query<<"%3q:order_id,";
        query<<"%4:deal_number,";
        query<<"%5:deal_price,";
        query<<"%6:deal_fee,";
        query<<"%7:deal_amount,";
        query<<"%8:status";
        query<<")";
        query.parse();
        query.template_defaults["buy_tx_id"]  = tx.id;
        query.template_defaults["seg_id"]  = tx.seg_id.c_str();
        query.template_defaults["seg_price"]  = tx.seg_price;
        query.template_defaults["order_id"] = tx.order_id.c_str();
        query.template_defaults["deal_number"] = tx.deal_number;
        query.template_defaults["deal_price"] = tx.deal_price;
        query.template_defaults["deal_fee"] = tx.history_fee;
        query.template_defaults["deal_amount"] = tx.deal_amount;
        query.template_defaults["status"] = tx.status;
    }
    void grid_db_plugin_impl::update_buy_status(mysqlpp::Query &query,const buy_tx_type &tx){
        boost::format sql_fmt("select * from sale_tx where status in (%d,%d) and use_status in (%d,%d) and seg_id=\"%s\" order by id asc");
        sql_fmt%db_tx_order_status::db_tx_complet_closed;
        sql_fmt%db_tx_order_status::db_tx_portion_closed;
        sql_fmt%db_tx_use_status::db_tx_unused;
        sql_fmt%db_tx_use_status::db_tx_portion_used;
        sql_fmt%tx.seg_id;
        sale_tx_array_type sale_lists =  query_sale_tx(sql_fmt.str());
        double all_remain_amount = tx.deal_amount;
        for(auto &it : sale_lists){
            double old_remain_amount = all_remain_amount;
            double old_free_amount = it.free_buy_amount;

            it.free_buy_amount -= old_remain_amount;
            it.use_status = db_tx_use_status::db_tx_portion_used;
            if(it.free_buy_amount<=0){
                it.free_buy_amount = 0;
                it.use_status = db_tx_use_status::db_tx_complet_used;
            }
            it.valid_sale_number = it.free_buy_amount/it.deal_price;
            it.used_buy_amount += old_free_amount-it.free_buy_amount;

            query.reset();
            query<<"update sale_tx set  used_buy_amount = %0:used_buy_amount,";
            query<<"free_buy_amount = %1:free_buy_amount,";
            query<<"valid_sale_number = %2:valid_sale_number,";
            query<<"use_status=%3:use_status ";
            query<<"where id=%4:id";
            query.parse();
            query.template_defaults["used_buy_amount"] = it.used_buy_amount;
            query.template_defaults["free_buy_amount"] = it.free_buy_amount;
            query.template_defaults["valid_sale_number"] = it.valid_sale_number;
            query.template_defaults["use_status"] = it.use_status;
            query.template_defaults["id"] = it.id;
            log_info<<"update_buy_status sql:"<<query.str();
            auto update_res = query.execute();
            if(!update_res){
                grid_db_exception e;
                e.msg("deliver_buy_tx update sale_tx  error!");
                hb_throw(e);
            }
            all_remain_amount = old_remain_amount - old_free_amount;
            if(all_remain_amount<=0){
                break;
            }
        }
    }
    void grid_db_plugin_impl::deliver_buy_tx(const buy_tx_type &tx){
        auto con = get_connection();
        auto self = shared_from_this();
        auto close = [=](){
            self->close_connection(con);
        };
        mysqlpp::Transaction t(*con);
        auto query = con->query();
        hb_try
            update_buy_tx(query,tx);
            log_info<<"update_buy_tx sql:"<<query.str();
            auto update_res = query.execute();
            if(!update_res){
                grid_db_exception e;
                e.msg("deliver_buy_tx update buy_tx  error!");
                hb_throw(e);
            }
            query.reset();
            insert_buy_history(query,tx);
            log_info<<"insert_buy_history sql:"<<query.str();
            auto insert_res = query.execute();
            if(!insert_res){
                grid_db_exception e;
                e.msg("insert into buy_history error!");
                hb_throw(e);
            }
            if((tx.status==db_tx_order_status::db_tx_portion_closed 
                    || tx.status==db_tx_order_status::db_tx_complet_closed) 
                    && tx.deal_amount>0){
                update_buy_status(query,tx);
            }
            t.commit();
        hb_catch([&](const auto &err) {
            t.rollback();
            close();
            log_throw("deliver_buy_tx", err);
            grid_db_exception e;
            e.msg("deliver_buy_tx error!");
            hb_throw(e);
        })
        close();
    }
} }