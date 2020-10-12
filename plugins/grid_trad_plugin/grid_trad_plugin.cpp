#include <hb/grid_trad_plugin/grid_trad_plugin.h>
#include <boost/algorithm/string.hpp>
#include <hb/grid_trad_plugin/grid_trad_error.h>

namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _grid_trad_plugin = app().register_plugin<grid_trad_plugin>();
        grid_trad_plugin::grid_trad_plugin(){

        }
        grid_trad_plugin::~grid_trad_plugin(){

        }
        void grid_trad_plugin::set_program_options(options_description& cli, options_description& cfg) {
                 cfg.add_options()
                        ("grid-intervals-seconds", boost::program_options::value<int>()->default_value(10), "the intervals seconds of query price.")
                        ("grid-order-cancel-seconds", boost::program_options::value<int>()->default_value(19), "after commit order, wait seconds to cancel the order.")
                        ("grid-sale-sleep-seconds", boost::program_options::value<int>()->default_value(0), "after create a new sale, wait seconds to deal next new sale.")
                        ("grid-every-max-sale-counts", boost::program_options::value<int>()->default_value(4), "grid-every-max-sale-counts.")
                        ("grid-buy-sleep-seconds", boost::program_options::value<int>()->default_value(5*60), "after create a new buy, wait seconds to deal next new buy.")
                        ("grid-every-max-buy-counts", boost::program_options::value<int>()->default_value(1), "grid-every-max-buy-counts.")
                        ("grid-min-number-num", boost::program_options::value<double>()->default_value(5), "minimum transaction number.")
                        ("grid-improve-deliver-premium", boost::program_options::value<double>()->default_value(0.1/100), "to improve delivery, increase the premium rate when trading.")
                        ("grid-fee-is-free", boost::program_options::value<bool>()->default_value(true), "Is the Tx trad is free, default is true.")
                        ("grid-tactics-config", bpo::value<vector<string>>(), "the grid trading basis tactics config, e.g: seg_id,sale_price,min_buy_price,sale_number,is_need_buy_in_batches")
                        ;
        }
        void grid_trad_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"grid_trad_plugin::plugin_initialize";
                my = make_shared<grid_trad_plugin_impl>();
                my->intervals_seconds(options.at( "grid-intervals-seconds" ).as<int>());
                my->order_cancel_seconds(options.at( "grid-order-cancel-seconds" ).as<int>());
                const double min_trad_num = options.at( "grid-min-number-num" ).as<double>();
                my->min_number_num(min_trad_num);
                my->improve_deliver_premium(options.at( "grid-improve-deliver-premium" ).as<double>());
                my->fee_is_free(options.at( "grid-fee-is-free" ).as<bool>());
                my->every_max_buy_counts(options.at( "grid-every-max-buy-counts" ).as<int>());
                my->every_max_sale_counts(options.at( "grid-every-max-sale-counts" ).as<int>());
                my->sale_sleep_seconds(options.at( "grid-sale-sleep-seconds" ).as<int>());
                my->buy_sleep_seconds(options.at( "grid-buy-sleep-seconds" ).as<int>());
                if(options.count("grid-tactics-config") > 0)
                {
                    auto targets = options.at("grid-tactics-config").as<std::vector<std::string>>();
                    for(auto& arg : targets)
                    {
                        vector<string> props;
                        boost::split(props, arg, boost::is_any_of("\t,"));
                        if(props.size()<4){
                            grid_trad_exception e;
                            e.msg("grid-tactics-config error: %s", arg);
                            hb_throw(e);
                        }
                        tactic_config_type one_tactic = {
                            .seg_id = props[0],
                            .sale_price = std::atof(props[1].c_str()),
                            .buy_price = std::atof(props[2].c_str()),
                            .sale_number = std::atof(props[3].c_str())
                        };
                        if(one_tactic.sale_price<=one_tactic.buy_price){
                            grid_trad_exception e;
                            e.msg("tactics-config error:[%s] the amount of buy is bigger than sale.", one_tactic.seg_id);
                            hb_throw(e);
                        }
                        if(one_tactic.sale_number<min_trad_num){
                            grid_trad_exception e;
                            e.msg("tactics-config error:[%s] the sale number is smaller than min trad num.", one_tactic.seg_id);
                            hb_throw(e);
                        }
                        LOG_INFO("add tactic:%s,%.4f,%.4f,%.4f",one_tactic.seg_id.c_str(),one_tactic.sale_price,one_tactic.buy_price,one_tactic.sale_number);
                        my->add_tactics(one_tactic);
                    }
                }
        }
        void grid_trad_plugin::plugin_startup() {
                log_info<<"grid_trad_plugin::plugin_startup";
                my->start();
        }
        void grid_trad_plugin::plugin_shutdown() {
                log_info<<"grid_trad_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }