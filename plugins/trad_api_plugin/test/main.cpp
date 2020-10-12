#define BOOST_TEST_MODULE trad_api_test
#include <boost/test/unit_test.hpp>
#include <boost/thread/thread.hpp>
#include <hb/log/log.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <hb/trad_api_plugin/trad_api_plugin_impl.h>
#include <memory>

using namespace std;
using namespace hb::plugin;
namespace bt = boost::unit_test;



struct Env : public trad_api_plugin_impl {
    Env() {
        trad_api_data_type d =  {
                host: "api.huobi.me",
                port: "80",
                access_key: "523333f0-bgbfh5tv3f-f1799835-8982f",
                secret_key: "3e8ea65f-bae19bb3-58ad7d9f-470bd",
                target_pair: "eosusdt",
                expired_seconds: 30,
                cert_pem: "/etc/huobi_cert/cert.pem",
                url_query_pirce: "/market/detail/merged?symbol=",
                url_query_account: "/v1/account/accounts",
                url_query_order: "/v1/order/orders/{order-id}",
                url_query_order_client: "/v1/order/orders/getClientOrder",
                url_cancel_order: "/v1/order/orders/{order-id}/submitcancel",
                url_new_order: "/v1/order/orders/place"
        };
        data(d);
    }
    ~Env() {

    }
};



BOOST_AUTO_TEST_SUITE(trad_api_test)

static shared_ptr<Env> my =make_shared<Env>();
static price_result_type price_result;
static string new_order_id;
static string new_client_order_id = "d4395bb2-4c0d-46c7-b346-b4be7651f6a9";

// get_price
BOOST_AUTO_TEST_CASE(get_price) try {
    log_info<<"get_price>>>>>>>>>>";
    auto result = my->get_price();
    price_result = result;
    log_info<<"close_price:"<<price_result.close_price;
    log_info<<"max_buy_price:"<<price_result.max_buy_price;
    log_info<<"max_buy_num:"<<price_result.max_buy_num;
    log_info<<"max_sale_price:"<<price_result.max_sale_price;
    log_info<<"max_sale_num:"<<price_result.max_sale_num;
    BOOST_CHECK_EQUAL(price_result.status, true);
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
} catch (...) {
    log_throw("get_price", nullptr);
    BOOST_FAIL("get_price error!");
};
// get_account
BOOST_AUTO_TEST_CASE(get_account) try {
    log_info<<"get_account>>>>>>>>>>";
    string account_id = my->get_account();
    my->account_id(account_id);
    log_info<<"spot_account_id is:"<<account_id;
    BOOST_CHECK_EQUAL(account_id, "1197869");
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
} catch (...) {
    log_throw("get_account", nullptr);
    BOOST_FAIL("get_account error!");
};

// new_sale_order
BOOST_AUTO_TEST_CASE(new_sale_order) try {
    log_info<<"new_sale_order>>>>>>>>>>";
    if(!price_result.status){
        BOOST_FAIL("price_result.status error!");
    }
    double use_sale_price = price_result.close_price + price_result.close_price*0.08;
    //new order
    new_order_id = my->new_sale_order(5,use_sale_price);
    log_info<<"new_order_id:"<<new_order_id;
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
} catch (...) {
    log_throw("new_sale_order", nullptr);
    BOOST_FAIL("new_sale_order error!");
};
// // query_order_id_by_client
// BOOST_AUTO_TEST_CASE(query_order_id_by_client) try {
//     log_info<<"query_order_id_by_client>>>>>>>>>>";
//     new_order_id = my->query_order_id_by_client(new_client_order_id);
//     log_info<<"query_order_id_by_client order_id:"<<new_order_id;
//     boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
// } catch (...) {
//     log_throw("query_order_id_by_client");
//     BOOST_FAIL("query_order_id_by_client error!");
// };

// get_order_status
BOOST_AUTO_TEST_CASE(get_order_status) try {
    //query order
    order_result_type order_status = my->get_order_status(new_order_id);
    log_info<<"order_status>>>>>>>>>>";
    log_info<<"status:"<<order_status.status;
    log_info<<"price:"<<order_status.price;
    log_info<<"number:"<<order_status.number;
    log_info<<"amount:"<<order_status.amount;
    log_info<<"fee:"<<order_status.fee;
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
} catch (...) {
    log_throw("get_order_status", nullptr);
    BOOST_FAIL("get_order_status error!");
};
// cancell_order
BOOST_AUTO_TEST_CASE(cancell_order) try {
    log_info<<"cancell_order>>>>>>>>>>";
    my->cancell_order(new_order_id);
} catch (...) {
    log_throw("cancell_order", nullptr);
    BOOST_FAIL("cancell_order error!");
};

//tt
// BOOST_AUTO_TEST_CASE(tt) try {

//     my->account_id("1197869");
//     my->cancell_order("84033115186");
// } catch (...) {
//     log_throw("tt");
//     BOOST_FAIL("tt error!");
// };
BOOST_AUTO_TEST_SUITE_END()