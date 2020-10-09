#include <fc/error/exception.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

namespace fc { namespace error {
    std::string exception_to_json(const fc_exception_base &e) {
        boost::property_tree::ptree pt_res;
        pt_res.put("fun",*boost::get_error_info<boost::throw_function>(e));
        pt_res.put("file",*boost::get_error_info<boost::throw_file>(e));
        pt_res.put("line",*boost::get_error_info<boost::throw_line>(e));
        pt_res.put("stack",*boost::get_error_info<exception_stack>(e));
        pt_res.put("code",*boost::get_error_info<err_code>(e));
        pt_res.put("msg",*boost::get_error_info<err_msg>(e));
        pt_res.put("type",*boost::get_error_info<err_type>(e));
        std::stringstream stream;
        boost::property_tree::write_json(stream,pt_res);
        return stream.str();
    }
    
} }