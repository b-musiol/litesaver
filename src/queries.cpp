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
       	"valInteger"	INTEGER,
       	"valFloat"	REAL,
       	"valText"	TEXT,
       	"valBlob"	BLOB,
       	"description"	TEXT,
       	PRIMARY KEY("key")
       );
    )sql",
                       table_name,
                       table_name);
}

std::string sql::create_log_table()
{
    return std::string(R"sql(
    DROP TABLE IF EXISTS "log";
    CREATE TABLE "log" (
       	"timestamp"	TEXT,
       	"msg_type"	TEXT,
       	"module"	TEXT,
       	"function"	TEXT,
       	"msg"	TEXT,
        "dumpInt"   INTEGER,
        "dumpFloat" FLOAT,
        "dumpText"  TEXT,
        "dumpBlob"  BLOB
       );
    )sql");
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
            q << "REAL" << "\n";
            break;
        case INTEGER:
            q << "INTEGER" << "\n";
            break;
        case TEXT:
            q << "TEXT" << "\n";
            break;
        case BLOB:
            q << "BLOB" << "\n";
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

std::string sql::create_help_table(const char *table_name)
{
    return std::format(R"sql(
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
         "valInteger", "valFloat", "valText", "valBlob",
         "description")
        VALUES
        (?, ?, ?, ?, ?, ?, ?);)sql", table_name
    );
}

std::string sql::insert_help_table(const char *table_name)
{
    return std::format(
        R"sql(
        INSERT INTO {}
        ("key", "type", "description")
        VALUES
        (?, ?, ?);)sql", table_name
    );
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
        "dumpInt",
        "dumpFloat",
        "dumpText",
        "dumpBlob"
        )
        VALUES
        (?, ?, ?, ?, ?, ?, ?, ?, ?);)sql"
    );
}

std::string sql::insert_log_table_no_dump()
{
    return std::format(
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
        (?, ?, ?, ?, ?);)sql"
    );
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
        "dumpInt"
        )
        VALUES
        (?, ?, ?, ?, ?, ?);)sql"
    );
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
        "dumpFloat"
        )
        VALUES
        (?, ?, ?, ?, ?, ?);)sql"
    );
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
        "dumpText"
        )
        VALUES
        (?, ?, ?, ?, ?, ?);)sql"
    );
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
        "dumpBlob"
        )
        VALUES
        (?, ?, ?, ?, ?, ?);)sql"
    );
}