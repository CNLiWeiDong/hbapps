#include <hb/log/log.h>
#include <hb/error/exception.h>
#include <fstream>
#include <stdlib.h>
#include <string>

const char* default_cfg_info = R"(
[Core]
DisableLogging=false
# Sink settings sections

[Sinks.TextFile]
# Sink destination type
Destination=TextFile
Target="logs"
FileName="%Target%/LOG_%Y-%m-%d_%H-%M-%S_%N.log"
# Sink-specific filter. Optional, by default no filter is applied.
Filter="%Channel% contains \"general\" and %Severity% > debug "
# Formatter string. Optional, by default only log record message text is written.
Format="%Message%"
# The flag shows whether the sink should be asynchronous
Asynchronous=false
# Enables automatic stream flush after each log record.
AutoFlush=false
# file size
RotationSize=5000000
# Total number of files in the target directory
MaxFiles=30
# "All" or "Matching" If not specified, no scanning will be performed.
ScanForFiles="All"

[Sinks.Syslog]
# Sink destination type
Destination=Syslog
Filter="%Channel% contains \"general\" and %Severity% > debug "
# Formatter string. Optional, by default only log record message text is written. [%ProcessID%]
Format="[monitor] [%TimeStamp%] [%ThreadID%] [%Channel%] [%Severity%] [%File%:%Function%:%Line%] %Message%"
# Remote address of the syslog server(udp). port default 514
TargetAddress="127.0.0.1"

[Sinks.Console]
# Sink destination type
Destination=Console
Filter="%Severity% > debug "
# Formatter string. Optional, by default only log record message text is written. [%ProcessID%]
Format="%Message%"
# The flag shows whether the sink should be asynchronous
Asynchronous=false
# Enables automatic stream flush after each log record.
AutoFlush=true
)";

namespace hb { namespace log {
    
    void initialize_config(const bfs::path& file_path) {
        if (!boost::filesystem::exists(file_path))
        {
            log_info << "initialize logging is null, write default config info.";
            std::ofstream log_cfg(file_path.string());
            log_cfg.write(default_cfg_info, strlen(default_cfg_info));
            log_cfg.flush();
            log_cfg.close();
        }

        std::ifstream file(file_path.c_str());
        hb_try
            logging::init_from_stream(file);
        hb_catch([](const auto &e){
            log_throw("initialize logging is fail, read log config file fail.", e);
            std::exit(-2);
        })
    }
} }

// if (!boost::filesystem::exists(app().get_logging_conf()))
//                 {
//                         std::cout << "initialize logging is fail, read log config file fail. path: " << app().get_logging_conf() << std::endl;
//                         exit(-2);
//                 }
//                 logging::add_common_attributes();

//                 logging::register_simple_formatter_factory<severity_level, char>("Severity");
//                 logging::register_simple_filter_factory<severity_level, char>("Severity");

//                 std::ifstream file(app().get_logging_conf());
//                 try
//                 {
//                         logging::init_from_stream(file);
//                 }
//                 catch (const std::exception& e)
//                 {
//                         std::cout << "initialize logging is fail, read log config file fail. curse: " << e.what() << std::endl;
//                         exit(-2);
//                 }
//                 src::severity_channel_logger<severity_level,std::string> file_log(keywords::channel = "TextFile");
//                 src::severity_channel_logger<severity_level,std::string> console_log(keywords::channel = "Console");