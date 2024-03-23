//
// Created by César B. on 3/22/2024.
//

#include "utils.hpp"
#include <assert.h>
#include <io.h>

int utils::find_last_eq_byte(utils::byte_t byte, utils::byte_t * buffer, size_t buflen)
{	int result = -1;
	for(size_t i = 0; i < buflen; ++i)
		if(buffer[i] == byte)
			result = i;
	return result;
}

FILE * utils::redirect_stdout(int * out_original_stdout)
{	// @note : fdid means file descriptor ID

	// create a copy of stdout's current file descriptor
	int stdout_fdid = fileno(stdout); assert(stdout_fdid != -1);
	int original_stdout_copy_fdid = dup(stdout_fdid); assert(stdout_fdid != -1);

	// write our copy's id to **out_original_stdout**
	assert(out_original_stdout);
	*out_original_stdout = original_stdout_copy_fdid;

	// create a temporary file
	FILE * tmp_file = tmpfile(); assert(tmp_file);
	int tmp_file_fdid = fileno(tmp_file); assert(tmp_file_fdid != -1);

	// copy our temporary file's descriptor into stdout's
	int result_fdid = dup2(tmp_file_fdid, stdout_fdid); assert(result_fdid == tmp_file_fdid);

	return tmp_file;
}

void utils::restore_stdout(FILE * tmp_file, int original_stdout)
{	// obtain stdout's current file descriptor ID
	int stdout_fdid = fileno(stdout);
	// copy our original stdout copy file descriptor into the current file descriptor of stdout
	int result_fdid = dup2(original_stdout, stdout_fdid);
	assert(result_fdid == original_stdout);
	// delete the temporary file
	fclose(tmp_file);
}
