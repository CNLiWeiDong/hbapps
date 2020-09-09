#define BOOST_TEST_MODULE grid_db_test

#include <boost/filesystem/path.hpp> 
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include "appbase/application.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/thread/thread.hpp>
#include <fc/logging/logging.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <hb/grid_db_plugin/grid_db_plugin.h>
#include <memory>

using namespace std;
using namespace appbase;
using namespace hb::plugin;
namespace bt = boost::unit_test;

BOOST_AUTO_TEST_SUITE(grid_db_test)

const string new_sale_order_id = "test-sale-order-id8";
const string new_buy_order_id = "test-buy-order-id8";
const string test_seg_id = "test-seg-id-8";
const double sale_price = 3.1;
const double sale_number = 10;
const double buy_price = 2.9;

// app_init
BOOST_AUTO_TEST_CASE(app_init) try {
    log_info<<"app_init>>>>>>>>>>";
    int argc = 1;
    char *argv[1];
    argv[0] = "grid_db_test";
    auto exePath = boost::filesystem::initial_path<boost::filesystem::path>();
    app().set_default_config_dir(exePath/"config");
    app().set_default_data_dir(exePath/"data");
    if(!app().initialize<grid_db_plugin>(argc, argv))
        BOOST_FAIL("initialize<grid_db_plugin> error!");
    app().startup();
} catch (...) {
    log_throw("app_init");
    BOOST_FAIL("app_init error!");
};
static sale_tx_type new_sale_tx;
// add_new_sale_tx
BOOST_AUTO_TEST_CASE(add_new_sale_tx) try {
    log_info<<"add_new_sale_tx>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    new_sale_tx.seg_id = test_seg_id;
    new_sale_tx.seg_price = sale_price;
    new_sale_tx.order_id = new_sale_order_id;
    new_sale_tx.sale_number = sale_number;
    new_sale_tx.sale_price = sale_price;
    api->add_new_sale_tx(new_sale_tx);
} catch (...) {
    log_throw("add_new_sale_tx");
    BOOST_FAIL("add_new_sale_tx error!");
};
// get_new_sale_array
BOOST_AUTO_TEST_CASE(get_new_sale_array) try {
    log_info<<"get_new_sale_array>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    sale_tx_array_type txs = api->get_new_sale_array();
    for(auto &it : txs){
        if(it.order_id==new_sale_order_id){
            BOOST_CHECK_EQUAL(it.seg_id, new_sale_tx.seg_id);
            BOOST_CHECK_EQUAL(it.seg_price, new_sale_tx.seg_price);
            BOOST_CHECK_EQUAL(it.sale_number, new_sale_tx.sale_number);
            BOOST_CHECK_EQUAL(it.sale_price, new_sale_tx.sale_price);
            new_sale_tx = it;
            return;
        }
    }
    BOOST_FAIL("get_new_sale_array not find the new sale tx!");
} catch (...) {
    log_throw("get_new_sale_array");
    BOOST_FAIL("get_new_sale_array error!");
};
// deliver_sale_tx
BOOST_AUTO_TEST_CASE(deliver_sale_tx) try {
    log_info<<"deliver_sale_tx>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    new_sale_tx.deal_price = new_sale_tx.sale_price;
    new_sale_tx.deal_number = new_sale_tx.sale_number-1;
    new_sale_tx.deal_amount = new_sale_tx.deal_price*new_sale_tx.deal_number;
    new_sale_tx.used_buy_amount = 0;
    new_sale_tx.free_buy_amount = new_sale_tx.deal_amount;
    new_sale_tx.status = db_tx_order_status::db_tx_portion_closed;
    new_sale_tx.use_status = db_tx_use_status::db_tx_unused;
    api->deliver_sale_tx(new_sale_tx);
    
} catch (...) {
    log_throw("deliver_sale_tx");
    BOOST_FAIL("deliver_sale_tx error!");
};
// get_new_sale_array_empty
BOOST_AUTO_TEST_CASE(get_new_sale_array_empty) try {
    log_info<<"get_new_sale_array_empty>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    sale_tx_array_type txs = api->get_new_sale_array();
    if(txs.size()>0){
        BOOST_FAIL("get_new_sale_array_empty is not should exist new sale tx!");
    }
} catch (...) {
    log_throw("get_new_sale_array_empty");
    BOOST_FAIL("get_new_sale_array_empty error!");
};
// get_delivered_sale_array
BOOST_AUTO_TEST_CASE(get_delivered_sale_array) try {
    log_info<<"get_delivered_sale_array>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    sale_tx_array_type txs = api->get_delivered_sale_array();
    for(auto &it : txs){
        if(it.order_id==new_sale_order_id){
            BOOST_CHECK_EQUAL(it.seg_id, new_sale_tx.seg_id);
            BOOST_CHECK_EQUAL(it.seg_price, new_sale_tx.seg_price);
            BOOST_CHECK_EQUAL(it.sale_number, new_sale_tx.sale_number);
            BOOST_CHECK_EQUAL(it.sale_price, new_sale_tx.sale_price);
            BOOST_CHECK_EQUAL(it.used_buy_amount, 0);
            BOOST_CHECK_EQUAL((boost::format("%.4f")%it.free_buy_amount).str(), (boost::format("%.4f")%new_sale_tx.deal_amount).str() );
            BOOST_CHECK_EQUAL(it.status, new_sale_tx.status);
            log_info<<"get_delivered_sale_array sale tx create_time:"<<it.create_time;
            new_sale_tx = it;
            return;
        }
    }
    BOOST_FAIL("get_delivered_sale_array not find the new sale tx!");
} catch (...) {
    log_throw("get_delivered_sale_array");
    BOOST_FAIL("get_delivered_sale_array error!");
};

