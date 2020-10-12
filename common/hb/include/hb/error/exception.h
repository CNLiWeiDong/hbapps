#pragma once

#include <exception>
// #include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include <boost/core/demangle.hpp>
#include <hb/error/error_code.h>
 #include <boost/format.hpp>

// #ifndef BOOST_ENABLE_ASSERT_DEBUG_HANDLER
// #define BOOST_ENABLE_ASSERT_DEBUG_HANDLER
// #endif

#define msg_max_length 512

namespace hb { namespace error {
    using namespace std;

    // typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> exception_stack;
    typedef boost::error_info<struct tag_err_code, int> err_code;
    typedef boost::error_info<struct tag_err_title, std::string> err_title;
    typedef boost::error_info<struct tag_err_msg, std::string> err_msg;
    typedef boost::error_info<struct tag_err_type, std::string> err_type;
    typedef boost::error_info<struct tag_err_file, std::string> err_file;
    typedef boost::error_info<struct tag_err_fun, std::string> err_fun;
    typedef boost::error_info<struct tag_err_line, int> err_line;

    class hb_exception_base: virtual public std::exception, virtual public boost::exception {
    public:
        virtual int code() = 0;
        virtual string msg() = 0;
        virtual string name() = 0;
        virtual string title() = 0;
    };

    template<typename Impl>
    class hb_exception: public hb_exception_base
    { 
    protected:
        hb_exception():name_(boost::core::demangle(typeid(Impl).name())) 
        { }
    public:
        hb_exception(const int &code, const std::string &title):
                code_(code), 
                title_(title), 
                name_(boost::core::demangle(typeid(Impl).name())) 
        { }
    
        virtual const char *what() const throw() 
        { 
            return msg_.c_str(); 
        }
        
        virtual int code() { return code_;}
        virtual string msg() { return msg_;}
        virtual string name() { return name_;}
        virtual string title() { return title_;}

        Impl& msg(const std::string & val) { msg_ = val; return *dynamic_cast<Impl*>(this);}
    private:
        /* 使用sprintf更简单,保留学习 */
        template<class TFirst>
        void string_format(boost::format& fmt, TFirst&& first)
        {
            fmt % first;
        }
        template<class TFirst, class... TOther>
        void string_format(boost::format& fmt, TFirst&& first, TOther&&... other)
        {
            fmt % first;
            string_format(fmt, other...);
        }
    public:
        template<class... TOther>
        Impl msg(const std::string & fmt_str, TOther&&... other)
        {
            boost::format fmt("%s"+fmt_str);
            string_format(fmt, "", other...);
            msg_ = fmt.str();
            return *dynamic_cast<Impl*>(this);
        }
    protected:
        Impl& code(const int & val) { code_ = val; return *dynamic_cast<Impl*>(this); }
        Impl& title(const string & val) { title_ = val; return *dynamic_cast<Impl*>(this); }
        Impl& name(const string & val) { name_ = val; return *dynamic_cast<Impl*>(this); }
    private:
        std::string msg_;
        std::string name_;
        std::string title_;
        int code_;
    };

    class assert_exception : public hb_exception<assert_exception> {
        public:
        assert_exception():hb_exception<assert_exception>(assert_condition_no, get_title(assert_condition_no)) {

        }
    };

    std::string exception_to_json(const hb_exception_base &e);

} }

#define hb_throw(e)                         \
throw(e                                     \
        << hb::error::err_code(e.code())    \
        << hb::error::err_msg(e.msg())      \
        << hb::error::err_type(e.name())    \
        << hb::error::err_title(e.title())  \
        << hb::error::err_file(__FILE__)    \
        << hb::error::err_fun(__func__)     \
        << hb::error::err_line(__LINE__)   \
        );

// boost::BOOST_THROW_EXCEPTION
// boost::enable_error_info(e)
// << hb::error::exception_stack(boost::stacktrace::stacktrace(0, 8))

#define HB_THROW hb_throw

template<typename T>
inline std::string exception_data(const T& e) {
  return boost::current_exception_diagnostic_information();                               
}

template<>
inline std::string exception_data(const hb::error::hb_exception_base& e) {
     return hb::error::exception_to_json(e);  
}

#define HB_TRY          \
try                     \
{ 

#define HB_CATCH(fun)                           \
}                                               \
catch(const hb::error::hb_exception_base& e)    \
{                                               \
    fun(e);                                     \
}                                               \
catch(...)                                      \
{                                               \
    fun(nullptr);                               \
}

#define hb_try HB_TRY

#define hb_catch HB_CATCH


#define HB_ASSERT(condition, fmt, ...)                                         \
  do {                                                                         \
    if (!(condition)) {                                                        \
        char str[log_max_length];                                              \
        sprintf(str, "%s" fmt, "", ##__VA_ARGS__);                             \
        hb_throw(hb::error::assert_exception().msg(str));                      \
    }                                                                          \
  } while (0)

#define hb_assert HB_ASSERT