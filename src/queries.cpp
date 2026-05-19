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

#include "../include/queries.hpp"
#include <format>
#include <sstream>
#include <stdexcept>

using namespace Litesaver;

std::string sql::create_unique_table(const char *table_name)
{
    return std::format(R"sql(
    DROP TABLE IF EXISTS "{}";
    CREATE TABLE "{}" (
       	"key"	TEXT,
       	"type"	TEXT,
       	"{}"	INTEGER,
       	"{}"	REAL,
       	"{}"	TEXT,
       	"{}"	BLOB,
       	"description"	TEXT,
       	PRIMARY KEY("key")
       );
    )sql",
                       table_name,
                       table_name,
                       sql::constants::integer_unique_col_name,
                       sql::constants::float_unique_col_name,
                       sql::constants::string_unique_col_name,
                       sql::constants::blob_unique_col_name);
}

std::string sql::create_log_table()
{
    return std::format(R"sql(
    DROP TABLE IF EXISTS "log";
    CREATE TABLE "log" (
       	"timestamp"	TEXT,
       	"msg_type"	TEXT,
       	"module"	TEXT,
       	"function"	TEXT,
       	"msg"	TEXT,
        "{}"   INTEGER,
        "{}" FLOAT,
        "{}"  TEXT,
        "{}"  BLOB
       );
    )sql",
                       sql::constants::integer_log_col_name,
                       sql::constants::float_log_col_name,
                       sql::constants::string_log_col_name,
                       sql::constants::blob_log_col_name);
}

std::string sql::create_nonunique_table(const char *table_name,
                                        ValueAnonSetConfig &vasc)
{
    std::stringstream q;
    q << std::format(R"sql(DROP TABLE IF EXISTS "{}";)sql", table_name) << "\n";
    q << std::format(R"sql(CREATE TABLE "{}" ()sql", table_name);
    bool first_flag = true;
    for (auto &[col_name, col_details] : vasc)
    {
        if (first_flag)
        {
            first_flag = false;
        }
        else
        {
            q << ", ";
        }
        q << std::format(R"sql("{}"	)sql", col_name);
        switch (col_details.value_type)
        {

        case FLOAT:
            q << sql::constants::float_sql_type_name << "\n";
            break;
        case INTEGER:
            q << sql::constants::integer_sql_type_name << "\n";
            break;
        case TEXT:
            q << sql::constants::string_sql_type_name << "\n";
            break;
        case BLOB:
            q << sql::constants::blob_sql_type_name << "\n";
            break;

        default:
            throw std::runtime_error(std::format(
                "Implementation Error [create_nonunique_table]: Column of Type "
                "found that is none of std::int64_t, double, std::string, "
                "std::vector<std::uint8_t>."));
        }
    }
    q << ");";
    return q.str();
}

std::string sql::insert_into_nonunique_table(const char *table_name,
                                             ValueAnonSetConfig &vasc)
{
    std::stringstream q;
    q << std::format(R"sql(INSERT INTO "{}" ()sql", table_name);
    bool first_flag = true;
    for (auto &[col_name, col_details] : vasc)
    {
        if (first_flag)
        {
            first_flag = false;
        }
        else
        {
            q << ", ";
        }
        q << col_name;
    }
    q << ") VALUES (";

    first_flag = true;
    for (auto &[col_name, col_details] : vasc)
    {
        if (first_flag)
        {
            first_flag = false;
        }
        else
        {
            q << ", ";
        }
        q << "?";
    }
    q << ");";
    return q.str();
}

std::string sql::create_help_table(const char *table_name)
{
    return std::format(
        R"sql(
    DROP TABLE IF EXISTS "{}";
    CREATE TABLE "{}" (
       	"key"	TEXT,
       	"type"	TEXT,
       	"description"	TEXT,
       	PRIMARY KEY("key")
       );
    )sql",
        table_name,
        table_name);
}

std::string sql::insert_into_unique_table(const char *table_name)
{
    return std::format(

        R"sql(
        INSERT INTO "{}"
        ("key", "type", 
         "{}", "{}", "{}", "{}",
         "description")
        VALUES
        (?, ?, ?, ?, ?, ?, ?);)sql",
        table_name,
        sql::constants::integer_unique_col_name,
        sql::constants::float_unique_col_name,
        sql::constants::string_unique_col_name,
        sql::constants::blob_unique_col_name);
}

