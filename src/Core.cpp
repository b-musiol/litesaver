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
    tz                      = timezone_db.locate_zone("Europe/Berlin");
}

void Base::Core::reset_tables(const char * prefix)
{
    for (auto &[segment_name, segment] : output_config)
    {
        if (true == segment.is_unique)
        {
            db->execute_plain(sql::create_unique_table(
                std::format("{}_{}",
                            prefix,
                            segment_name)
                    .c_str()));

            std::vector<SQLiteDB::Row> unique_params;
            for (auto &[col, detail] : segment.content)
            {
                SQLiteDB::Row unique_row;
                unique_row.push_text(col);

                switch (detail.value_type)
                {

                case FLOAT:
                    unique_row.push_text(sql::constants::float_type_name);
                    break;
                case INTEGER:
                    unique_row.push_text(sql::constants::integer_type_name);
                    break;
                case TEXT:
                    unique_row.push_text(sql::constants::string_type_name);
                    break;
                case BLOB:
                    unique_row.push_text(sql::constants::blob_type_name);
                    break;
                }

                unique_row.push_null(); // valInteger
                unique_row.push_null(); // valFloat
                unique_row.push_null(); // valText
                unique_row.push_null(); // valBlob

                unique_row.push_text(detail.description);

                unique_params.push_back(unique_row);
            }

            db->execute_statement_norows(
                sql::insert_into_unique_table(
                    std::format("{}_{}",
                                prefix,
                                segment_name)
                        .c_str()),
                unique_params);
        }
        else
        {
            db->execute_plain(sql::create_help_table(
                std::format("{}_{}_{}",
                            prefix,
                            segment_name,
                            sql::constants::help_table_suffix)
                    .c_str()));
            std::vector<SQLiteDB::Row> help_params;
            for (auto &[col, detail] : segment.content)
            {
                SQLiteDB::Row help_row;
                help_row.push_text(col);
                switch (detail.value_type)
                {

                case FLOAT:
                    help_row.push_text(sql::constants::float_type_name);
                    break;

                case INTEGER:
                    help_row.push_text(sql::constants::integer_type_name);
                    break;

                case TEXT:
                    help_row.push_text(sql::constants::string_type_name);
                    break;

                case BLOB:
                    help_row.push_text(sql::constants::blob_type_name);
                    break;
                }
                help_row.push_text(detail.description);
                help_params.push_back(help_row);
            }

            db->execute_statement_norows(
                sql::insert_help_table(
                    std::format("{}_{}_{}",
                                prefix,
                                segment_name,
                                sql::constants::help_table_suffix)
                        .c_str()),
                help_params);
            db->execute_plain(sql::create_nonunique_table(
                std::format("{}_{}",
                            prefix,
                            segment_name)
                    .c_str(),
                segment.content));
        }
    }
}

void Base::Core::reset_input_tables()
{
    reset_tables(sql::constants::input_table_prefix);
}


void Base::Core::reset_output_tables()
{
    reset_tables(sql::constants::output_table_prefix);
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

void Base::Core::insert_log(std::string_view msg)
{
    SQLiteDB::Row params = params_base_(msg);

    db->execute_statement_norows((sql::insert_log_table_no_dump()), params);
}

void Base::Core::insert_log(std::string_view msg, double dump)
{
    SQLiteDB::Row params = params_base_(msg);
    params.push_real(dump);

    db->execute_statement_norows((sql::insert_log_table_float_dump()), params);
}

void Base::Core::insert_log(std::string_view msg, std::int64_t dump)
{
    SQLiteDB::Row params = params_base_(msg);
    params.push_integer(dump);

    db->execute_statement_norows((sql::insert_log_table_int_dump()), params);
}

void Base::Core::insert_log(std::string_view msg, std::string_view dump)
{
    SQLiteDB::Row params = params_base_(msg);
    params.push_text(std::string(dump));

    db->execute_statement_norows((sql::insert_log_table_text_dump()), params);
}

void Base::Core::insert_log(std::string_view msg,
                            std::vector<std::uint8_t> &dump)
{
    SQLiteDB::Row params = params_base_(msg);
    params.push_blob(dump);

    db->execute_statement_norows((sql::insert_log_table_blob_dump()), params);
}

SQLiteDB::Row Base::Core::params_base_(std::string_view msg)
{
    SQLiteDB::Row params;
    // timestamp
    params.push_text(get_current_time_string());
    // msg_type
    params.push_text(log_msg_type);
    // module
    params.push_text(log_module);
    // function
    params.push_text(log_function);
    // msg
    params.push_text(std::string(msg));

    return params;
}

std::vector<SQLiteDB::Row> Base::Core::into_vector_of_rows(table_t table)
{
    std::vector<SQLiteDB::Row> sqlitetable;

    for (auto &row : table)
    {
        sqlitetable.push_back(into_row(row));
    }

    return sqlitetable;
}

SQLiteDB::Row Base::Core::into_row(row_t row)
{
    SQLiteDB::Row sqliterow;
    for (auto &[col_name, value] : row)
    {
        if (std::holds_alternative<std::int64_t>(value))
        {
            sqliterow.push_integer(std::get<std::int64_t>(value));
        }
        else if (std::holds_alternative<double>(value))
        {
            sqliterow.push_real(std::get<double>(value));
        }
        else if (std::holds_alternative<std::string>(value))
        {
            sqliterow.push_text(std::get<std::string>(value));
        }
        else if (std::holds_alternative<std::vector<std::uint8_t>>(value))
        {
            sqliterow.push_blob(std::get<std::vector<std::uint8_t>>(value));
        }
        else
        {
            throw std::runtime_error(std::format(
                "Implementation Error [Core::into_row]: Column of Type "
                "found that is none of std::int64_t, double, std::string, "
                "std::vector<std::uint8_t>."));
        }
    }
    return sqliterow;
}