//
// Created by César B. on 3/22/2024.
//

#pragma once

#include <stdint.h>
#include <stdio.h>

namespace utils
{
	typedef char byte_t;

	/// @return index of the found byte, else -1
	int find_last_eq_byte(byte_t byte, byte_t * buffer, size_t buflen);

	/**
	 * @brief redirects stdout to a new temporary file
	 * @param out_original_stdout contains a copy of stdout's original file descriptor
	 * @return a temporary file that'll be written into instead of stdout
	 */
	FILE * redirect_stdout(int * out_original_stdout);

	/// @see redirect_stdout
	void restore_stdout(FILE * tmp_file, int original_stdout);
}
