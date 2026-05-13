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
#include <SQLiteDB.hpp>
#include <filesystem>
#include <memory>

using namespace Litesaver;

struct Base::Core
{
    std::unique_ptr<SQLiteDB::Database> db;

    Core(std::filesystem::path db_path, bool fast_mode, bool multithread_enable)
        : db(std::make_unique<SQLiteDB::Database>(db_path.string(),
                                                  true,
                                                  multithread_enable,
                                                  !fast_mode,
                                                  fast_mode,
                                                  fast_mode))
    {
    }
};

Base::Base(std::filesystem::path file_path,
           bool fast_mode,
           bool multithread_enable)
    : core(std::make_unique<Core>(file_path, fast_mode, multithread_enable))
{
}

Base::~Base()
{
}