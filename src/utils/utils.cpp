//
// Created by César B. on 3/22/2024.
//

#include "utils.hpp"

int utils::find_last_eq_byte(utils::byte_t byte, utils::byte_t * buffer, size_t buflen)
{	int result = -1;
	for(size_t i = 0; i < buflen; ++i)
		if(buffer[i] == byte)
			result = i;
	return result;
}
