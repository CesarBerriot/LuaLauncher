//
// Created by César B. on 3/22/2024.
//

#pragma once

#include <stdint.h>

namespace utils
{
	typedef char byte_t;
	/// @return index of the found byte, else -1
	int find_last_eq_byte(byte_t byte, byte_t * buffer, size_t buflen);
}
