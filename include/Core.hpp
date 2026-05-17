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

namespace Litesaver
{
struct Base::Core
{
    std::unique_ptr<SQLiteDB::Database> db;
    InputConfig input_config;   // std::map
    OutputConfig output_config; // std::map

    Core(std::filesystem::path db_path,
         bool fast_mode,
         bool multithread_enable);

    void reset_input_table();
    void reset_output_tables();
};
} // namespace Litesaver

#endif // _LITESAVER_CORE_HPP