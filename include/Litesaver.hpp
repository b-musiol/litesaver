/**
 * Litesaver
 * SQLite based utility to save and retrieve data for input (e.g. config),
 * logging and output (e.g. results).
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#ifndef _LITESAVER_HPP_
#define _LITESAVER_HPP_

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace Litesaver
{

namespace log
{
enum class MsgType
{
    INFO,
    ERROR,
    WARNING,
    COMMENT,
};
} // namespace log

enum ValueType
{
    FLOAT,
    INTEGER,
    TEXT,
    BLOB
};

typedef std::variant<std::monostate,
                     std::int64_t,
                     double,
                     std::string,
                     std::vector<std::uint8_t>>
    value_t;

typedef std::map<std::string, value_t> row_t;
typedef std::vector<row_t> table_t;

struct ValueConfig
{
    value_t value; // default value
    std::string description;
};
struct ValueAnonConfig
{
    ValueType value_type;
    std::string description;
};

typedef std::map<std::string, ValueAnonConfig> ValueAnonSetConfig;
typedef std::map<std::string, ValueConfig> ValueSetConfig;

struct OutputAnonSegment
{
    ValueAnonSetConfig content;
    bool is_unique;
};

struct OutputSegment
{
    ValueSetConfig content;
    bool is_unique;
};

typedef std::map<std::string, ValueConfig> InputConfig;
typedef std::map<std::string, OutputAnonSegment> OutputConfig;

class Base
{
  public:
    /**
     * Assumes this is a Database at `file_path` that has previously been
     * assembled and extracts the configs from it. Optionally opens it in a
     * `fast_mode` and allows to be used multithreaded with
     * `multithread_enable`.
     */
    Base(std::filesystem::path file_path,
         bool fast_mode          = false,
         bool multithread_enable = false);
    /**
     * Assumes this is a fresh Database at `file_path`. If one exists, it is
     * overwritten and reset entirely! Optionally opens it in a `fast_mode` and
     * allows to be used multithreaded with `multithread_enable`.
     */
    Base(std::filesystem::path file_path,
         InputConfig input_config,
         OutputConfig output_config,
         bool fast_mode          = false,
         bool multithread_enable = false);
    ~Base();

    void set_timezone(std::string_view timezone);

    // Log
  public:
    void log(log::MsgType msg_type, std::string_view msg);
    void log(log::MsgType msg_type, std::string_view msg, double dump);
    void log(log::MsgType msg_type, std::string_view msg, std::int64_t dump);
    void log(log::MsgType msg_type,
             std::string_view msg,
             std::string_view dump);
    void log(log::MsgType msg_type,
             std::string_view msg,
             std::vector<std::uint8_t> &dump);
    void log_set_module(std::string_view module_name);
    void log_set_function(std::string_view function_name);

    // Input
  public:
    value_t get_input(std::string_view key);

    //
  public:
    void save_nonunique(const char *table_name, row_t values);
    void save_nonunique(const char *table_name, table_t values);

    void update_unique_with_null(const char *table_name, const char *key);
    void update_unique(const char *table_name,
                       const char *key,
                       std::int64_t value);
    void update_unique(const char *table_name, const char *key, double value);
    void update_unique(const char *table_name,
                       const char *key,
                       std::string value);
    void update_unique(const char *table_name,
                       const char *key,
                       std::vector<std::uint8_t> value);

  private:
    struct Core;

  private:
    std::unique_ptr<Core> core;
};

} // namespace Litesaver

#endif // _LITESAVER_HPP_