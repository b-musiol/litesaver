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

#include "../include/Core.hpp"
#include "../include/queries.hpp"
#include <SQLiteDB.hpp>
#include <chrono>
#include <filesystem>
#include <format>
#include <memory>


using namespace Litesaver;

Base::Core::Core(std::filesystem::path db_path,
                 bool fast_mode,
                 bool multithread_enable)
    : db(std::make_unique<SQLiteDB::Database>(db_path.string(),
                                              true,
                                              multithread_enable,
                                              !fast_mode,
                                              fast_mode,
                                              fast_mode))
{
    const auto &timezone_db = std::chrono::get_tzdb();
    tz = timezone_db.locate_zone("Europe/Berlin");
}

void Base::Core::reset_input_table()
{
    db->execute_plain(
        sql::create_unique_table(sql::constants::input_table_name));
    std::vector<SQLiteDB::Row> params;
    for (auto &[key, values] : input_config)
    {
        SQLiteDB::Row row;
        row.push_text(key);
        if (std::holds_alternative<std::int64_t>(values.value))
        {
            row.push_integer(std::get<std::int64_t>(values.value));
            row.push_null();
            row.push_null();
            row.push_null();
        }
        else if (std::holds_alternative<double>(values.value))
        {
            row.push_null();
            row.push_real(std::get<double>(values.value));
            row.push_null();
            row.push_null();
        }
        else if (std::holds_alternative<std::string>(values.value))
        {
            row.push_null();
            row.push_null();
            row.push_text(std::get<std::string>(values.value));
            row.push_null();
        }
        else if (std::holds_alternative<std::vector<std::uint8_t>>(
                     values.value))
        {
            row.push_null();
            row.push_null();
            row.push_null();
            row.push_blob(std::get<std::vector<std::uint8_t>>(values.value));
        }
        else
        {
            throw std::runtime_error(std::format(
                "Implementation Error [reset_input_table]: Column of Type "
                "found that is none of std::int64_t, double, std::string, "
                "std::vector<std::uint8_t>."));
        }
        row.push_text(values.description);
        params.push_back(row);
    }
    db->execute_statement_norows(
        sql::insert_into_unique_table(sql::constants::input_table_name),
        params);
}

void Base::Core::reset_output_tables()
{
    for (auto &[segment_name, segment] : output_config)
    {
        if (true == segment.is_unique)
        {
            db->execute_plain(sql::create_unique_table(
                std::format("{}_{}",
                            sql::constants::output_table_prefix,
                            segment_name)
                    .c_str()));
        }
        else
        {
            db->execute_plain(sql::create_help_table(
                std::format("{}_{}_{}",
                            sql::constants::output_table_prefix,
                            segment_name,
                            sql::constants::help_table_suffix)
                    .c_str()));
            db->execute_plain(sql::create_nonunique_table(
                std::format("{}_{}",
                            sql::constants::output_table_prefix,
                            segment_name)
                    .c_str(),
                segment.content));
        }
    }
}

void Base::Core::reset_log_table()
{
    db->execute_plain(sql::create_log_table());
    SQLiteDB::Row params;
    // timestamp
    params.push_text(get_current_time_string());
    // msg_type
    params.push_text("SYSTEM");
    // module
    params.push_text("Litesaver");
    // function
    params.push_text("init");
    // msg
    params.push_text("Litesaver first time initialization done.");
    db->execute_statement_norows((sql::insert_log_table_no_dump()), params);
}

std::string Base::Core::get_current_time_string()
{
    auto now = std::chrono::system_clock::now();
    return std::format("{:%Y-%m-%d %H:%M:%S}", tz->to_local(now));
}