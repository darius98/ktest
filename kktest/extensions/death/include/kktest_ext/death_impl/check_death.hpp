#ifndef KKTEST_EXTENSIONS_DEATH_INCLUDE_KKTEST_EXT_DEATH_IMPL_CHECK_DEATH_HPP_
#define KKTEST_EXTENSIONS_DEATH_INCLUDE_KKTEST_EXT_DEATH_IMPL_CHECK_DEATH_HPP_

#include <kktest_ext/death_impl/death_status.hpp>

namespace kktest {
namespace death {

KKTEST_EXPORT DeathStatus checkDeath(const Executable& func,
                                     double timeTicksLimit=1);

}
}

#endif