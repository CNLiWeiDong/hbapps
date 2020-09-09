//
// Created by 李卫东 on 2019-11-16.
//

#ifndef CPP_SERVER_LOGGING_H
#define CPP_SERVER_LOGGING_H

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp> 
#include <boost/exception/all.hpp>

namespace fc {
  namespace logging = boost::log;
  namespace src = boost::log::sources;
  namespace sinks = boost::log::sinks;
  namespace keywords = boost::log::keywords;
  namespace trivial = boost::log::trivial;
  namespace bfs = boost::filesystem;
  namespace attrs = boost::log::attributes;
  
  void initialize_config(const bfs::path& file);
  // Convert file path to only the filename
  inline std::string path_to_filename(std::string path) {
    bfs::path file_path(path);
    return file_path.filename().string();
    // return path.substr(path.find_last_of("/\\")+1);
  }

  BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(general_logger, src::severity_logger_mt<trivial::severity_level>)
  {
      // Do something that needs to be done on logger initialization,
      // e.g. add a stop watch attribute.
      logging::add_common_attributes();
      logging::register_simple_formatter_factory<trivial::severity_level, char>("Severity");
      logging::register_simple_filter_factory<trivial::severity_level, char>("Severity");
      // register_udp_sink();
      src::severity_logger_mt<trivial::severity_level> lg(keywords::severity = trivial::trace);
      lg.add_attribute("Channel",attrs::constant< std::string >("general"));
      lg.add_attribute("File", attrs::mutable_constant<std::string>(""));
      lg.add_attribute("Line", attrs::mutable_constant<int>(0));
      lg.add_attribute("Function", attrs::mutable_constant<std::string>(""));
      return lg;
  }
} // namespace fc

#define SET_CUSTOM_ATTRIBUTE(VALUE_TYPE, KEY, VALUE) \
    boost::log::attribute_cast<boost::log::attributes::mutable_constant<VALUE_TYPE>>(fc::general_logger::get().get_attributes()[KEY]).set(VALUE);

#define LOG_LOCATION(SERVERITY) \
    SET_CUSTOM_ATTRIBUTE(int, "Line", __LINE__) \
    SET_CUSTOM_ATTRIBUTE(std::string, "File", fc::path_to_filename(__FILE__)) \
    SET_CUSTOM_ATTRIBUTE(std::string, "Function", __func__) \
    BOOST_LOG_SEV(fc::general_logger::get(), SERVERITY)

#define log_trace   LOG_LOCATION(fc::trivial::trace)
#define log_debug   LOG_LOCATION(fc::trivial::debug)
#define log_info    LOG_LOCATION(fc::trivial::info)
#define log_warn    LOG_LOCATION(fc::trivial::warning)
#define log_error   LOG_LOCATION(fc::trivial::error)
#define log_fatal   LOG_LOCATION(fc::trivial::fatal)

#define log_max_length 512

#define LOG_TRACE(fmt, ...)                                                    \
  do {                                                                         \
      char str[log_max_length];                                                \
      sprintf(str, "%s" fmt, "", ##__VA_ARGS__);                               \
      log_trace<<str;                                                          \
  } while (0)

#define LOG_DEBUG(fmt, ...)                                                    \
  do {                                                                         \
      char str[log_max_length];                                                \
      sprintf(str, "%s" fmt, "", ##__VA_ARGS__);                               \
      log_debug<<str;                                                          \
  } while (0)

#define LOG_INFO(fmt, ...)                                                    \
  do {                                                                        \
      char str[log_max_length];                                               \
      sprintf(str, "%s" fmt, "", ##__VA_ARGS__);                              \
      log_info<<str;                                                          \
  } while (0)

#define LOG_WARN(fmt, ...)                                                    \
  do {                                                                        \
      char str[log_max_length];                                               \
      sprintf(str, "%s" fmt, "", ##__VA_ARGS__);                              \
      log_warn<<str;                                                          \
  } while (0)

#define LOG_ERROR(fmt, ...)                                                    \
  do {                                                                         \
      char str[log_max_length];                                                \
      sprintf(str, "%s" fmt, "", ##__VA_ARGS__);                               \
      log_error<<str;                                                          \
  } while (0)

#define LOG_FATAL(fmt, ...)                                                    \
  do {                                                                         \
      char str[log_max_length];                                                \
      sprintf(str, "%s" fmt, "", ##__VA_ARGS__);                               \
      log_fatal<<str;                                                          \
      boost::throw_exception(std::runtime_error(str));                         \
  } while (0)

#define FC_ASSERT(condition, fmt, ...)                                         \
  do {                                                                         \
    if (!(condition)) {                                                        \
      LOG_FATAL(fmt,##__VA_ARGS__);                                            \
    }                                                                          \
  } while (0)

#define fc_assert FC_ASSERT

#define log_throw(task)                                                               \
({                                                                                    \
    std::string error_str = task+boost::current_exception_diagnostic_information();   \
    log_fatal<<error_str;                                                             \
    error_str;                                                                        \
})
#define LOG_THROW log_throw
#endif // CPP_SERVER_LOGGING_H