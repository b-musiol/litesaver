/**
 * Litesaver
 * SQLite based utility to save and retrieve data for input (e.g. config),
 * logging and output (e.g. results).
 * Tests
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#include "../include/Litesaver.hpp"
#include <gtest/gtest.h>
#include <filesystem>

TEST(Litesaver, basic_open)
{
    {
        Litesaver::Base litesaver("./Litesaver_basic_open.db");
    }
}
