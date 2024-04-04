//
// Created by César B. on 3/22/2024.
//

#include "utils.hpp"
#include <assert.h>
#include <io.h>
#ifdef WIN32
	#define REDIRECTED_STDOUT_FILE_NAME "redirected_stdout.txt"
#endif

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
{
	#ifdef WIN32
	// make a file for stdout to be redirected in
	FILE * file = fopen(REDIRECTED_STDOUT_FILE_NAME, "w+");
	assert(file);
	int fclose_res = fclose(file);
	assert(!fclose_res);

	// reopen stdout to our file
	FILE * freopen_res = freopen(REDIRECTED_STDOUT_FILE_NAME, "w+", stdout);
	assert(freopen_res == stdout);

	return {};
	#else
	// @note : fdid means file descriptor id

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
	#endif
}

void utils::restore_stdout(utils::redirected_stdout redirected_stdout)
{
	#ifdef WIN32
	// reopen stdout to the terminal's output stream, or just close it if the terminal doesn't exist
	if(GetConsoleWindow())
	{	FILE * freopen_res = freopen("CONOUT$", "w", stdout);
		assert(freopen_res == stdout);
	}
	else
	{	int fclose_res = fclose(stdout);
		assert(!fclose_res);
	}

	// delete our temporary file, as its no longer needed
	int remove_res = remove(REDIRECTED_STDOUT_FILE_NAME);
	assert(!remove_res);
	#else
	// @note : fdid means file descriptor id

	int e;

	// close the pipe
	e = close(redirected_stdout.pipe[0]); assert(!e);
	e = close(redirected_stdout.pipe[1]); assert(!e);

	// overwrite stdout
	int stdout_fdid = fileno(stdout); assert(stdout_fdid);
	e = dup2(redirected_stdout.original_stdout, stdout_fdid); assert(!e);

	// destroy our copy of stdout's original file descriptor
	e = close(redirected_stdout.original_stdout); assert(!e);
	#endif
}

int utils::read_redirected_stdout(utils::redirected_stdout redirected_stdout, char * buffer, size_t buflen)
{
	#if WIN32
	// determine stdout's length & reset its cursor
	int fseek_res = fseek(stdout, 0, SEEK_END);
	assert(!fseek_res);
	int len = ftell(stdout);
	fseek_res = fseek(stdout, 0, SEEK_SET);
	assert(!fseek_res);

	// ensure the given buffer's length fits stdout
	if(buflen < len)
		return -1;

	// if stdout contains more than just '\0', read it into our buffer and return how many bytes were read
	if(len > 1)
	{	int fread_res = fread(buffer, 1, len, stdout);
		assert(fread_res == len - 1);
		buffer[fread_res] = 0;
		return fread_res;
	}
	// else zero-terminate our buffer and return 0, as 0 bytes were read
	else
	{	buffer[0] = '\0';
		return 0;
	}

	#else
	// dirty hack that fixes infinite read() problems on some terminals
	putchar(' ');
	fflush(stdout);
	// posix read
	int r = read(redirected_stdout.pipe[0], buffer, buflen - 1);
	// zero-termination of the read data
	if(r != -1)
		buffer[r] = '\0';
	return r;
	#endif
}
