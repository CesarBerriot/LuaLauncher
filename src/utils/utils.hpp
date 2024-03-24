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
	int find_first_eq_byte(byte_t byte, byte_t * buffer, size_t buflen);

	/// @return index of the found byte, else -1
	int find_last_eq_byte(byte_t byte, byte_t * buffer, size_t buflen);

	/// used by **redirect_stdout** and **restore_stdout**
	struct redirected_stdout
	{	/// a pipe, for more info lookup pipe() on the linux manual
		int pipe[2];
		/// file descriptor id of a copy of the original file descriptor of stdout
		int original_stdout;
	};

	/// @see redirect_stdout
	redirected_stdout redirect_stdout();

	/// simplifies a posix read() on the read file descriptor of a redirected_stdout's pipe
	int read_redirected_stdout(redirected_stdout, char buffer[], size_t buflen);

	/// @see redirect_stdout
	void restore_stdout(redirected_stdout);
}
