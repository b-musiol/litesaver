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

#include "../incl/Litesaver.hpp"
#include <cstdint>
#include <filesystem>
#include <gtest/gtest.h>

TEST(Litesaver, basic_open)
{
    {
        Litesaver::Base litesaver("./Litesaver_basic_open.db");
    }
}

TEST(Litesaver, init_from_scratch)
{
    {
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);

        Litesaver::TableConfig input_config;
        Litesaver::TableConfig output_config;

        Litesaver::AnonSegment in_os;
        Litesaver::ValueAnonSetConfig in_vasc;
        Litesaver::ValueAnonConfig in_vac;
        in_os.is_unique = true;
        in_vasc.clear();
        in_vac.description      = "in_ha1 float stuff";
        in_vac.value_type       = Litesaver::ValueType::FLOAT;
        in_vasc["in_a1"]        = in_vac;
        in_vac.description      = "in_ha2 text stuff";
        in_vac.value_type       = Litesaver::ValueType::TEXT;
        in_vasc["in_a2"]        = in_vac;
        in_vac.description      = "in_ha3 int stuff";
        in_vac.value_type       = Litesaver::ValueType::INTEGER;
        in_vasc["in_a3"]        = in_vac;
        in_vac.description      = "in_ha4 blob stuff";
        in_vac.value_type       = Litesaver::ValueType::BLOB;
        in_vasc["in_a4"]        = in_vac;
        in_os.content           = in_vasc;
        input_config["unique1"] = in_os;
        in_os.is_unique         = false;
        in_vasc.clear();
        in_vac.description         = "in_hb1 float stuff";
        in_vac.value_type          = Litesaver::ValueType::FLOAT;
        in_vasc["in_b1"]           = in_vac;
        in_vac.description         = "in_hb2 text stuff";
        in_vac.value_type          = Litesaver::ValueType::TEXT;
        in_vasc["in_b2"]           = in_vac;
        in_vac.description         = "in_hb3 int stuff";
        in_vac.value_type          = Litesaver::ValueType::INTEGER;
        in_vasc["in_b3"]           = in_vac;
        in_vac.description         = "in_hb4 blob stuff";
        in_vac.value_type          = Litesaver::ValueType::BLOB;
        in_vasc["in_b4"]           = in_vac;
        in_os.content              = in_vasc;
        input_config["nonunique1"] = in_os;

        Litesaver::AnonSegment out_os;
        Litesaver::ValueAnonSetConfig out_vasc;
        Litesaver::ValueAnonConfig out_vac;
        out_os.is_unique = true;
        out_vasc.clear();
        out_vac.description      = "ha1 float stuff";
        out_vac.value_type       = Litesaver::ValueType::FLOAT;
        out_vasc["a1"]           = out_vac;
        out_vac.description      = "ha2 text stuff";
        out_vac.value_type       = Litesaver::ValueType::TEXT;
        out_vasc["a2"]           = out_vac;
        out_vac.description      = "ha3 int stuff";
        out_vac.value_type       = Litesaver::ValueType::INTEGER;
        out_vasc["a3"]           = out_vac;
        out_vac.description      = "ha4 blob stuff";
        out_vac.value_type       = Litesaver::ValueType::BLOB;
        out_vasc["a4"]           = out_vac;
        out_os.content           = out_vasc;
        output_config["unique1"] = out_os;
        out_os.is_unique         = false;
        out_vasc.clear();
        out_vac.description         = "hb1 float stuff";
        out_vac.value_type          = Litesaver::ValueType::FLOAT;
        out_vasc["b1"]              = out_vac;
        out_vac.description         = "hb2 text stuff";
        out_vac.value_type          = Litesaver::ValueType::TEXT;
        out_vasc["b2"]              = out_vac;
        out_vac.description         = "hb3 int stuff";
        out_vac.value_type          = Litesaver::ValueType::INTEGER;
        out_vasc["b3"]              = out_vac;
        out_vac.description         = "hb4 blob stuff";
        out_vac.value_type          = Litesaver::ValueType::BLOB;
        out_vasc["b4"]              = out_vac;
        out_os.content              = out_vasc;
        output_config["nonunique1"] = out_os;

        Litesaver::Base litesaver("./Litesaver_init_from_scratch.db",
                                  input_config,
                                  output_config);

        litesaver.direct_write_access(R"sql(
            UPDATE "input_unique1" SET "valInteger" = 343 WHERE "key" = 'in_a3';
            )sql");
        EXPECT_EQ(std::get<std::int64_t>(litesaver.get_input_unique("unique1", "in_a3")), 343);
    }
}

