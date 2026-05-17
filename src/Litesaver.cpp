/**
 * Litesaver
 * SQLite based utility to save and retrieve data for input (e.g. config),
 * logging and output (e.g. results).
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#include "../include/Litesaver.hpp"
#include "../include/Core.hpp"
#include <SQLiteDB.hpp>
#include <filesystem>
#include <memory>
#include <chrono>
#include <string_view>

using namespace Litesaver;

Base::Base(std::filesystem::path file_path,
           bool fast_mode,
           bool multithread_enable)
    : core(std::make_unique<Core>(file_path, fast_mode, multithread_enable))
{
    
}

Base::Base(std::filesystem::path file_path,
           InputConfig input_config,
           OutputConfig output_config,
           bool fast_mode,
           bool multithread_enable)
    : core(std::make_unique<Core>(file_path, fast_mode, multithread_enable))
{
    core->input_config = input_config;
    core->output_config = output_config;

    core->reset_input_table();
    core->reset_output_tables();
    core->reset_log_table();
}

Base::~Base()
{
}

void Base::set_timezone(std::string_view timezone)
{
    const auto& timezone_db = std::chrono::get_tzdb();
    core->tz = timezone_db.locate_zone(timezone);
}