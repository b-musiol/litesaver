/**
 * Litesaver
 * SQLite based utility to save and retrieve data for input (e.g. config),
 * logging and output (e.g. results).
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#ifndef _LITESAVER_HPP_
#define _LITESAVER_HPP_

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>


namespace Litesaver
{

enum ValueType
{
    FLOAT,
    INTEGER,
    TEXT,
    BLOB
};

typedef std::
    variant<std::int64_t, double, std::string, std::vector<std::uint8_t>>
        value_t;

struct ValueConfig
{
    value_t value; // default value
    std::string description;
};
struct ValueAnonConfig
{
    ValueType value_type;
    std::string description;
};

typedef std::map<std::string, ValueAnonConfig> ValueAnonSetConfig;

struct OutputSegment
{
    ValueAnonSetConfig content;
    bool is_unique;
};


typedef std::map<std::string, ValueConfig> InputConfig;
typedef std::map<std::string, OutputSegment> OutputConfig;

class Base
{
  public:
    /**
     * Assumes this is a Database at `file_path` that has previously been
     * assembled and extracts the configs from it. Optionally opens it in a
     * `fast_mode` and allows to be used multithreaded with
     * `multithread_enable`.
     */
    Base(std::filesystem::path file_path,
         bool fast_mode          = false,
         bool multithread_enable = false);
    /**
     * Assumes this is a fresh Database at `file_path`. If one exists, it is
     * overwritten and reset entirely! Optionally opens it in a `fast_mode` and
     * allows to be used multithreaded with `multithread_enable`.
     */
    Base(std::filesystem::path file_path,
         InputConfig input_config,
         OutputConfig output_config,
         bool fast_mode          = false,
         bool multithread_enable = false);
    ~Base();

  private:
    struct Core;

  private:
    std::unique_ptr<Core> core;
};

} // namespace Litesaver

#endif // _LITESAVER_HPP_