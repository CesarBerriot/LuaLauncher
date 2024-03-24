#pragma once

#include <string>
#include <vector>

namespace io
{
	std::vector<std::string> obtain_script_list();
	std::string get_full_scripts_path();
	/**
	 * @brief reads and returns code from a script file
	 * @param file_name script file name, no extension, no path
	 * @return code, or an empty string on error (empty files are considered to be an error)
	 */
	std::string read_script(std::string file_name, bool use_executor_log = false);
	// copies a script from wherever into the scripts folder, doesn't refresh ui
	void add_script(std::string full_path);
}