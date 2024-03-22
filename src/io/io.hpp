#pragma once

#include <string>
#include <vector>

namespace io
{
	std::vector<std::string> obtain_script_list();
	void run_script(std::string file_name);
	std::string get_full_scripts_path();
}