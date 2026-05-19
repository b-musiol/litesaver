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
        Litesaver::InputConfig input_config;
        Litesaver::OutputConfig output_config;

        Litesaver::ValueConfig vc;
        vc.value                    = 1;
        vc.description              = "some integer";
        input_config["intstuff"]    = vc;
        vc.value                    = 45.5;
        vc.description              = "some real";
        input_config["floatstuff"]  = vc;
        vc.value                    = "jahoo u";
        vc.description              = "some text";
        input_config["stringstuff"] = vc;
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);
        vc.value                  = blobval;
        vc.description            = "some blob";
        input_config["blobstuff"] = vc;

        Litesaver::OutputAnonSegment os;
        Litesaver::ValueAnonSetConfig vasc;
        Litesaver::ValueAnonConfig vac;
        os.is_unique = true;
        vasc.clear();
        vac.description          = "ha1 float stuff";
        vac.value_type           = Litesaver::ValueType::FLOAT;
        vasc["a1"]               = vac;
        vac.description          = "ha2 text stuff";
        vac.value_type           = Litesaver::ValueType::TEXT;
        vasc["a2"]               = vac;
        vac.description          = "ha3 int stuff";
        vac.value_type           = Litesaver::ValueType::INTEGER;
        vasc["a3"]               = vac;
        vac.description          = "ha4 blob stuff";
        vac.value_type           = Litesaver::ValueType::BLOB;
        vasc["a4"]               = vac;
        os.content               = vasc;
        output_config["unique1"] = os;
        os.is_unique             = false;
        vasc.clear();
        vac.description             = "hb1 float stuff";
        vac.value_type              = Litesaver::ValueType::FLOAT;
        vasc["b1"]                  = vac;
        vac.description             = "hb2 text stuff";
        vac.value_type              = Litesaver::ValueType::TEXT;
        vasc["b2"]                  = vac;
        vac.description             = "hb3 int stuff";
        vac.value_type              = Litesaver::ValueType::INTEGER;
        vasc["b3"]                  = vac;
        vac.description             = "hb4 blob stuff";
        vac.value_type              = Litesaver::ValueType::BLOB;
        vasc["b4"]                  = vac;
        os.content                  = vasc;
        output_config["nonunique1"] = os;

        Litesaver::Base litesaver("./Litesaver_init_from_scratch.db",
                                  input_config,
                                  output_config);
    }
}