TEST(Litesaver, test_log)
{
    {
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);

        Litesaver::TableConfig input_config;
        Litesaver::TableConfig output_config;

        Litesaver::AnonSegment in_os;
        Litesaver::ValueAnonSetConfig in_vasc;
        Litesaver::ValueAnonConfig in_vac;
        in_os.is_unique = true;
        in_vasc.clear();
        in_vac.description      = "in_ha1 float stuff";
        in_vac.value_type       = Litesaver::ValueType::FLOAT;
        in_vasc["in_a1"]        = in_vac;
        in_vac.description      = "in_ha2 text stuff";
        in_vac.value_type       = Litesaver::ValueType::TEXT;
        in_vasc["in_a2"]        = in_vac;
        in_vac.description      = "in_ha3 int stuff";
        in_vac.value_type       = Litesaver::ValueType::INTEGER;
        in_vasc["in_a3"]        = in_vac;
        in_vac.description      = "in_ha4 blob stuff";
        in_vac.value_type       = Litesaver::ValueType::BLOB;
        in_vasc["in_a4"]        = in_vac;
        in_os.content           = in_vasc;
        input_config["unique1"] = in_os;
        in_os.is_unique         = false;
        in_vasc.clear();
        in_vac.description         = "in_hb1 float stuff";
        in_vac.value_type          = Litesaver::ValueType::FLOAT;
        in_vasc["in_b1"]           = in_vac;
        in_vac.description         = "in_hb2 text stuff";
        in_vac.value_type          = Litesaver::ValueType::TEXT;
        in_vasc["in_b2"]           = in_vac;
        in_vac.description         = "in_hb3 int stuff";
        in_vac.value_type          = Litesaver::ValueType::INTEGER;
        in_vasc["in_b3"]           = in_vac;
        in_vac.description         = "in_hb4 blob stuff";
        in_vac.value_type          = Litesaver::ValueType::BLOB;
        in_vasc["in_b4"]           = in_vac;
        in_os.content              = in_vasc;
        input_config["nonunique1"] = in_os;

        Litesaver::AnonSegment out_os;
        Litesaver::ValueAnonSetConfig out_vasc;
        Litesaver::ValueAnonConfig out_vac;
        out_os.is_unique = true;
        out_vasc.clear();
        out_vac.description      = "ha1 float stuff";
        out_vac.value_type       = Litesaver::ValueType::FLOAT;
        out_vasc["a1"]           = out_vac;
        out_vac.description      = "ha2 text stuff";
        out_vac.value_type       = Litesaver::ValueType::TEXT;
        out_vasc["a2"]           = out_vac;
        out_vac.description      = "ha3 int stuff";
        out_vac.value_type       = Litesaver::ValueType::INTEGER;
        out_vasc["a3"]           = out_vac;
        out_vac.description      = "ha4 blob stuff";
        out_vac.value_type       = Litesaver::ValueType::BLOB;
        out_vasc["a4"]           = out_vac;
        out_os.content           = out_vasc;
        output_config["unique1"] = out_os;
        out_os.is_unique         = false;
        out_vasc.clear();
        out_vac.description         = "hb1 float stuff";
        out_vac.value_type          = Litesaver::ValueType::FLOAT;
        out_vasc["b1"]              = out_vac;
        out_vac.description         = "hb2 text stuff";
        out_vac.value_type          = Litesaver::ValueType::TEXT;
        out_vasc["b2"]              = out_vac;
        out_vac.description         = "hb3 int stuff";
        out_vac.value_type          = Litesaver::ValueType::INTEGER;
        out_vasc["b3"]              = out_vac;
        out_vac.description         = "hb4 blob stuff";
        out_vac.value_type          = Litesaver::ValueType::BLOB;
        out_vasc["b4"]              = out_vac;
        out_os.content              = out_vasc;
        output_config["nonunique1"] = out_os;

        Litesaver::Base litesaver("./Litesaver_test_log.db",
                                  input_config,
                                  output_config);

        litesaver.log_set_module("TEST");
        litesaver.log_set_function("test_log");
        litesaver.log(Litesaver::log::MsgType::INFO, "some info1");
        litesaver.log(Litesaver::log::MsgType::INFO, "some info2");
        litesaver.log(Litesaver::log::MsgType::ERROR, "some error1");
        litesaver.log(Litesaver::log::MsgType::WARNING, "some warning1");
        litesaver.log(Litesaver::log::MsgType::COMMENT, "some comment1");
        litesaver.log_set_function("test_log_change");
        litesaver.log(Litesaver::log::MsgType::INFO, "some info3");
        litesaver.log_set_module("TEST_change");
        litesaver.log(Litesaver::log::MsgType::ERROR, "some error2");
        litesaver.log(Litesaver::log::MsgType::ERROR,
                      "some error2",
                      static_cast<int64_t>(4711));
        litesaver.log(Litesaver::log::MsgType::ERROR,
                      "some error2",
                      static_cast<double>(47.11));
        litesaver.log(Litesaver::log::MsgType::ERROR,
                      "some error2",
                      "Some Payload");
        litesaver.log(Litesaver::log::MsgType::ERROR, "some error2", blobval);
    }
}