std::string sql::insert_help_table(const char *table_name)
{
    return std::format(
        R"sql(
        INSERT INTO {}
        ("key", "type", "description")
        VALUES
        (?, ?, ?);)sql",
        table_name);
}

std::string sql::insert_log_table()
{
    return std::format(
        R"sql(
        INSERT INTO "log"
        (
        "timestamp",
        "msg_type",
        "module",
        "function",
        "msg",
        "{}",
        "{}",
        "{}",
        "{}"
        )
        VALUES
        (?, ?, ?, ?, ?, ?, ?, ?, ?);)sql",
        sql::constants::integer_log_col_name,
        sql::constants::float_log_col_name,
        sql::constants::string_log_col_name,
        sql::constants::blob_log_col_name);
}

std::string sql::insert_log_table_no_dump()
{
    return std::string(
        R"sql(
        INSERT INTO "log"
        (
        "timestamp",
        "msg_type",
        "module",
        "function",
        "msg"
        )
        VALUES
        (?, ?, ?, ?, ?);)sql");
}

std::string sql::insert_log_table_int_dump()
{
    return std::format(
        R"sql(
        INSERT INTO "log"
        (
        "timestamp",
        "msg_type",
        "module",
        "function",
        "msg",
        "{}"
        )
        VALUES
        (?, ?, ?, ?, ?, ?);)sql",
        sql::constants::integer_log_col_name);
}

std::string sql::insert_log_table_float_dump()
{
    return std::format(
        R"sql(
        INSERT INTO "log"
        (
        "timestamp",
        "msg_type",
        "module",
        "function",
        "msg",
        "{}"
        )
        VALUES
        (?, ?, ?, ?, ?, ?);)sql",
        sql::constants::float_log_col_name);
}

std::string sql::insert_log_table_text_dump()
{
    return std::format(
        R"sql(
        INSERT INTO "log"
        (
        "timestamp",
        "msg_type",
        "module",
        "function",
        "msg",
        "{}"
        )
        VALUES
        (?, ?, ?, ?, ?, ?);)sql",
        sql::constants::string_log_col_name);
}

std::string sql::insert_log_table_blob_dump()
{
    return std::format(
        R"sql(
        INSERT INTO "log"
        (
        "timestamp",
        "msg_type",
        "module",
        "function",
        "msg",
        "{}"
        )
        VALUES
        (?, ?, ?, ?, ?, ?);)sql",
        sql::constants::blob_log_col_name);
}

std::string sql::select_from_unique_table(const char *table_name)
{
    return std::format(
        R"sql(
        SELECT "key", "type", 
         "{}", "{}", "{}", "{}",
         "description"
         FROM "{}"
         WHERE key = ?;
        )sql",
        sql::constants::integer_unique_col_name,
        sql::constants::float_unique_col_name,
        sql::constants::string_unique_col_name,
        sql::constants::blob_unique_col_name,
        table_name);
}

std::string sql::update_unique_output_table(const char *table_name,
                                            const char *col_name)
{
    return std::format(
        R"sql(
        UPDATE "output_{}"
        SET "{}" = ?
        WHERE "key" = ?;
        )sql",
        table_name,
        col_name);
}

std::string sql::update_unique_output_table_all_values(const char *table_name)
{
    return std::format(
        R"sql(
        UPDATE "output_{}"
        SET
        "{}" = ? AND
        "{}" = ? AND
        "{}" = ? AND
        "{}" = ?
        WHERE "key" = ?;
        )sql",
        table_name,
        sql::constants::integer_unique_col_name,
        sql::constants::float_unique_col_name,
        sql::constants::string_unique_col_name,
        sql::constants::blob_unique_col_name);
}

std::string sql::insert_into_nonunique_output_table(const char *table_name,
                                                    row_t &row)
{
    std::stringstream q;
    q << std::format(R"sql(INSERT INTO "output_{}" ()sql", table_name);
    bool first_flag = true;
    for (auto &[col_name, col_val] : row)
    {
        if (first_flag)
        {
            first_flag = false;
        }
        else
        {
            q << ", ";
        }
        q << col_name;
    }
    q << ") VALUES (";

    first_flag = true;
    for (auto &[col_name, col_val] : row)
    {
        if (first_flag)
        {
            first_flag = false;
        }
        else
        {
            q << ", ";
        }
        q << "?";
    }
    q << ");";
    return q.str();
}

std::string sql::insert_into_nonunique_output_table(const char *table_name,
                                                    table_t &table)
{
    return insert_into_nonunique_output_table(table_name, table.at(0));
}
