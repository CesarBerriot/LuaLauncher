//
// Created by César B. on 3/22/2024.
//

#include "utils.hpp"
#include <assert.h>
#include <io.h>

int utils::find_first_eq_byte(utils::byte_t byte, utils::byte_t * buffer, size_t buflen)
{	int result = -1;
	for(int i = 0; i < buflen; ++i)
		if(buffer[i] == byte)
			return i;
	return 0;
}

int utils::find_last_eq_byte(utils::byte_t byte, utils::byte_t * buffer, size_t buflen)
{	int result = -1;
	for(int i = 0; i < buflen; ++i)
		if(buffer[i] == byte)
			result = i;
	return result;
}

utils::redirected_stdout utils::redirect_stdout()
{	// @note : fdid means file descriptor id

	int e;
	utils::redirected_stdout result;

	// create a copy of stdout's current file descriptor
	int stdout_fdid = fileno(stdout); assert(stdout_fdid != -1);
	result.original_stdout = dup(stdout_fdid); assert(result.original_stdout != -1);

	// create a new pipe
	e = _pipe(result.pipe, 0, 0); assert(!e);

	// overwrite stdout
	e = dup2(result.pipe[1], stdout_fdid); assert(!e);

	return result;
}

void utils::restore_stdout(utils::redirected_stdout redirected_stdout)
{	// @note : fdid means file descriptor id

	int e;

	// close the pipe
	e = close(redirected_stdout.pipe[0]); assert(!e);
	e = close(redirected_stdout.pipe[1]); assert(!e);

	// overwrite stdout
	int stdout_fdid = fileno(stdout); assert(stdout_fdid);
	e = dup2(redirected_stdout.original_stdout, stdout_fdid); assert(!e);

	// destroy our copy of stdout's original file descriptor
	e = close(redirected_stdout.original_stdout); assert(!e);
}

int utils::read_redirected_stdout(utils::redirected_stdout redirected_stdout, char * buffer, size_t buflen)
{	// dirty hack that fixes infinite read() problems due to microsoft's posix implementations being absolute shit
	putchar(' ');
	fflush(stdout);
	// posix read
	int r = read(redirected_stdout.pipe[0], buffer, buflen - 1);
	// zero-termination of the read data
	if(r != -1)
		buffer[r] = '\0';
	return r;
}