TEST(Litesaver, save_unique_test)
{
    {
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);

        Litesaver::TableConfig input_config;
        Litesaver::TableConfig output_config;

        Litesaver::AnonSegment in_os;
        Litesaver::ValueAnonSetConfig in_vasc;
        Litesaver::ValueAnonConfig in_vac;
        in_os.is_unique = true;
        in_vasc.clear();
        in_vac.description      = "in_ha1 float stuff";
        in_vac.value_type       = Litesaver::ValueType::FLOAT;
        in_vasc["in_a1"]        = in_vac;
        in_vac.description      = "in_ha2 text stuff";
        in_vac.value_type       = Litesaver::ValueType::TEXT;
        in_vasc["in_a2"]        = in_vac;
        in_vac.description      = "in_ha3 int stuff";
        in_vac.value_type       = Litesaver::ValueType::INTEGER;
        in_vasc["in_a3"]        = in_vac;
        in_vac.description      = "in_ha4 blob stuff";
        in_vac.value_type       = Litesaver::ValueType::BLOB;
        in_vasc["in_a4"]        = in_vac;
        in_os.content           = in_vasc;
        input_config["unique1"] = in_os;
        in_os.is_unique         = false;
        in_vasc.clear();
        in_vac.description         = "in_hb1 float stuff";
        in_vac.value_type          = Litesaver::ValueType::FLOAT;
        in_vasc["in_b1"]           = in_vac;
        in_vac.description         = "in_hb2 text stuff";
        in_vac.value_type          = Litesaver::ValueType::TEXT;
        in_vasc["in_b2"]           = in_vac;
        in_vac.description         = "in_hb3 int stuff";
        in_vac.value_type          = Litesaver::ValueType::INTEGER;
        in_vasc["in_b3"]           = in_vac;
        in_vac.description         = "in_hb4 blob stuff";
        in_vac.value_type          = Litesaver::ValueType::BLOB;
        in_vasc["in_b4"]           = in_vac;
        in_os.content              = in_vasc;
        input_config["nonunique1"] = in_os;

        Litesaver::AnonSegment out_os;
        Litesaver::ValueAnonSetConfig out_vasc;
        Litesaver::ValueAnonConfig out_vac;
        out_os.is_unique = true;
        out_vasc.clear();
        out_vac.description      = "ha1 float stuff";
        out_vac.value_type       = Litesaver::ValueType::FLOAT;
        out_vasc["a1"]           = out_vac;
        out_vac.description      = "ha2 text stuff";
        out_vac.value_type       = Litesaver::ValueType::TEXT;
        out_vasc["a2"]           = out_vac;
        out_vac.description      = "ha3 int stuff";
        out_vac.value_type       = Litesaver::ValueType::INTEGER;
        out_vasc["a3"]           = out_vac;
        out_vac.description      = "ha4 blob stuff";
        out_vac.value_type       = Litesaver::ValueType::BLOB;
        out_vasc["a4"]           = out_vac;
        out_os.content           = out_vasc;
        output_config["unique1"] = out_os;
        out_os.is_unique         = false;
        out_vasc.clear();
        out_vac.description         = "hb1 float stuff";
        out_vac.value_type          = Litesaver::ValueType::FLOAT;
        out_vasc["b1"]              = out_vac;
        out_vac.description         = "hb2 text stuff";
        out_vac.value_type          = Litesaver::ValueType::TEXT;
        out_vasc["b2"]              = out_vac;
        out_vac.description         = "hb3 int stuff";
        out_vac.value_type          = Litesaver::ValueType::INTEGER;
        out_vasc["b3"]              = out_vac;
        out_vac.description         = "hb4 blob stuff";
        out_vac.value_type          = Litesaver::ValueType::BLOB;
        out_vasc["b4"]              = out_vac;
        out_os.content              = out_vasc;
        output_config["nonunique1"] = out_os;

        Litesaver::Base litesaver("./Litesaver_save_unique_test.db",
                                  input_config,
                                  output_config);

        litesaver.update_unique("unique1", "a1", static_cast<int64_t>(-37));
        litesaver.update_unique("unique1", "a2", 0.3454);
        litesaver.update_unique("unique1", "a3", "more,textß");
        std::vector<std::uint8_t> blobval2;
        blobval2.push_back(5);
        blobval2.push_back(5);
        blobval2.push_back(0xe7);
        blobval2.push_back(4);
        blobval2.push_back(5);
        blobval2.push_back(0xff);
        blobval2.push_back(6);
        blobval2.push_back(0xe7);
        litesaver.update_unique("unique1", "a4", blobval2);
    }
}

