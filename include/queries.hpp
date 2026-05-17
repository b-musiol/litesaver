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
} // namespace constants

/**
 * Query to create a nonunique table of the name `table_name` parametrized by
 * the config `vasc`, meaning that the entries are columns and there are
 * multiple rows.
 */
std::string create_nonunique_table(const char *table_name,
                                   ValueAnonSetConfig &vasc);

/**
 * Query to creates a unique table, meaning there is one entry per key. The
 * description/help is integrated into the same table.
 */
std::string create_unique_table(const char *table_name);
std::string insert_into_unique_table(const char *table_name);

/**
 * Query to create the help table of a nonunique table of the name
 * `table_name`.
 */
std::string create_help_table(const char *table_name);
std::string insert_help_table(const char *table_name);

} // namespace sql
} // namespace Litesaver

#endif // _LITESAVER_QUERIES_HPP