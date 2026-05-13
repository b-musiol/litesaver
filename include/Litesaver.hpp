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
#include <memory>

namespace Litesaver
{
class Base
{
  public:
    Base(std::filesystem::path,
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