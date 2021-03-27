////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/Constants.hpp
/// @brief List of common constants.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

namespace uni
{
namespace common
{
constexpr uint32_t DAYS_IN_WEEK{ 7U };
constexpr uint32_t HOURS_IN_DAY{ 24U };
constexpr uint32_t MINITES_IN_HOUR{ 60U };
constexpr uint32_t SECONDS_IN_MINUTE{ 60U };

constexpr uint64_t DEFAULT_TIMEOUT_MS{ 50U };

}  // namespace common
}  // namespace uni
