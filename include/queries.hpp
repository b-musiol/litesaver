/**
 * Litesaver
 * SQLite based utility to save and retrieve data for input (e.g. config),
 * logging and output (e.g. results).
 * SQL queries
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */
#ifndef _LITESAVER_QUERIES_HPP
#define _LITESAVER_QUERIES_HPP

#include "Litesaver.hpp"
#include <string>
namespace Litesaver
{
namespace sql
{
namespace constants
{
constexpr const char *input_table_name    = "input";
constexpr const char *output_table_prefix = "output";
constexpr const char *help_table_suffix   = "help";

constexpr const char *integer_type_name = "integer";
constexpr const char *float_type_name   = "float";
constexpr const char *string_type_name  = "string";
constexpr const char *blob_type_name    = "blob";
} // namespace constants

/**
 * Query to create a nonunique table of the name `table_name` parametrized by
 * the config `vasc`, meaning that the entries are columns and there are
 * multiple rows.
 */
std::string create_nonunique_table(const char *table_name,
                                   ValueAnonSetConfig &vasc);
std::string insert_into_nonunique_table(const char *table_name,
                                        ValueAnonSetConfig &vasc);
std::string insert_into_nonunique_output_table(const char *table_name,
                                               row_t &vasc);
std::string insert_into_nonunique_output_table(const char *table_name,
                                               table_t &vasc);

/**
 * Query to create a unique table, meaning there is one entry per key. The
 * description/help is integrated into the same table.
 */
std::string create_unique_table(const char *table_name);
std::string insert_into_unique_table(const char *table_name);
std::string update_unique_table(const char *table_name, const char *col_name);
std::string select_from_unique_table(const char *table_name);

/**
 * Query to create the help table of a nonunique table of the name
 * `table_name`.
 */
std::string create_help_table(const char *table_name);
std::string insert_help_table(const char *table_name);

/**
 * Query to create the log table.
 */
std::string create_log_table();
std::string insert_log_table();
std::string insert_log_table_no_dump();
std::string insert_log_table_int_dump();
std::string insert_log_table_float_dump();
std::string insert_log_table_text_dump();
std::string insert_log_table_blob_dump();

} // namespace sql
} // namespace Litesaver

#endif // _LITESAVER_QUERIES_HPP