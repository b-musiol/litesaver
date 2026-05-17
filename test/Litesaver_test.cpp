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
        vc.value = 1;
        vc.description = "some integer";
        input_config["intstuff"] = vc;
        vc.value = 45.5;
        vc.description = "some real";
        input_config["floatstuff"] = vc;
        vc.value = "jahoo u";
        vc.description = "some text";
        input_config["stringstuff"] = vc;
        std::vector<std::uint8_t> blobval;
        blobval.push_back(4);
        blobval.push_back(5);
        blobval.push_back(0);
        blobval.push_back(6);
        blobval.push_back(0xe7);
        vc.value = blobval;
        vc.description = "some blob";
        input_config["blobstuff"] = vc;

        Litesaver::OutputSegment os;
        Litesaver::ValueAnonSetConfig vasc;
        Litesaver::ValueAnonConfig vac;
        os.is_unique = true;
        vasc.clear();
        vac.description = "ha1 float stuff";
        vac.value_type = Litesaver::ValueType::FLOAT;
        vasc["a1"] = vac;
        vac.description = "ha2 text stuff";
        vac.value_type = Litesaver::ValueType::TEXT;
        vasc["a2"] = vac;
        vac.description = "ha3 int stuff";
        vac.value_type = Litesaver::ValueType::INTEGER;
        vasc["a3"] = vac;
        vac.description = "ha4 blob stuff";
        vac.value_type = Litesaver::ValueType::BLOB;
        vasc["a4"] = vac;
        os.content = vasc;
        output_config["unique1"] = os;
        os.is_unique = false;
        vasc.clear();
        vac.description = "hb1 float stuff";
        vac.value_type = Litesaver::ValueType::FLOAT;
        vasc["b1"] = vac;
        vac.description = "hb2 text stuff";
        vac.value_type = Litesaver::ValueType::TEXT;
        vasc["b2"] = vac;
        vac.description = "hb3 int stuff";
        vac.value_type = Litesaver::ValueType::INTEGER;
        vasc["b3"] = vac;
        vac.description = "hb4 blob stuff";
        vac.value_type = Litesaver::ValueType::BLOB;
        vasc["b4"] = vac;
        os.content = vasc;
        output_config["nonunique1"] = os;



        Litesaver::Base litesaver("./Litesaver_init_from_scratch.db",
                                  input_config,
                                  output_config);
    }
}