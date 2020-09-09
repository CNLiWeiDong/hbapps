#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/attributes/value_visitation.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/from_settings.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>


namespace fc {
    namespace logging = boost::log;
    namespace src = boost::log::sources;
    namespace expr = boost::log::expressions;
    namespace sinks = boost::log::sinks;
    namespace keywords = boost::log::keywords;
    using namespace boost::asio;
    using boost::asio::ip::udp;

    //  public sinks::basic_sink_backend<
    //     sinks::combine_requirements<
    //         sinks::synchronized_feeding,
    //         sinks::flushing
    //     >::type
    // >
    // class udp_skin_backend : public sinks::text_ostream_backend
    // {
    //     public:
    //     // void consume(logging::record_view const& rec) {
    //     //     const std::string request = "1234";
    //     //     size_t request_length = request.size();
    //     //     udp_socket_.send_to(boost::asio::buffer(request, request_length), endpoint_);
    //     // }
    //     // void flush() {

    //     // }
    //     udp_skin_backend(const std::string &ip, const std::string &port):
    //             sinks::text_ostream_backend(),
    //             ioc_(),
    //             udp_socket_(ioc_, udp::endpoint(udp::v4(), 0))
    //     {
    //         boost::asio::ip::udp::resolver resolver(ioc_);
    //         boost::asio::ip::udp::resolver::query query(ip, port);
    //         boost::system::error_code ec;
    //         auto it = resolver.resolve(query, ec);
    //         if(ec)
    //         {
    //             std::cout<< "querydns can't resolve " << ip << std::endl;
    //             return;
    //         }
    //         endpoint_ = std::move(*it);
    //     }
    //     private:
    //     boost::asio::io_context ioc_;
    //     udp::endpoint endpoint_;
    //     udp::socket udp_socket_;

    // };
    // Factory for the udp_skin_backend sink
    class udp_skin_factory :
        public logging::sink_factory< char >
    {
    public:
        // Creates the sink with the provided parameters
        boost::shared_ptr< sinks::sink > create_sink(settings_section const& settings)
        {
            // Read sink parameters
            std::string ip = "127.0.0.1";
            if (boost::optional< std::string > param = settings["IP"])
                ip = param.get();
            std::string port = "8888";
            if (boost::optional< std::string > param = settings["Port"])
                port  = param.get();
            boost::shared_ptr< sinks::syslog_backend > backend(new sinks::syslog_backend(
                keywords::facility = sinks::syslog::local0,             /*< the logging facility >*/
                keywords::use_impl = sinks::syslog::udp_socket_based    /*< the built-in socket-based implementation should be used >*/
            ));
            backend->set_target_address(ip, std::stoi(port));
            
            // Create the sink
            // boost::shared_ptr< udp_skin_backend > backend = boost::make_shared< udp_skin_backend >(ip, port);
            boost::shared_ptr< sinks::synchronous_sink< sinks::syslog_backend > > sink = boost::make_shared< sinks::synchronous_sink< sinks::syslog_backend > >(backend);

            if (boost::optional< std::string > param = settings["Filter"])
            {
                sink->set_filter(logging::parse_filter(param.get()));
            }

            if (boost::optional< std::string > param = settings["Format"])
            {
                sink->set_formatter(logging::parse_formatter(param.get()));
            }

            return sink;
        }
    };

    inline void register_udp_sink(){
        logging::register_sink_factory("SysUdp", boost::make_shared< udp_skin_factory >());
    }

}