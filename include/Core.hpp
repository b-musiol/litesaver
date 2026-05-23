/**
 * Litesaver
 * SQLite based utility to save and retrieve data for input (e.g. config),
 * logging and output (e.g. results).
 * Core implementation
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#ifndef _LITESAVER_CORE_HPP
#define _LITESAVER_CORE_HPP

#include "../include/Litesaver.hpp"
#include <SQLiteDB.hpp>
#include <chrono>
#include <filesystem>
#include <memory>

namespace Litesaver
{
struct Base::Core
{
    std::unique_ptr<SQLiteDB::Database> db;
    TableConfig input_config;  // std::map
    TableConfig output_config; // std::map
    const std::chrono::time_zone *tz;
    std::string log_module   = "";
    std::string log_function = "";
    std::string log_msg_type = "";

    Core(std::filesystem::path db_path,
         bool fast_mode,
         bool multithread_enable);

    void reset_input_tables();
    void reset_output_tables();
    void reset_tables(const char *prefix, TableConfig &table_config);
    void reset_log_table();

    ValueAnonSetConfig convert_ValueSetConfig_to_Anon(ValueSetConfig &vsc);

    std::string get_current_time_string();

    constexpr std::string msg_type_to_string(log::MsgType mode)
    {
        switch (mode)
        {
        case log::MsgType::INFO:
            return "Info";
        case log::MsgType::ERROR:
            return "Error";
        case log::MsgType::WARNING:
            return "Warning";
        case log::MsgType::COMMENT:
            return "Comment";
        default:
            return "not_implemented_";
        }

        return "unknown";
    }

    void insert_log(std::string_view msg);
    void insert_log(std::string_view msg, double dump);
    void insert_log(std::string_view msg, std::int64_t dump);
    void insert_log(std::string_view msg, std::string_view dump);
    void insert_log(std::string_view msg, std::vector<std::uint8_t> &dump);

    static std::vector<SQLiteDB::Row> into_vector_of_rows(table_t table);
    static SQLiteDB::Row into_row(row_t row);

  private:
    SQLiteDB::Row params_base_(std::string_view msg);
};
} // namespace Litesaver

#endif // _LITESAVER_CORE_HPP