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
#include "../include/queries.hpp"
#include <SQLiteDB.hpp>
#include <chrono>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <variant>

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
    core->input_config  = input_config;
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
    const auto &timezone_db = std::chrono::get_tzdb();
    core->tz                = timezone_db.locate_zone(timezone);
}

void Base::log(log::MsgType msg_type, std::string_view msg)
{
    core->log_msg_type = core->msg_type_to_string(msg_type);
    core->insert_log(msg);
}

void Base::log(log::MsgType msg_type, std::string_view msg, double dump)
{
    core->log_msg_type = core->msg_type_to_string(msg_type);
    core->insert_log(msg, dump);
}

void Base::log(log::MsgType msg_type, std::string_view msg, std::int64_t dump)
{
    core->log_msg_type = core->msg_type_to_string(msg_type);
    core->insert_log(msg, dump);
}

void Base::log(log::MsgType msg_type,
               std::string_view msg,
               std::string_view dump)
{
    core->log_msg_type = core->msg_type_to_string(msg_type);
    core->insert_log(msg, dump);
}

void Base::log(log::MsgType msg_type,
               std::string_view msg,
               std::vector<std::uint8_t> &dump)
{
    core->log_msg_type = core->msg_type_to_string(msg_type);
    core->insert_log(msg, dump);
}

void Base::log_set_module(std::string_view module_name)
{
    core->log_module = module_name;
}

void Base::log_set_function(std::string_view function_name)
{
    core->log_function = function_name;
}

value_t Base::get_input(std::string_view key)
{
    SQLiteDB::Row params;
    params.push_text(std::string(key));
    SQLiteDB::Table result = core->db->execute_statement_returns(
        sql::select_from_unique_table(sql::constants::input_table_name),
        params);
    if (result.data.empty())
    {
        throw std::runtime_error(
            std::format("Litesaver: Tried to get an input value for key [{}]. "
                        "This value does not exist.",
                        key));
    }
    value_t value;
    auto &output = result.data.at(0);
    auto type    = output.get_text(1);
    if (sql::constants::integer_type_name == type)
    {
        value = output.get_integer(2);
    }
    else if (sql::constants::float_type_name == type)
    {
        value = output.get_real(3);
    }
    else if (sql::constants::string_type_name == type)
    {
        value = output.get_text(4);
    }
    else if (sql::constants::blob_type_name == type)
    {
        value = output.get_blob(5);
    }
    else
    {
        value = std::monostate();
    }

    return value;
}

void Base::save_nonunique(const char *table_name, row_t values)
{
    table_t t_values;
    t_values.push_back(values);
    save_nonunique(table_name, t_values);
}

void Base::save_nonunique(const char *table_name, table_t values)
{
    std::vector<SQLiteDB::Row> params = core->into_vector_of_rows(values);

    core->db->execute_statement_norows(
        sql::insert_into_nonunique_output_table(table_name, values),
        params);
}

void Base::update_unique_with_null(const char *table_name, const char *key)
{
    SQLiteDB::Row params;
    params.push_null(); // valInteger
    params.push_null(); // valFloat
    params.push_null(); // valText
    params.push_null(); // valBlob
    params.push_text(key);

    core->db->execute_statement_norows(
        sql::update_unique_output_table_all_values(table_name),
        params);
}
void Base::update_unique(const char *table_name,
                         const char *key,
                         std::int64_t value)
{
    SQLiteDB::Row params;
    params.push_integer(value); // valInteger
    params.push_text(key);

    core->db->execute_statement_norows(
        sql::update_unique_output_table(
            table_name,
            sql::constants::integer_unique_col_name),
        params);
}
void Base::update_unique(const char *table_name, const char *key, double value)
{
    SQLiteDB::Row params;
    params.push_real(value); // valFloat
    params.push_text(key);

    core->db->execute_statement_norows(
        sql::update_unique_output_table(table_name,
                                        sql::constants::float_unique_col_name),
        params);
}
void Base::update_unique(const char *table_name,
                         const char *key,
                         std::string value)
{
    SQLiteDB::Row params;
    params.push_text(value); // valText
    params.push_text(key);

    core->db->execute_statement_norows(
        sql::update_unique_output_table(table_name,
                                        sql::constants::string_unique_col_name),
        params);
}
void Base::update_unique(const char *table_name,
                         const char *key,
                         std::vector<std::uint8_t> value)
{
    SQLiteDB::Row params;
    params.push_blob(value); // valBlob
    params.push_text(key);

    core->db->execute_statement_norows(
        sql::update_unique_output_table(table_name,
                                        sql::constants::blob_unique_col_name),
        params);
}
