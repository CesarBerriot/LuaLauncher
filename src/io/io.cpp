#include "io.hpp"
#include "ui/ui.hpp"
#include "macros/macros.hpp"
#include <sstream>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <libgen.h>
#include <Windows.h>

std::vector<std::string> io::obtain_script_list()
{	// obtain full path of scripts directory
	std::string scripts_path = get_full_scripts_path();

	// open scripts folder
	DIR * dir_stream = opendir(scripts_path.c_str());
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

std::string io::get_full_scripts_path()
{	char cwd_buffer[PATH_MAX];
	getcwd(cwd_buffer, PATH_MAX);
	return make_string(cwd_buffer << "\\scripts");
}

std::string io::read_script(std::string file_name, bool use_executor_log)
{	// operator ""s
	using namespace std::string_literals;

	try
	{	if(!file_name.length())
			throw "Script Read Error : Empty file name"s;

		// determine the file's full path
		std::string path = make_string(get_full_scripts_path() << '\\' << file_name << ".lua");

		// open the file fo read operations
		FILE * file = fopen(path.c_str(), "r");
		if(!file)
			throw make_string("Script Read Error : Couldn't open file : '" << path << '\'');

		// determine the file's size
		size_t file_size;
		fseek(file, 0, SEEK_END);
		file_size = ftell(file);
		fseek(file, 0, SEEK_SET);
		if(!file_size)
			throw make_string("Script Read Error : Script '" << path << "' appears to be empty");

		// reserve the required buffer space and read into it
		std::string result;
		result.resize(file_size);
		fread((void *)result.c_str(), 1, file_size, file);

		return result;
	}
	catch(std::string error_msg)
	{	// @note : I wanted to use a ternary exp on function pointers
		// but c++ is fucking retarded so that's that
		if(use_executor_log)
			ui::executor_log(error_msg);
		else
			ui::log(error_msg);
		return {};
	}
}

void io::add_script(std::string full_path)
{	// operator ""s
	using namespace std::string_literals;

	std::string file_name;

	try
	{	if(!full_path.length())
			throw "Empty script path"s;

		file_name = basename((char *)full_path.c_str());
		std::string copy_path = make_string(io::get_full_scripts_path() << '\\' << file_name);
		std::string a = "make_string(io::get_full_scripts_path() << '\\' << file_name);"s;

		bool copy_result = CopyFileA(full_path.c_str(), copy_path.c_str(), true);
		if(!copy_result)
			switch(GetLastError())
			{	case ERROR_FILE_NOT_FOUND:
					throw "Invalid source file path"s;
				case ERROR_ACCESS_DENIED:
					throw "Access to the destination file denied"s;
				case ERROR_ENCRYPTION_FAILED:
					throw "Bitlocker encryption of the destination file failed"s;
				default:
					throw "A script with an identical file name already exists within the scripts folder"s;
			}

		ui::log(make_string("Successfully Imported File '" << file_name.c_str() << '\''));
	}
	catch(std::string error_msg)
	{	std::string file_name_str = file_name.length() ? make_string(" (" << file_name << ')') : std::string();
		ui::log(make_string("Script Import Error" << file_name_str << " : " << error_msg));
	}
}
