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

#include "SQLiteDB.hpp"
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

struct AnonSegment
{
    ValueAnonSetConfig content;
    bool is_unique;
};

struct Segment
{
    ValueSetConfig content;
    bool is_unique;
};

typedef std::map<std::string, AnonSegment> TableConfig;

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
         TableConfig input_config,
         TableConfig output_config,
         bool fast_mode          = false,
         bool multithread_enable = false);
    ~Base();

    /**
     * Sets the `timezone` to correctly save the log timestamps.
     * Example: Europe/Berlin
     */
    void set_timezone(std::string_view timezone);

    // Log
  public:
    /**
     * Logs a `msg` of `msg_type`.
     */
    void log(log::MsgType msg_type, std::string_view msg);
    /**
     * Logs a `msg` of `msg_type` with an attached floating point data `dump`.
     */
    void log(log::MsgType msg_type, std::string_view msg, double dump);
    /**
     * Logs a `msg` of `msg_type` with an attached integer data `dump`.
     */
    void log(log::MsgType msg_type, std::string_view msg, std::int64_t dump);
    /**
     * Logs a `msg` of `msg_type` with an attached string data `dump`.
     */
    void log(log::MsgType msg_type,
             std::string_view msg,
             std::string_view dump);
    /**
     * Logs a `msg` of `msg_type` with an attached blob data `dump`.
     */
    void log(log::MsgType msg_type,
             std::string_view msg,
             std::vector<std::uint8_t> &dump);
    /**
     * Sets the string `module_name` for column `module` for all consecutive
     * logs until this method is called with another `module_name` again.
     */
    void log_set_module(std::string_view module_name);
    /**
     * Sets the string `function_name` for column `function` for all
     * consecutive logs until this method is called with another
     * `function_name` again.
     */
    void log_set_function(std::string_view function_name);

    // Input
  public:
    /**
     * Gets the input entry at `key` from the pre-set type in one unique input
     * `table`. The return value is a `std::variant`. For available data, see
     * `Litesaver::value_t`.
     */
    value_t get_input_unique(std::string_view table, std::string_view key);

    /**
     * Allows custom read access to the database with a `query` and a row of
     * `params`, returning a `Table` with `SQLiteDB::Row`.
     */
    SQLiteDB::Table direct_read_access(std::string query, SQLiteDB::Row params);
    /**
     * Allows custom read access to the database with a `query`, returning a
     * `Table` with `SQLiteDB::Row`.
     */
    SQLiteDB::Table direct_read_access(std::string query);
    /**
     * Allows custom write access to the database with a `query` and a vector of
     * rows of `params`.
     */
    void direct_write_access(std::string query,
                             std::vector<SQLiteDB::Row> params);
    /**
     * Allows custom write access to the database with a `query` and a row of
     * `params`.
     */
    void direct_write_access(std::string query, SQLiteDB::Row params);
    /**
     * Allows custom write access to the database with a `query`.
     */
    void direct_write_access(std::string query);

    // Output
  public:
    /**
     * Saves one row of `values` to the nonunique table output_`table_name`. The
     * user is responsible for passing a fitting row.
     */
    void save_nonunique(const char *table_name, row_t values);
    /**
     * Saves one table (multiple rows) of `values` to the nonunique table
     * output_`table_name`. The user is responsible for passing a fitting table
     * of rows.
     */
    void save_nonunique(const char *table_name, table_t values);

    /**
     * Sets all values in the unique table output_`table_name` at `key` to
     * `NULL`.
     */
    void update_unique_with_null(const char *table_name, const char *key);
    /**
     * Sets the integer `value` in the unique table output_`table_name` at
     * `key`.
     */
    void update_unique(const char *table_name,
                       const char *key,
                       std::int64_t value);
    /**
     * Sets the floating point `value` in the unique table output_`table_name`
     * at `key`.
     */
    void update_unique(const char *table_name, const char *key, double value);
    /**
     * Sets the string `value` in the unique table output_`table_name` at
     * `key`.
     */
    void update_unique(const char *table_name,
                       const char *key,
                       std::string value);
    /**
     * Sets the blob `value` in the unique table output_`table_name` at `key`.
     */
    void update_unique(const char *table_name,
                       const char *key,
                       std::vector<std::uint8_t> value);

  private:
    /// PIMPL
    struct Core;

  private:
    /// PIMPL
    std::unique_ptr<Core> core;
};

} // namespace Litesaver

#endif // _LITESAVER_HPP_