TEST(Litesaver, test_log)
{
    {
        Litesaver::InputConfig input_config;
        Litesaver::OutputConfig output_config;

        Litesaver::ValueConfig vc;
        vc.value                    = 1;
        vc.description              = "some integer";
        input_config["intstuff"]    = vc;
        vc.value                    = 45.5;
        vc.description              = "some real";
        input_config["floatstuff"]  = vc;
        vc.value                    = "jahoo u";
        vc.description              = "some text";
        input_config["stringstuff"] = vc;
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);
        vc.value                  = blobval;
        vc.description            = "some blob";
        input_config["blobstuff"] = vc;

        Litesaver::OutputAnonSegment os;
        Litesaver::ValueAnonSetConfig vasc;
        Litesaver::ValueAnonConfig vac;
        os.is_unique = true;
        vasc.clear();
        vac.description          = "ha1 float stuff";
        vac.value_type           = Litesaver::ValueType::FLOAT;
        vasc["a1"]               = vac;
        vac.description          = "ha2 text stuff";
        vac.value_type           = Litesaver::ValueType::TEXT;
        vasc["a2"]               = vac;
        vac.description          = "ha3 int stuff";
        vac.value_type           = Litesaver::ValueType::INTEGER;
        vasc["a3"]               = vac;
        vac.description          = "ha4 blob stuff";
        vac.value_type           = Litesaver::ValueType::BLOB;
        vasc["a4"]               = vac;
        os.content               = vasc;
        output_config["unique1"] = os;
        os.is_unique             = false;
        vasc.clear();
        vac.description             = "hb1 float stuff";
        vac.value_type              = Litesaver::ValueType::FLOAT;
        vasc["b1"]                  = vac;
        vac.description             = "hb2 text stuff";
        vac.value_type              = Litesaver::ValueType::TEXT;
        vasc["b2"]                  = vac;
        vac.description             = "hb3 int stuff";
        vac.value_type              = Litesaver::ValueType::INTEGER;
        vasc["b3"]                  = vac;
        vac.description             = "hb4 blob stuff";
        vac.value_type              = Litesaver::ValueType::BLOB;
        vasc["b4"]                  = vac;
        os.content                  = vasc;
        output_config["nonunique1"] = os;

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

TEST(Litesaver, input_pull_test)
{
    {
        Litesaver::InputConfig input_config;
        Litesaver::OutputConfig output_config;

        Litesaver::ValueConfig vc;
        vc.value                    = 1;
        vc.description              = "some integer";
        input_config["intstuff"]    = vc;
        vc.value                    = 45.5;
        vc.description              = "some real";
        input_config["floatstuff"]  = vc;
        vc.value                    = "jahoo u";
        vc.description              = "some text";
        input_config["stringstuff"] = vc;
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);
        vc.value                  = blobval;
        vc.description            = "some blob";
        input_config["blobstuff"] = vc;

        Litesaver::OutputAnonSegment os;
        Litesaver::ValueAnonSetConfig vasc;
        Litesaver::ValueAnonConfig vac;
        os.is_unique = true;
        vasc.clear();
        vac.description          = "ha1 float stuff";
        vac.value_type           = Litesaver::ValueType::FLOAT;
        vasc["a1"]               = vac;
        vac.description          = "ha2 text stuff";
        vac.value_type           = Litesaver::ValueType::TEXT;
        vasc["a2"]               = vac;
        vac.description          = "ha3 int stuff";
        vac.value_type           = Litesaver::ValueType::INTEGER;
        vasc["a3"]               = vac;
        vac.description          = "ha4 blob stuff";
        vac.value_type           = Litesaver::ValueType::BLOB;
        vasc["a4"]               = vac;
        os.content               = vasc;
        output_config["unique1"] = os;
        os.is_unique             = false;
        vasc.clear();
        vac.description             = "hb1 float stuff";
        vac.value_type              = Litesaver::ValueType::FLOAT;
        vasc["b1"]                  = vac;
        vac.description             = "hb2 text stuff";
        vac.value_type              = Litesaver::ValueType::TEXT;
        vasc["b2"]                  = vac;
        vac.description             = "hb3 int stuff";
        vac.value_type              = Litesaver::ValueType::INTEGER;
        vasc["b3"]                  = vac;
        vac.description             = "hb4 blob stuff";
        vac.value_type              = Litesaver::ValueType::BLOB;
        vasc["b4"]                  = vac;
        os.content                  = vasc;
        output_config["nonunique1"] = os;

        Litesaver::Base litesaver("./Litesaver_input_pull_test.db",
                                  input_config,
                                  output_config);

        auto intstuff = litesaver.get_input("intstuff");
        EXPECT_EQ(std::get<std::int64_t>(intstuff), 1);
        auto floatstuff = litesaver.get_input("floatstuff");
        EXPECT_EQ(std::get<double>(floatstuff), 45.5);
        auto stringstuff = litesaver.get_input("stringstuff");
        EXPECT_EQ(std::get<std::string>(stringstuff), "jahoo u");
        auto blobstuff = litesaver.get_input("blobstuff");
        EXPECT_EQ(std::get<std::vector<std::uint8_t>>(blobstuff), blobval);
    }
}