static buy_tx_type new_buy_tx;
// add_new_buy_tx
BOOST_AUTO_TEST_CASE(add_new_buy_tx) try {
    log_info<<"add_new_buy_tx>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    new_buy_tx.seg_id = new_sale_tx.seg_id;
    new_buy_tx.seg_price = buy_price;
    new_buy_tx.order_id = new_buy_order_id;
    new_buy_tx.buy_number = new_sale_tx.free_buy_amount/new_buy_tx.seg_price;
    new_buy_tx.buy_price = buy_price;    
    api->add_new_buy_tx(new_buy_tx);
} catch (...) {
    log_throw("add_new_buy_tx");
    BOOST_FAIL("add_new_buy_tx error!");
};
// get_new_buy_array
BOOST_AUTO_TEST_CASE(get_new_buy_array) try {
    log_info<<"get_new_buy_array>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    buy_tx_array_type txs = api->get_new_buy_array();
    for(auto &it : txs){
        if(it.order_id==new_buy_order_id){
            BOOST_CHECK_EQUAL(it.seg_id, new_buy_tx.seg_id);
            BOOST_CHECK_EQUAL(it.seg_price, new_buy_tx.seg_price);
            BOOST_CHECK_EQUAL((boost::format("%.4f")%it.buy_number).str(), (boost::format("%.4f")%new_buy_tx.buy_number).str() );
            BOOST_CHECK_EQUAL(it.buy_price, new_buy_tx.buy_price);
            new_buy_tx = it;
            return;
        }
    }
    BOOST_FAIL("get_new_buy_array not find the new buy tx!");
} catch (...) {
    log_throw("get_new_buy_array");
    BOOST_FAIL("get_new_buy_array error!");
};

// deliver_buy_tx
BOOST_AUTO_TEST_CASE(deliver_buy_tx) try {
    log_info<<"deliver_buy_tx>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    new_buy_tx.deal_price = new_buy_tx.buy_price;
    new_buy_tx.deal_number = new_buy_tx.buy_number-1;
    new_buy_tx.deal_amount = new_buy_tx.deal_price*new_buy_tx.deal_number;
    new_buy_tx.status = db_tx_order_status::db_tx_portion_closed;
    api->deliver_buy_tx(new_buy_tx);
    
} catch (...) {
    log_throw("deliver_buy_tx");
    BOOST_FAIL("deliver_buy_tx error!");
};

// get_delivered_sale_array_exist
BOOST_AUTO_TEST_CASE(get_delivered_sale_array_empty) try {
    log_info<<"get_delivered_sale_array_exist>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    sale_tx_array_type txs = api->get_delivered_sale_array();
    if(txs.size()==0){
        BOOST_FAIL("get_delivered_sale_array_exist is should exist delivered sale tx!");
    }
} catch (...) {
    log_throw("get_delivered_sale_array_exist");
    BOOST_FAIL("get_delivered_sale_array_exist error!");
};
// get_new_buy_array_empty
BOOST_AUTO_TEST_CASE(get_new_buy_array_empty) try {
    log_info<<"get_new_buy_array_empty>>>>>>>>>>";
    auto api = app().get_plugin<grid_db_plugin>().get_api();
    buy_tx_array_type txs = api->get_new_buy_array();
    if(txs.size()>0){
        BOOST_FAIL("get_new_buy_array_empty is should not exist new buy tx!");
    }
} catch (...) {
    log_throw("get_new_buy_array_empty");
    BOOST_FAIL("get_new_buy_array_empty error!");
};
BOOST_AUTO_TEST_SUITE_END()