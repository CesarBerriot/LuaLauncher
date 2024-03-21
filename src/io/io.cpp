#include "io.hpp"
#include "ui/ui.hpp"
#include <sstream>
#include <dirent.h>
#include <limits.h>

void io::run_script(std::string file_name)
{	std::stringstream stream;
	stream << "Running Script '" << file_name << ".lua'";
	ui::log(stream.str());
}

std::vector<std::string> io::obtain_script_list()
{	// obtain full path of scripts directory
	char cwd_buffer[PATH_MAX];
	getcwd(cwd_buffer, PATH_MAX);
	std::stringstream cwd_stream;
	cwd_stream << cwd_buffer << "\\scripts";

	// open scripts folder
	DIR * dir_stream = opendir(cwd_stream.str().c_str());
	if(!dir_stream)
	{	ui::log("Folder 'scripts' doesn't exist !");
		return {};
	}

	// obtain lua file list
	std::vector<std::string> result;
	dirent * current_entry;
	while(current_entry = readdir(dir_stream))
	{	// sub-string len, length of ".lua"
		enum { slen = 4 };

		// can't contain .lua
		if(current_entry->d_namlen < 4)
			continue;

		// doesn't contain .lua
		if(strcmp(".lua", &current_entry->d_name[current_entry->d_namlen - slen]))
			continue;

		// remove file extension
		std::string file_name(current_entry->d_name);
		file_name.resize(file_name.size() - slen);
		// add this entry to the result vector
		result.push_back(file_name);
	}
	return result;
}