TEST(Litesaver, save_unique_test)
{
    {
        Litesaver::InputConfig input_config;
        Litesaver::OutputConfig output_config;

        Litesaver::ValueConfig vc;
        vc.value                    = 1;
        vc.description              = "some integer";
        input_config["intstuff"]    = vc;
        vc.value                    = 45.5;
        vc.description              = "some real";
        input_config["floatstuff"]  = vc;
        vc.value                    = "jahoo u";
        vc.description              = "some text";
        input_config["stringstuff"] = vc;
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);
        vc.value                  = blobval;
        vc.description            = "some blob";
        input_config["blobstuff"] = vc;

        Litesaver::OutputAnonSegment os;
        Litesaver::ValueAnonSetConfig vasc;
        Litesaver::ValueAnonConfig vac;
        os.is_unique = true;
        vasc.clear();
        vac.description          = "ha1 float stuff";
        vac.value_type           = Litesaver::ValueType::FLOAT;
        vasc["a1"]               = vac;
        vac.description          = "ha2 text stuff";
        vac.value_type           = Litesaver::ValueType::TEXT;
        vasc["a2"]               = vac;
        vac.description          = "ha3 int stuff";
        vac.value_type           = Litesaver::ValueType::INTEGER;
        vasc["a3"]               = vac;
        vac.description          = "ha4 blob stuff";
        vac.value_type           = Litesaver::ValueType::BLOB;
        vasc["a4"]               = vac;
        os.content               = vasc;
        output_config["unique1"] = os;
        os.is_unique             = false;
        vasc.clear();
        vac.description             = "hb1 float stuff";
        vac.value_type              = Litesaver::ValueType::FLOAT;
        vasc["b1"]                  = vac;
        vac.description             = "hb2 text stuff";
        vac.value_type              = Litesaver::ValueType::TEXT;
        vasc["b2"]                  = vac;
        vac.description             = "hb3 int stuff";
        vac.value_type              = Litesaver::ValueType::INTEGER;
        vasc["b3"]                  = vac;
        vac.description             = "hb4 blob stuff";
        vac.value_type              = Litesaver::ValueType::BLOB;
        vasc["b4"]                  = vac;
        os.content                  = vasc;
        output_config["nonunique1"] = os;

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
        Litesaver::InputConfig input_config;
        Litesaver::OutputConfig output_config;

        Litesaver::ValueConfig vc;
        vc.value                    = 1;
        vc.description              = "some integer";
        input_config["intstuff"]    = vc;
        vc.value                    = 45.5;
        vc.description              = "some real";
        input_config["floatstuff"]  = vc;
        vc.value                    = "jahoo u";
        vc.description              = "some text";
        input_config["stringstuff"] = vc;
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);
        vc.value                  = blobval;
        vc.description            = "some blob";
        input_config["blobstuff"] = vc;

        Litesaver::OutputAnonSegment os;
        Litesaver::ValueAnonSetConfig vasc;
        Litesaver::ValueAnonConfig vac;
        os.is_unique = true;
        vasc.clear();
        vac.description          = "ha1 float stuff";
        vac.value_type           = Litesaver::ValueType::FLOAT;
        vasc["a1"]               = vac;
        vac.description          = "ha2 text stuff";
        vac.value_type           = Litesaver::ValueType::TEXT;
        vasc["a2"]               = vac;
        vac.description          = "ha3 int stuff";
        vac.value_type           = Litesaver::ValueType::INTEGER;
        vasc["a3"]               = vac;
        vac.description          = "ha4 blob stuff";
        vac.value_type           = Litesaver::ValueType::BLOB;
        vasc["a4"]               = vac;
        os.content               = vasc;
        output_config["unique1"] = os;
        os.is_unique             = false;
        vasc.clear();
        vac.description             = "hb1 float stuff";
        vac.value_type              = Litesaver::ValueType::FLOAT;
        vasc["b1"]                  = vac;
        vac.description             = "hb2 text stuff";
        vac.value_type              = Litesaver::ValueType::TEXT;
        vasc["b2"]                  = vac;
        vac.description             = "hb3 int stuff";
        vac.value_type              = Litesaver::ValueType::INTEGER;
        vasc["b3"]                  = vac;
        vac.description             = "hb4 blob stuff";
        vac.value_type              = Litesaver::ValueType::BLOB;
        vasc["b4"]                  = vac;
        os.content                  = vasc;
        output_config["nonunique1"] = os;

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