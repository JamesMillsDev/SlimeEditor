#pragma once

#include <concepts>

template<class T, class U>
concept derived = std::is_base_of_v<U, T>;

#include "Types.h"

constexpr byte BYTE_MAX = 255;