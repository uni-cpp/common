////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/ErrorCode.hpp
/// @brief Declaration ErrorCode class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace uni
{
namespace common
{
enum class ErrorCode
{
    NONE,
    TIMEOUT,

    UNDEFINED
};


}  // namespace common

using common::ErrorCode;

}  // namespace uni