TEST(Litesaver, save_unique_test_then_null)
{
    {
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);

        Litesaver::TableConfig input_config;
        Litesaver::TableConfig output_config;

        Litesaver::AnonSegment in_os;
        Litesaver::ValueAnonSetConfig in_vasc;
        Litesaver::ValueAnonConfig in_vac;
        in_os.is_unique = true;
        in_vasc.clear();
        in_vac.description      = "in_ha1 float stuff";
        in_vac.value_type       = Litesaver::ValueType::FLOAT;
        in_vasc["in_a1"]        = in_vac;
        in_vac.description      = "in_ha2 text stuff";
        in_vac.value_type       = Litesaver::ValueType::TEXT;
        in_vasc["in_a2"]        = in_vac;
        in_vac.description      = "in_ha3 int stuff";
        in_vac.value_type       = Litesaver::ValueType::INTEGER;
        in_vasc["in_a3"]        = in_vac;
        in_vac.description      = "in_ha4 blob stuff";
        in_vac.value_type       = Litesaver::ValueType::BLOB;
        in_vasc["in_a4"]        = in_vac;
        in_os.content           = in_vasc;
        input_config["unique1"] = in_os;
        in_os.is_unique         = false;
        in_vasc.clear();
        in_vac.description         = "in_hb1 float stuff";
        in_vac.value_type          = Litesaver::ValueType::FLOAT;
        in_vasc["in_b1"]           = in_vac;
        in_vac.description         = "in_hb2 text stuff";
        in_vac.value_type          = Litesaver::ValueType::TEXT;
        in_vasc["in_b2"]           = in_vac;
        in_vac.description         = "in_hb3 int stuff";
        in_vac.value_type          = Litesaver::ValueType::INTEGER;
        in_vasc["in_b3"]           = in_vac;
        in_vac.description         = "in_hb4 blob stuff";
        in_vac.value_type          = Litesaver::ValueType::BLOB;
        in_vasc["in_b4"]           = in_vac;
        in_os.content              = in_vasc;
        input_config["nonunique1"] = in_os;

        Litesaver::AnonSegment out_os;
        Litesaver::ValueAnonSetConfig out_vasc;
        Litesaver::ValueAnonConfig out_vac;
        out_os.is_unique = true;
        out_vasc.clear();
        out_vac.description      = "ha1 float stuff";
        out_vac.value_type       = Litesaver::ValueType::FLOAT;
        out_vasc["a1"]           = out_vac;
        out_vac.description      = "ha2 text stuff";
        out_vac.value_type       = Litesaver::ValueType::TEXT;
        out_vasc["a2"]           = out_vac;
        out_vac.description      = "ha3 int stuff";
        out_vac.value_type       = Litesaver::ValueType::INTEGER;
        out_vasc["a3"]           = out_vac;
        out_vac.description      = "ha4 blob stuff";
        out_vac.value_type       = Litesaver::ValueType::BLOB;
        out_vasc["a4"]           = out_vac;
        out_os.content           = out_vasc;
        output_config["unique1"] = out_os;
        out_os.is_unique         = false;
        out_vasc.clear();
        out_vac.description         = "hb1 float stuff";
        out_vac.value_type          = Litesaver::ValueType::FLOAT;
        out_vasc["b1"]              = out_vac;
        out_vac.description         = "hb2 text stuff";
        out_vac.value_type          = Litesaver::ValueType::TEXT;
        out_vasc["b2"]              = out_vac;
        out_vac.description         = "hb3 int stuff";
        out_vac.value_type          = Litesaver::ValueType::INTEGER;
        out_vasc["b3"]              = out_vac;
        out_vac.description         = "hb4 blob stuff";
        out_vac.value_type          = Litesaver::ValueType::BLOB;
        out_vasc["b4"]              = out_vac;
        out_os.content              = out_vasc;
        output_config["nonunique1"] = out_os;

        Litesaver::Base litesaver("./Litesaver_save_unique_test_then_null.db",
                                  input_config,
                                  output_config);

        litesaver.update_unique("unique1", "a1", static_cast<int64_t>(-37));
        litesaver.update_unique("unique1", "a2", 0.3454);
        litesaver.update_unique("unique1", "a3", "more,textß");
        std::vector<std::uint8_t> blobval2;
        blobval2.push_back(5);
        blobval2.push_back(5);
        blobval2.push_back(0xe7);
        blobval2.push_back(4);
        blobval2.push_back(5);
        blobval2.push_back(0xff);
        blobval2.push_back(6);
        blobval2.push_back(0xe7);
        litesaver.update_unique("unique1", "a4", blobval2);
        litesaver.update_unique_with_null("unique1", "a2");
    }
}