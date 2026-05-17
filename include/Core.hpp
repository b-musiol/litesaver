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
#include <filesystem>
#include <memory>
#include <chrono>

namespace Litesaver
{
struct Base::Core
{
    std::unique_ptr<SQLiteDB::Database> db;
    InputConfig input_config;   // std::map
    OutputConfig output_config; // std::map
    const std::chrono::time_zone * tz;

    Core(std::filesystem::path db_path,
         bool fast_mode,
         bool multithread_enable);

    void reset_input_table();
    void reset_output_tables();
    void reset_log_table();

    std::string get_current_time_string();


    constexpr std::string_view msg_type_to_string(log::MsgType mode) {
    switch (mode) {
        case log::MsgType::INFO:  return "Info";
        case log::MsgType::ERROR:  return "Error";
        case log::MsgType::WARNING: return "Warning";
        case log::MsgType::COMMENT: return "Comment";
        default: return "not_implemented_";
    }

    return "unknown";
}
};
} // namespace Litesaver

#endif // _LITESAVER_CORE_HPP