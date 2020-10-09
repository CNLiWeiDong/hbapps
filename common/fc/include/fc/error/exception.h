#pragma once

#include <exception>
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include <boost/core/demangle.hpp>
#include <fc/error/error_code.h>
 
namespace fc { namespace error {
    using namespace std;

    typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> exception_stack;
    typedef boost::error_info<struct tag_err_code, int> err_code;
    typedef boost::error_info<struct tag_err_msg, std::string> err_msg;
    typedef boost::error_info<struct tag_err_type, std::string> err_type;

    class fc_exception_base: virtual public std::exception, virtual public boost::exception {
    public:
        virtual int code() = 0;
        virtual string msg() = 0;
        virtual string name() = 0;
    };

    template<typename Impl>
    class fc_exception: public fc_exception_base
    {
        
    public:
        fc_exception():name_(boost::core::demangle(typeid(Impl).name())) 
        { }
        fc_exception(const int &code, const std::string &msg):
                code_(code), 
                msg_(msg), 
                name_(boost::core::demangle(typeid(Impl).name())) 
        { }
    
        virtual const char *what() const throw() 
        { 
            return msg_.c_str(); 
        }
        
        virtual int code() { return code_;}
        virtual string msg() { return msg_;}
        virtual string name() { return name_;}
        Impl& code(const int & val) { code_ = val; return *this; }
        Impl& msg(const std::string & val) { msg_ = val; return *this;}
        Impl& name(const string & val) { name_ = val; return *this; }
    private:
        std::string msg_;
        std::string name_;
        int code_;
    };

    class assert_exception : fc_exception<assert_exception> {
        assert_exception():fc_exception<assert_exception>(asset_condition_no, error_msg[asset_condition_no]) {

        }
    };

    std::string exception_to_json(const fc_exception_base &e);

} }

#define fc_throw(e) \
boost::BOOST_THROW_EXCEPTION(boost::enable_error_info(e) \
    << fc::error::exception_stack(boost::stacktrace::stacktrace(0, 8)) \
    << fc::error::err_code(e.code()) \
    << fc::error::err_msg(e.msg()) \
    << fc::error::err_type(e.name()) \
    )

#define FC_ASSERT(condition, fmt, ...)                                         \
  do {                                                                         \
    if (!(condition)) {                                                        \
        char str[log_max_length];                                              \
        sprintf(str, "%s" fmt, "", ##__VA_ARGS__);                             \
        fc_throw(fc::error::assert_exception().msg(str));                      \
    }                                                                          \
  } while (0)

#define fc_assert FC_ASSERT